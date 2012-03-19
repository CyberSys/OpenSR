/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_STYLES_H
#define RANGERS_STYLES_H

#include "Types.h"
#include <boost/variant.hpp>

namespace Rangers
{
class Texture;
struct ButtonStyle;
struct ScrollBarStyle;
struct FontDescriptor;

struct FontDescriptor
{
    std::wstring path;
    int size;
};

struct ResourceDescriptor
{
    enum {NONE, NINEPATCH, SPRITE, FONT} type;
    typedef boost::variant<TextureRegion, NinePatchDescriptor, FontDescriptor> Resource;
    Resource resource;
};

struct ButtonStyle
{
    ResourceDescriptor normal;
    ResourceDescriptor hovered;
    ResourceDescriptor pressed;
    int color;
    FontDescriptor font;
    Rect contentRect;
};

struct ScrollBarStyle
{
    ButtonStyle upButton;
    ButtonStyle scroll;
    ButtonStyle downButton;
};

struct Skin
{
    ButtonStyle buttonStyle;
    ScrollBarStyle scrollStyle;
};

}

#endif
