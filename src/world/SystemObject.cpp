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

#include "SystemObject.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{
SystemObject::SystemObject(uint64_t id): SpaceObject(id)
{
}

bool SystemObject::deserialize(std::istream& stream)
{
    if (!SpaceObject::deserialize(stream))
        return false;

    WorldHelper::deserializeString(m_name, stream);

    if (!stream.good())
        return false;

    return true;
}

std::string SystemObject::name() const
{
    return m_name;
}

bool SystemObject::serialize(std::ostream& stream) const
{
    if (!SpaceObject::serialize(stream))
        return false;

    uint32_t nameLength = m_name.length();

    stream.write((const char *)&nameLength, sizeof(uint32_t));
    stream.write(m_name.c_str(), m_name.length());

    if (!stream.good())
        return false;

    return true;
}

uint32_t SystemObject::type() const
{
    return WorldHelper::TYPE_SYSTEMOBJECT;
}

void SystemObject::setName(const std::string& name)
{
    m_name = name;
}
}
}
