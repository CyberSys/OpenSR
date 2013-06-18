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

#include <boost/python.hpp>
#include "Ship.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct ShipWrap: Ship, boost::python::wrapper<Ship>
{
    ShipWrap(uint64_t id = 0): Ship(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(Ship)
};

void exportShip()
{
    using namespace boost::python;

    class_<ShipWrap, bases<SpaceObject>, boost::noncopyable> c("Ship", init<uint64_t>());
    c.def(init<>())
    .def("name", &Ship::name)
    .def("setName", &Ship::setName)
    .def("context", &Ship::context)
    .def("setContext", &Ship::setContext);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(Ship, ShipWrap, c);
}
}
}
}