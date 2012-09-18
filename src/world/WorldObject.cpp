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

#include "WorldObject.h"
#include "WorldHelper.h"
#include "WorldManager.h"

namespace Rangers
{
namespace World
{
WorldObject::WorldObject(uint64_t id): m_objectID(id)
{
    if (!id)
        m_objectID = WorldManager::getNextId();
}

bool WorldObject::deserialize(std::istream& stream)
{
    return true;
}

uint64_t WorldObject::id() const
{
    return m_objectID;
}

bool WorldObject::serialize(std::ostream& stream) const
{
    return true;
}

std::list<uint64_t> WorldObject::dependencies()
{
    return std::list<uint64_t>();
}

uint32_t WorldObject::type() const
{
    return WorldHelper::TYPE_WORLDOBJECT;
}

}
}
