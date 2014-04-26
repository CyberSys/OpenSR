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
#include <OpenSR/ColorLabel.h>
#include <OpenSR/Font.h>

#include "OpenSR/python/SpriteWrap.h"

namespace Rangers
{
namespace Python
{
struct ColorLabelWrap : ColorLabel, boost::python::wrapper<ColorLabel>
{
    ColorLabelWrap()
        : ColorLabel()
    {
    }
    ColorLabelWrap(const std::string& text, boost::shared_ptr<Font> font = boost::shared_ptr<Font>())
        : ColorLabel(text, font)
    {
    }
    RANGERS_PYTHON_WRAP_SPRITE(ColorLabel)
};

void exportColorLabel()
{
    using namespace boost::python;

    class_<ColorLabelWrap, bases<Label>, boost::shared_ptr<ColorLabelWrap>, boost::noncopyable> c("ColorLabel", init<const std::string&>());
    c.def(init<>())
    .add_property("textColor", &ColorLabel::textColor, &ColorLabel::setTextColor)
    .add_property("selectionColor", &ColorLabel::selectionColor, &ColorLabel::setSelectionColor);
    RANGERS_PYTHON_WRAP_SPRITE_DEF(ColorLabel, ColorLabelWrap, c);
    register_ptr_to_python<boost::shared_ptr<ColorLabel> >();
}
}
}