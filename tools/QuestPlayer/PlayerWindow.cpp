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

#include "PlayerWindow.h"
#include "QuestPlayer.h"
#include "ui_PlayerWindow.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtGui/QCursor>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtCore/QtGlobal>

namespace Rangers
{
namespace QuestPlayer
{

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::PlayerWindow)
{
    m_ui->setupUi(this);

    connect(m_ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(loadQuest()));
    connect(&m_player, SIGNAL(locationChanged()), this, SLOT(updateQuest()));
    connect(&m_player, SIGNAL(transitionText(QString)), this, SLOT(showTransition(QString)));
}

PlayerWindow::~PlayerWindow()
{
    delete m_ui;
}

void PlayerWindow::loadQuest()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open quest"));
    if (!file.isEmpty())
        m_player.loadQuest(file);
}

void PlayerWindow::updateQuest()
{
    for (QLabel * b : m_transitionButtons.keys())
    {
        m_ui->buttonLayout->removeWidget(b);
        delete b;
    }
    m_transitionButtons.clear();

    QStringList ps = m_player.visibleParameters();
    m_ui->variableLabel->setText(ps.join("<br>"));
    m_ui->textLabel->setText("<p>" + m_player.locationText().replace("\r\n", "</p><p>") + "</p>");

    std::map<uint32_t, QString> newButtons = m_player.visibleTransitions();
    for (std::pair<uint32_t, QString> p : newButtons)
    {
        QLabel *b = new QLabel(p.second, this);
        b->setWordWrap(true);
        b->setStyleSheet("QLabel {color: blue} QLabel:hover {color: darkblue}");
        b->setCursor(QCursor(Qt::PointingHandCursor));
        b->installEventFilter(this);
        m_ui->buttonLayout->addWidget(b);
        m_transitionButtons[b] = p.first;
    }
}

void PlayerWindow::showTransition(const QString& text)
{
    m_transition = true;
    for (QLabel * b : m_transitionButtons.keys())
    {
        m_ui->buttonLayout->removeWidget(b);
        delete b;
    }
    m_transitionButtons.clear();
    QString t = text;
    QStringList ps = m_player.visibleParameters();
    m_ui->variableLabel->setText(ps.join("<br>"));
    m_ui->textLabel->setText("<p>" + t.replace("\r\n", "</p><p>") + "</p>");

    QLabel *b = new QLabel(tr("Next"), this);
    b->setWordWrap(true);
    b->setStyleSheet("QLabel {color: blue} QLabel:hover {color: darkblue}");
    b->setCursor(QCursor(Qt::PointingHandCursor));
    b->installEventFilter(this);
    m_ui->buttonLayout->addWidget(b);
    m_transitionButtons[b] = 0;
}

bool PlayerWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *m = dynamic_cast<QMouseEvent*>(event);
        if (m->button() == Qt::LeftButton)
        {
            if (m_transition)
            {
                m_player.finishTransition();
                m_transition = false;
            }
            else
            {
                auto t = m_transitionButtons.find(dynamic_cast<QLabel*>(obj));
                if (t != m_transitionButtons.end())
                {
                    m_player.startTransition(*t);
                }
            }
            return true;
        }
        return obj->event(event);
    }
    else
        return obj->event(event);
}

}
}

