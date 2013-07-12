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

#ifndef WORLD_TYPES_H
#define WORLD_TYPES_H

#include <string>

namespace Rangers
{
namespace World
{
struct Point
{
    Point(float _x, float _y);
    Point();

    float x;
    float y;
};
struct PlanetStyle
{
    PlanetStyle();

    std::string id;
    std::wstring texture;
    std::wstring cloud;
    bool hasCloud;
    float speed;
    float cloudSpeed;
    std::wstring ring;
    std::wstring ringBackground;
    bool hasRing;
    bool hasRingBackground;
    int ambientColor;
    float ringOffsetX, ringOffsetY;
    float ringBgOffsetX, ringBgOffsetY;

    bool serialize(std::ostream &stream) const;
    bool deserialize(std::istream &stream);
};
}
}

#endif
