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
#include "PlanetarySystem.h"

#include "WorldObjectWrap.h"

namespace Rangers
{
namespace World
{
namespace Python
{
struct PlanetarySystemWrap: PlanetarySystem, boost::python::wrapper<PlanetarySystem>
{
    PlanetarySystemWrap(uint64_t id = 0): PlanetarySystem(id)
    {
    }

    WORLD_PYTHON_WRAP_WORLD_OBJECT(PlanetarySystem)
};

void exportPlanetarySystem()
{
    using namespace boost::python;

    class_<PlanetarySystemWrap, bases<WorldObject>, boost::shared_ptr<PlanetarySystemWrap>, boost::noncopyable> c("PlanetarySystem", init<uint64_t>());
    c.def(init<>())
    .def("name", &PlanetarySystem::name)
    .def("position", &PlanetarySystem::position)
    .def("systemObjects", &PlanetarySystem::systemObjects)
    .def("size", &PlanetarySystem::size)
    .def("addObject", &PlanetarySystem::addObject)
    .def("removeObject", &PlanetarySystem::removeObject)
    .def("setName", &PlanetarySystem::setName)
    .def("setPosition", &PlanetarySystem::setPosition)
    .def("setStyle", (void (PlanetarySystem::*)(const std::string&))&PlanetarySystem::setStyle)
    .def("setStyle", (void (PlanetarySystem::*)(uint32_t))&PlanetarySystem::setStyle)
    .def("style", &PlanetarySystem::style)
    .def("setSize", &PlanetarySystem::setSize);
    WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(PlanetarySystem, PlanetarySystemWrap, c);
    register_ptr_to_python<boost::shared_ptr<PlanetarySystem> >();
}
}
}
}