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

#ifndef RANGERS_ACTION_H
#define RANGERS_ACTION_H

#include <boost/variant.hpp>
#include "Types.h"
#include "SDL_keysym.h"

namespace Rangers
{
class Widget;
class Action;

class RANGERS_ENGINE_API Action
{
public:
    enum Type {BUTTON_CLICKED, KEY_PRESSED, CHECKBOX_TOGGLED};
    typedef boost::variant<std::wstring, Rect, SDL_keysym, bool> Argument;

    Action(Widget *source, Type type, const Argument& argument);

    Widget* source() const;
    Type type() const;
    Argument argument() const;

    static std::wstring getStringArgument(const Action &a);
    static Rect getRectArgument(const Action &a);
    static SDL_keysym getKeyArgument(const Action &a);
    static bool getBoolArgument(const Action &a);

private:
    Widget *m_source;
    Type m_type;
    Argument m_argument;
};
}

#endif
