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
#include "WorldManager.h"

#include "WorldObject.h"
#include "SolarSystem.h"


namespace Rangers
{
namespace World
{
namespace Python
{
void exportWorldManager()
{
    using namespace boost::python;

    class_<WorldManager, boost::noncopyable> c("WorldManager", boost::python::no_init);
    c.def("instance", &WorldManager::instance, return_value_policy<reference_existing_object>())
    .staticmethod("instance")
    .def("getNextId", &WorldManager::getNextId)
    .staticmethod("getNextId")
    .def("addObject", &WorldManager::addObject)
    .def("removeObject", (void (WorldManager::*)(boost::shared_ptr<WorldObject>))&WorldManager::removeObject)
    .def("removeObject", (void (WorldManager::*)(uint64_t))&WorldManager::removeObject)
    .def("generateWorld", &WorldManager::generateWorld)
    .def("saveWorld", &WorldManager::saveWorld)
    .def("getObject", &WorldManager::getObject)
    .def("solarSystems", &WorldManager::solarSystems)
    .def("currentSolarSystem", &WorldManager::currentSolarSystem)
    .def("styleManager", &WorldManager::styleManager, return_value_policy<reference_existing_object>());
}
}
}
}