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

#ifndef WORLD_SPACE_BASE
#define WORLD_SPACE_BASE

#include "SystemObject.h"
#include "LandContext.h"
#include "ShipContext.h"

namespace Rangers
{
namespace World
{
class SpaceBase: public SystemObject
{
public:
    LandContext landContext() const;
    ShipContext shipContext() const;

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);

protected:
    LandContext m_landContext;
    ShipContext m_shipContext;
};
}
}

#endif