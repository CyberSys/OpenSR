/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2014 Kosyak <ObKo@mail.ru>

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

#include "SystemWidget.h"
#include "SystemPlanetWidget.h"
#include "PlanetarySystem.h"
#include "Planet.h"
#include "WorldManager.h"
#include "InfoWidget.h"
#include "WorldHelper.h"
#include "Asteroid.h"

#include <cmath>
#include <OpenSR/AnimatedSprite.h>
#include <OpenSR/Sprite.h>
#include <OpenSR/Engine.h>
#include <OpenSR/ActionListener.h>
#include <OpenSR/Action.h>
#include <OpenSR/WidgetNode.h>
#include <OpenSR/Button.h>
#include <OpenSR/AnimatedTexture.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/SpriteWidget.h>
#include <OpenSR/Button.h>
#include <OpenSR/TiledBezierCurve.h>

namespace
{
const int TURN_INTERVAL = 3000;
}

namespace Rangers
{
namespace World
{
class SystemWidget::SystemWidgetListener: public ActionListener
{
public:
    SystemWidgetListener(SystemWidget *parent): m_parent(parent)
    {
    }

    virtual void actionPerformed(const Action &action)
    {
        if (action.source() == m_parent->m_turnButton)
        {
            if (action.type() == Action::BUTTON_CLICKED)
            {
                if (!m_parent->m_turn)
                {
                    WorldManager::instance().calcTurn();
                    m_parent->m_turn = true;
                    m_parent->m_turnTime = 0;
                }
            }
        }
        else
        {
            if (action.type() == Action::MOUSE_LEAVE)
            {
                m_parent->m_infoWidget->clear();
                m_parent->m_infoWidget->setVisible(false);
                m_parent->hideTrajectory();
            }
            if (action.type() == Action::MOUSE_ENTER)
            {
                if (boost::shared_ptr<SpaceObjectWidget> w = boost::dynamic_pointer_cast<SpaceObjectWidget>(action.source()))
                {
                    if (boost::shared_ptr<Asteroid> a = boost::dynamic_pointer_cast<Asteroid>(w->object()))
                    {
                        m_parent->m_infoWidget->show(a);
                        m_parent->m_infoWidget->setVisible(true);
                        Trajectory t = w->object()->trajectory();
                        m_parent->showTrajectory(t);
                    }
                    else if (boost::shared_ptr<Planet> p = boost::dynamic_pointer_cast<Planet>(w->object()))
                    {
                        m_parent->m_infoWidget->show(p);
                        m_parent->m_infoWidget->setVisible(true);
                    }
                }
                else if (action.source() == m_parent->m_starWidget)
                {
                    m_parent->m_infoWidget->show(m_parent->m_system);
                    m_parent->m_infoWidget->setVisible(true);
                }
            }
        }
    }

private:
    SystemWidget *m_parent;
};

SystemWidget::SystemWidget(boost::shared_ptr<PlanetarySystem> system): Widget(),
    m_xOffset(0), m_yOffset(0), m_moveDirection(NONE), m_turn(false), m_turnTime(0)
{
    setWidth(Engine::instance().screenWidth());
    setHeight(Engine::instance().screenHeight());
    setPosition(0, 0);

    m_actionListener = boost::shared_ptr<SystemWidgetListener>(new SystemWidgetListener(this));

    m_node = boost::shared_ptr<WidgetNode>(new WidgetNode());
    addWidget(m_node);

    m_infoWidget = boost::shared_ptr<InfoWidget>(new InfoWidget(WorldManager::instance().skinManager().infoWidgetStyle()));
    m_infoWidget->setPosition(10, 10);
    m_infoWidget->setVisible(false);
    addWidget(m_infoWidget);

    m_turnButton = boost::shared_ptr<Button>(new Button(Engine::instance().defaultSkin()->buttonStyle));
    m_turnButton->setAutoResize(true);
    m_turnButton->setText("Turn");
    m_turnButton->addListener(m_actionListener);
    m_turnButton->setPosition(Engine::instance().screenWidth() - m_turnButton->width() - 20, Engine::instance().screenHeight() - m_turnButton->height() - 20);
    m_turnButton->setLayer(1);
    addWidget(m_turnButton);

    setSystem(system);
}

void SystemWidget::processLogic(int dt)
{
    Widget::processLogic(dt);

    if (m_turn)
    {
        m_turnTime += dt;
        if (m_turnTime >= TURN_INTERVAL)
        {
            WorldManager::instance().finishTurn();
            m_turn = false;
        }
        else
        {
            if (m_system)
                m_system->turn(float(m_turnTime) / TURN_INTERVAL);
        }
    }

    m_node->processLogic(dt);

    switch (m_moveDirection)
    {
    case TOP:
        m_yOffset += (500.0f * dt) / 1000.0f;
        break;
    case BOTTOM:
        m_yOffset -= (500.0f * dt) / 1000.0f;
        break;
    case LEFT:
        m_xOffset += (500.0f * dt) / 1000.0f;
        break;
    case RIGHT:
        m_xOffset -= (500.0f * dt) / 1000.0f;
        break;
    case TOP_LEFT:
        m_yOffset += (500.0f * dt) / 1000.0f;
        m_xOffset += (500.0f * dt) / 1000.0f;
        break;
    case TOP_RIGHT:
        m_yOffset += (500.0f * dt) / 1000.0f;
        m_xOffset -= (500.0f * dt) / 1000.0f;
        break;
    case BOTTOM_LEFT:
        m_yOffset -= (500.0f * dt) / 1000.0f;
        m_xOffset += (500.0f * dt) / 1000.0f;
        break;
    case BOTTOM_RIGHT:
        m_yOffset -= (500.0f * dt) / 1000.0f;
        m_xOffset -= (500.0f * dt) / 1000.0f;
        break;
    default:
        break;
    }
    if (m_bgSprite)
        m_bgSprite->setPosition(m_xOffset / 10 - (m_bgSprite->width() - width()) / 2, m_yOffset / 10 - (m_bgSprite->height() - height()) / 2);
    m_node->setPosition(m_xOffset + width() / 2, m_yOffset + height() / 2);
}

void SystemWidget::setSystem(boost::shared_ptr< PlanetarySystem > system)
{
    if (m_system && (!system || system != m_system))
    {
        removeWidget(m_starWidget);
        removeChild(m_bgSprite);
        m_starWidget = boost::shared_ptr<SpriteWidget>();
        m_bgSprite = boost::shared_ptr<Sprite>();
    }
    m_system = system;

    for (boost::shared_ptr<SystemPlanetWidget> w : m_planetWidgets)
    {
        removeWidget(w);
    }
    m_planetWidgets.clear();

    for (const std::pair<boost::shared_ptr<SpaceObject>, boost::shared_ptr<SpaceObjectWidget> > &p : m_objectWidgets)
    {
        removeWidget(p.second);
    }
    m_objectWidgets.clear();

    if (!m_system)
        return;

    std::list<boost::shared_ptr<SystemObject> > objects = system->systemObjects();

    std::list<boost::shared_ptr<SystemObject> >::const_iterator end = objects.end();
    for (std::list<boost::shared_ptr<SystemObject> >::const_iterator i = objects.begin(); i != end; ++i)
    {
        if (boost::shared_ptr<Planet> planet = boost::dynamic_pointer_cast<Planet>(*i))
        {
            boost::shared_ptr<SystemPlanetWidget> w = boost::shared_ptr<SystemPlanetWidget>(new SystemPlanetWidget(planet));
            w->addListener(m_actionListener);

            m_planetWidgets.push_back(w);
            m_node->addWidget(w);

        }
        else
        {
            boost::shared_ptr<SpaceObjectWidget> w = boost::shared_ptr<SpaceObjectWidget>(new SpaceObjectWidget((*i)));
            m_objectWidgets[(*i)] = w;
            m_node->addWidget(w);
            w->addListener(m_actionListener);
        }

    }

    boost::shared_ptr<SystemStyle> style = WorldManager::instance().styleManager().systemStyle(m_system->style());

    if (style)
    {
        Sprite *s;
        if (!style->starAnimation.empty())
            s = new AnimatedSprite(style->starAnimation);
        else
            s = new Sprite(style->starImage);

        s->setColor(style->color);
        m_starWidget = boost::shared_ptr<SpriteWidget>(new SpriteWidget(boost::shared_ptr<Sprite>(s)));

        m_starWidget->setPosition(-m_starWidget->width() / 2, -m_starWidget->height() / 2);
        m_starWidget->addListener(m_actionListener);
        m_starWidget->setShape(SpriteWidget::SHAPE_CIRCLE);
        m_starWidget->setLayer(-1);
        m_node->addWidget(m_starWidget);
    }

    try
    {
        std::string bg = WorldManager::instance().styleManager().systemBackground(m_system->background());
        m_bgSprite = boost::shared_ptr<Sprite>(new Sprite(bg));
        m_bgSprite->setPosition(m_xOffset / 10 - (m_bgSprite->width() - width()) / 2, m_yOffset / 10 - (m_bgSprite->height() - height()) / 2);
        m_bgSprite->setLayer(-2);
        addChild(m_bgSprite);
    }
    catch (const std::exception &e) // No background
    {
        if (m_bgSprite)
            removeChild(m_bgSprite);
        m_bgSprite =  boost::shared_ptr<Sprite>();
    }
}

boost::shared_ptr< PlanetarySystem > SystemWidget::system() const
{
    return m_system;
}


void SystemWidget::processMain()
{
    Widget::processMain();
}

void SystemWidget::mouseMove(const Vector &p)
{
    if (p.x > width() - 15)
    {
        if (p.y < 15)
            m_moveDirection = TOP_RIGHT;
        else if (p.y > height() - 15)
            m_moveDirection = BOTTOM_RIGHT;
        else
            m_moveDirection = RIGHT;
    }
    else if (p.x < 15)
    {
        if (p.y < 15)
            m_moveDirection = TOP_LEFT;
        else if (p.y > height() - 15)
            m_moveDirection = BOTTOM_LEFT;
        else
            m_moveDirection = LEFT;
    }
    else
    {
        if (p.y < 15)
            m_moveDirection = TOP;
        else if (p.y > height() - 15)
            m_moveDirection = BOTTOM;
        else
            m_moveDirection = NONE;
    }
    Widget::mouseMove(p);
}

void SystemWidget::draw() const
{
    if (!prepareDraw())
        return;

    if (m_bgSprite)
        m_bgSprite->draw();
    m_node->draw();
    m_infoWidget->draw();
    m_turnButton->draw();

    endDraw();
}

void SystemWidget::showTrajectory(const Trajectory& t)
{
    if (m_trajectory.size())
        hideTrajectory();
    for (const BezierCurve & c : t.nextTurns)
    {
        auto o = boost::shared_ptr<TiledBezierCurve>(new TiledBezierCurve("ORC/UnitPath2.png"));
        o->setCurve(c);
        o->setLayer(100);
        m_node->addChild(o);
        m_trajectory.push_back(o);
    }
}

void SystemWidget::hideTrajectory()
{
    for (boost::shared_ptr<TiledBezierCurve> c : m_trajectory)
    {
        m_node->removeChild(c);
    }
    m_trajectory.clear();
}
}
}