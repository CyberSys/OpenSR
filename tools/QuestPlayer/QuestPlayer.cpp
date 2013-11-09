/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "QuestPlayer.h"

#include "Parser.h"

#include <fstream>

#include <QStringList>
#include <QDebug>
#include <QRegularExpression>

namespace Rangers
{
namespace QuestPlayer
{

QuestPlayer::QuestPlayer(QObject *parent): QObject(parent)
{
}

QuestPlayer::~QuestPlayer()
{

}

QM::Location QuestPlayer::currentLocation() const
{
    return m_currentLocation;
}

QString QuestPlayer::substituteValues(const QString &str) const
{
    //TODO: Use <regex>
    QString result = str;
    QRegularExpression valueExp("\\[p(\\d)+\\]");
    QRegularExpression expExp("\\{(.+?)\\}");
    int deltaPos = 0;

    QRegularExpressionMatchIterator i = expExp.globalMatch(str);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString value = "<font color=\"blue\">" + QString::number(eval(match.captured(1), m_parameters)) + "</font>";
        result.replace(match.capturedStart() + deltaPos, match.capturedLength(), value);
        deltaPos += value.length() - match.capturedLength();
    }
    deltaPos = 0;
    QString str2 = result;
    i = valueExp.globalMatch(str2);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString value = "<font color=\"blue\">" + QString::number(eval(match.captured(0), m_parameters)) + "</font>";
        result.replace(match.capturedStart() + deltaPos, match.capturedLength(), value);
        deltaPos += value.length() - match.capturedLength();
    }

    result.replace("<ToStar>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.toStar) + "</font>");
    result.replace("<ToPlanet>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.toPlanet) + "</font>");
    result.replace("<FromStar>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.fromStar) + "</font>");
    result.replace("<FromPlanet>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.fromPlanet) + "</font>");
    result.replace("<Date>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.date) + "</font>");
    result.replace("<Money>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.money) + "</font>");
    result.replace("<Ranger>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.ranger) + "</font>");

    result.replace("<clr>", "<font color=\"blue\">");
    result.replace("<clrEnd>", "</font>");

    return result;
}

void QuestPlayer::loadQuest(const QString& file)
{
    std::ifstream f(file.toStdString());
    m_quest = Rangers::QM::readQuest(f);

    resetQuest();
}

void QuestPlayer::resetQuest()
{
    m_parametersVisibility.clear();
    m_parameters.clear();
    m_transitionCounts.clear();
    m_locationDescriptionsCount.clear();

    for (const std::pair<uint32_t, QM::Parameter>& p : m_quest.parameters)
    {
        QString start = QString::fromStdWString(p.second.start);
        int32_t value = eval(start, std::map<uint32_t, int32_t>());
        m_parameters[p.first] = value;
        m_parametersVisibility[p.first] = true;
    }

    setLocation(m_quest.startLocation);
}

void QuestPlayer::setLocation(uint32_t location)
{
    if (m_quest.locations.find(location) == m_quest.locations.end())
    {
        qWarning() << "Invalid location: " << location;
        return;
    }
    m_currentLocation = m_quest.locations.at(location);

    m_oldParameters = m_parameters;

    for (const QM::Modifier & m : m_currentLocation.modifiers)
    {
        applyModifier(m);
    }

    checkTransitions();
    reduceTransitions();

    if (!m_currentLocation.empty)
    {
        if (m_currentLocation.descriptionExpression && !m_currentLocation.expression.empty())
        {
            int32_t t = eval(QString::fromStdWString(m_currentLocation.expression), m_parameters);
            if ((t > 10) || (m_currentLocation.descriptions.at(t - 1).empty()))
            {
                qCritical() << "Invalid location description selection in location " << m_currentLocation.id;
                m_locationText = "";
            }
            else
                m_locationText = substituteValues(QString::fromStdWString(m_currentLocation.descriptions.at(t - 1)));
        }
        else
        {
            uint32_t value;
            if (m_locationDescriptionsCount.find(m_currentLocation.id) == m_locationDescriptionsCount.end())
            {
                m_locationDescriptionsCount[m_currentLocation.id] = 0;
                value = 0;
            }
            else
            {
                value = m_locationDescriptionsCount[m_currentLocation.id];
                value = (value + 1) % 10;
            }

            for (int i = 0; (i < 10) && (m_currentLocation.descriptions.at(value).empty()); i++)
                value = (value + 1) % 10;

            m_locationDescriptionsCount[m_currentLocation.id] = value;

            m_locationText = substituteValues(QString::fromStdWString(m_currentLocation.descriptions.at(value)));
        }
    }

    qDebug() << "L" << m_currentLocation.id;

    if (m_currentLocation.empty && (m_possibleTransitions.size() == 1) && m_currentLocation.transitions[m_possibleTransitions.front()].title.empty() &&
            m_currentTransition.description.empty())
        startTransition(m_possibleTransitions.front());
    else if (m_currentLocation.type == QM::Location::LOCATION_SUCCESS)
        emit(questCompleted(m_locationText));
    else if (m_currentLocation.type == QM::Location::LOCATION_FAIL)
        emit(questFailed(m_locationText, m_currentLocation.death));
    else if (checkCriticalParameters())
        emit(locationChanged());
}

bool QuestPlayer::checkCriticalParameters()
{
    for (const std::pair<uint32_t, int32_t>& p : m_parameters)
    {
        const QM::Parameter &pr = m_quest.parameters.at(p.first);

        if (pr.type == QM::Parameter::PARAMETER_NORMAL)
            continue;

        bool crit = false;

        if (pr.minCritical && p.second <= pr.min)
            crit = true;
        else if (!pr.minCritical && p.second >= pr.max)
            crit = true;

        if (crit)
        {
            switch (pr.type)
            {
            case QM::Parameter::PARAMETER_DEATH:
                emit(questFailed(substituteValues(QString::fromStdWString(pr.critText)), true));
                break;
            case QM::Parameter::PARAMETER_FAIL:
                emit(questFailed(substituteValues(QString::fromStdWString(pr.critText)), false));
                break;
            case QM::Parameter::PARAMETER_SUCCESS:
                emit(questCompleted(substituteValues(QString::fromStdWString(pr.critText))));
                break;
            }
            return false;
        }
    }
    return true;
}

QString QuestPlayer::locationText() const
{
    return m_locationText;
}

QStringList QuestPlayer::visibleParameters() const
{
    QStringList params;
    for (const std::pair<uint32_t, int32_t>& p : m_parameters)
    {
        if ((p.second != 0 || m_quest.parameters.at(p.first).showOnZero) && m_parametersVisibility.at(p.first))
        {
            QString value;
            for (const Rangers::QM::Parameter::Range & r : m_quest.parameters.at(p.first).ranges)
            {
                if ((p.second >= r.from) && (p.second <= r.to))
                {
                    value = QString::fromStdWString(r.text).replace("<>", QString::number(p.second));
                    break;
                }
            }
            params.push_back(substituteValues(value));
        }
    }
    return params;
}

bool QuestPlayer::checkCondition(const QM::Transition::Condition& c) const
{
    int32_t param = m_parameters.at(c.param);

    if ((param < c.rangeFrom) || (param > c.rangeTo))
        return false;

    if (c.param == 2)
    {
        param = param;
    }

    if (c.includeValues)
    {
        if ((c.values.size()) && (std::find(c.values.begin(), c.values.end(), param) == c.values.end()))
            return false;
    }
    else
    {
        if ((c.values.size()) && (std::find(c.values.begin(), c.values.end(), param) != c.values.end()))
            return false;
    }

    if (c.includeMultiples)
    {
        for (int32_t m : c.multiples)
        {
            if ((param % m) != 0)
                return false;
        }
    }
    else
    {
        for (int32_t m : c.multiples)
        {
            if ((param % m) == 0)
                return false;
        }
    }
    return true;
}

void QuestPlayer::checkTransitions()
{
    m_possibleTransitions.clear();

    int i = 0;
    for (const QM::Transition & t : m_currentLocation.transitions)
    {
        bool cond = true;
        for (const QM::Transition::Condition & c : t.conditions)
        {
            cond = cond && checkCondition(c);
        }
        bool passed = (t.passCount == 0) || !((m_transitionCounts.find(t.id) != m_transitionCounts.end()) && (m_transitionCounts.at(t.id) >= t.passCount));
        if (passed && cond && (t.globalCondition.empty() || eval(QString::fromStdWString(t.globalCondition), m_parameters)))
        {
            m_possibleTransitions.push_back(i);
        }
        i++;
    }
}

std::map<uint32_t, QString> QuestPlayer::visibleTransitions()
{
    std::map<uint32_t, QString> r;
    for (uint32_t t : m_possibleTransitions)
    {
        if (!m_currentLocation.transitions.at(t).title.empty())
            r[t] = substituteValues(QString::fromStdWString(m_currentLocation.transitions.at(t).title));
        else
            r[t] = tr("Next");
    }
    return r;
}

void QuestPlayer::applyModifier(const QM::Modifier& m)
{
    switch (m.visibility)
    {
    case QM::Modifier::VISIBILITY_HIDE:
        m_parametersVisibility[m.param] = false;
        break;
    case QM::Modifier::VISIBILITY_SHOW:
        m_parametersVisibility[m.param] = true;
        break;
    case QM::Modifier::VISIBILITY_NO_CHANGE:
        break;
    }

    int32_t value;

    switch (m.operation)
    {
    case QM::Modifier::OPERATION_ASSIGN:
        value = m.value;
        break;
    case QM::Modifier::OPERATION_CHANGE:
        value = m_oldParameters[m.param] + m.value;
        break;
    case QM::Modifier::OPERATION_PERCENT:
        value = m_oldParameters[m.param] + (m.value * m_oldParameters[m.param]) / 100;
        break;
    case QM::Modifier::OPERATION_EXPRESSION:
        if (!m.expression.empty())
            value = eval(QString::fromStdWString(m.expression), m_oldParameters);
        else
            value = m_oldParameters[m.param];
        break;
    }

    m_parameters[m.param] = std::min(value, m_quest.parameters.at(m.param).max);
    m_parameters[m.param] = std::max(m_parameters[m.param], m_quest.parameters.at(m.param).min);
}

void QuestPlayer::startTransition(uint32_t num)
{
    if (m_currentLocation.transitions.size() <= num)
        return;

    m_currentTransition = m_currentLocation.transitions[num];

    m_oldParameters = m_parameters;

    for (const QM::Modifier & m : m_currentTransition.modifiers)
    {
        applyModifier(m);
    }

    qDebug() << "P" << m_currentTransition.id;

    m_locationText = substituteValues(QString::fromStdWString(m_currentTransition.description));

    if (m_quest.locations.at(m_currentTransition.to).empty)
    {
        finishTransition();
    }
    else if (m_currentTransition.description.empty())
    {
        finishTransition();
    }
    else
    {
        emit(transitionText(substituteValues(QString::fromStdWString(m_currentTransition.description))));
    }
}

void QuestPlayer::finishTransition()
{
    if (m_transitionCounts.find(m_currentTransition.id) == m_transitionCounts.end())
    {
        m_transitionCounts[m_currentTransition.id] = 1;
    }
    else
    {
        m_transitionCounts[m_currentTransition.id]++;
    }
    setLocation(m_currentTransition.to);
}

//FIXME: Optimize
void QuestPlayer::reduceTransitions()
{
    std::map<std::wstring, std::list<uint32_t> > transitions;

    for (uint32_t t : m_possibleTransitions)
    {
        const QM::Transition& tr = m_currentLocation.transitions.at(t);
        transitions[tr.title].push_back(t);
    }

    m_possibleTransitions.clear();

    for (std::pair<std::wstring, std::list<uint32_t> > p : transitions)
    {
        if (p.second.size() == 1)
        {
            m_possibleTransitions.push_back(p.second.front());
            continue;
        }
        double range = 0;

        for (uint32_t t : p.second)
        {
            range += m_currentLocation.transitions.at(t).priority;
        }

        uint32_t randRange = range * 1000;
        uint32_t r = rand() % randRange;
        uint32_t counter = 0;
        uint32_t selected = 0;

        for (uint32_t t : p.second)
        {
            counter += m_currentLocation.transitions.at(t).priority * 1000;
            selected = t;
            if (counter >= r)
                break;
        }
        m_possibleTransitions.push_back(selected);
    }
}
}
}