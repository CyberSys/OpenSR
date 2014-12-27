/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_PLANET_H
#define WORLD_PLANET_H

#include "OpenSR/World/SystemObject.h"

namespace Rangers
{
namespace World
{
class RANGERS_WORLD_API Planet: public SystemObject
{
public:
    Planet(uint64_t id = 0);

    float radius() const;
    float orbit() const;
    uint32_t style() const;
    float angle() const;
    float angleSpeed() const;

    void setRadius(float radius);
    void setOrbit(float orbit);
    void setStyle(uint32_t style);
    void setStyle(const std::string& style);
    void setAngle(float angle);
    void setAngleSpeed(float speed);

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);
    virtual void calcTurn();
    virtual void finishTurn();
    virtual void turn(float progress);

protected:
    void updatePosition();

    float m_radius;
    float m_orbit;
    float m_angle;
    float m_angleSpeed;
    uint32_t m_style;

    float m_startAngle;
};
}
}

#endif