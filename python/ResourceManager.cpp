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

#include <OpenSR/ResourceManager.h>
#include <OpenSR/Texture.h>
#include <OpenSR/AnimatedTexture.h>
#include <OpenSR/Font.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
void exportResourceManager()
{
    using namespace boost::python;
    //class_<std::istream, boost::noncopyable>("stdStream", no_init())
    class_<ResourceManager, boost::noncopyable>("ResourceManager", no_init)
    .def("addRPKG", &ResourceManager::addRPKG)
    .def("addDir", &ResourceManager::addDir)
    .def("addMapping", &ResourceManager::addMapping)
    .def("loadTexture", &ResourceManager::loadTexture)
    .def("loadAnimation", &ResourceManager::loadAnimation)
    .def("loadFont", &ResourceManager::loadFont)
    .def("instance", &ResourceManager::instance, return_value_policy<reference_existing_object>())
    .staticmethod("instance");
}
}
}
