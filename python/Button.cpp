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
#include <OpenSR/Button.h>
#include <OpenSR/Font.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct ButtonWrap : Button, boost::python::wrapper<Button>
{
    ButtonWrap(Widget *parent = 0)
        : Button()
    {
    }
    ButtonWrap(boost::shared_ptr<Texture> texture)
        : Button(texture)
    {
    }
    ButtonWrap(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture)
        : Button(texture, hoverTexture, pressTexture)
    {
    }
    ButtonWrap(const std::wstring& texture)
        : Button(texture)
    {
    }
    ButtonWrap(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture)
        : Button(texture, hoverTexture, pressTexture)
    {
    }
    RANGERS_PYTHON_WRAP_WIDGET(Button)
};

void exportButton()
{
    using namespace boost::python;

    class_<ButtonWrap, bases<Widget>, boost::shared_ptr<ButtonWrap>, boost::noncopyable> c("Button", init<boost::shared_ptr<Texture>, boost::shared_ptr<Texture>, boost::shared_ptr<Texture> >());
    c.def(init<boost::shared_ptr<Texture> >())
    .def(init<const std::wstring&, const std::wstring&, const std::wstring&>())
    .def(init<const std::wstring&>())
    .def(init<>())
    .def("setText", &Button::setText)
    .def("text", &Button::text)
    .def("setFont", &Button::setFont)
    .def("font", &Button::font)
    .def("setAutoResize", &Button::setAutoResize)
    .def("autoResize", &Button::autoResize)
    .def("setSounds", (void (Button::*)(boost::shared_ptr<Sound>, boost::shared_ptr<Sound>, boost::shared_ptr<Sound>))&Button::setSounds)
    .def("setSounds", (void (Button::*)(const std::wstring&, const std::wstring&, const std::wstring&))&Button::setSounds);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(Button, ButtonWrap, c);
    register_ptr_to_python<boost::shared_ptr<Button> >();
}
}
}