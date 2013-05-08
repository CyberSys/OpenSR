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

#include <OpenSR/Object.h>
#include <boost/python.hpp>
#include "Wrappers.h"

namespace Rangers
{
namespace Python
{
struct ObjectWrap: ObjectWrap_<Object>
{
    ObjectWrap(Object *parent = 0): ObjectWrap_<Object>(parent)
    {
    }
};

void exportObject()
{
    using namespace boost::python;
    class_<Python::ObjectWrap, boost::noncopyable> c("Object", init<Object*>());
    c
    .def(init<>())
    .def("setPosition", (void (Object::*)(float, float))&Object::setPosition)
    .def("setPosition", (void (Object::*)(const Vector&))&Object::setPosition)
    .def("setRotation", &Object::setRotation)
    .def("setColor", (void (Object::*)(float, float, float, float))&Object::setColor)
    .def("addChild", &Object::addChild)
    .def("removeChild", &Object::removeChild)
    .def("setParent", &Object::setParent)
    .def("position", &Object::position)
    .def("rotation", &Object::rotation)
    .def("layer", &Object::layer)
    .def("parent", &Object::parent, boost::python::return_internal_reference<>())
    .def("setLayer", &Object::processLogic)
    .def("needUpdate", &Object::needUpdate)
    .def("color", &Object::color)
    .def("mapFromGlobal", &Object::mapFromGlobal)
    .def("mapFromParent", &Object::mapFromParent)
    .def("mapFromSceen", &Object::mapFromScreen)
    .def("mapToGlobal", (Vector(Object::*)(const Vector&) const)&Object::mapToGlobal)
    .def("mapToParent", (Vector(Object::*)(const Vector&) const)&Object::mapToParent)
    .def("mapToSceen", (Vector(Object::*)(const Vector&) const)&Object::mapToScreen)
    .def("mapToGlobal", (Rect(Object::*)(const Rect&) const)&Object::mapToGlobal)
    .def("mapToParent", (Rect(Object::*)(const Rect&) const)&Object::mapToParent)
    .def("mapToSceen", (Rect(Object::*)(const Rect&) const)&Object::mapToScreen);
    ObjectWrap::defWrapped(c);
}
}
}
