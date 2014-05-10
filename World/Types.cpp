/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#include "Types.h"
#include <OpenSR/libRanger.h>
#include "WorldHelper.h"

namespace
{
const uint32_t PLANET_STYLE_SIGNATURE = *((uint32_t*)"SRPS");
const uint32_t SYSTEM_STYLE_SIGNATURE = *((uint32_t*)"SRSS");
const uint32_t ASTEROID_STYLE_SIGNATURE = *((uint32_t*)"SRAS");
const uint32_t TRAJECTORY_SIGNATURE = *((uint32_t*)"SRTR");
}

namespace Rangers
{
namespace World
{
Point::Point(float _x, float _y)
{
    x = _x;
    y = _y;
}

Point::Point()
{
    x = 0.0f;
    y = 0.0f;
}

PlanetStyle::PlanetStyle()
{
    hasCloud = false;
    speed = 0.0f;
    cloudSpeed = 0.0f;
    hasRing = false;
    hasRingBackground = false;
    ringOffsetX = 0.0f;
    ringOffsetY = 0.0f;
    ringBgOffsetX = 0.0f;
    ringBgOffsetY = 0.0f;
}

SystemStyle::SystemStyle()
{
}

AsteroidStyle::AsteroidStyle(): animated(true)
{

}

Trajectory::Trajectory()
{
}

}
}
