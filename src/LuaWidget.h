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

#ifndef LUAWIDGET_H
#define LUAWIDGET_H

#include "Widget.h"
#include "LuaBindings.h"

namespace Rangers
{
class LuaWidget: public Widget
{
public:
    //TODO: Remove this
    LuaWidget(const std::wstring& fileName, Object *parent = 0);
    LuaWidget(const char *data, size_t size, const std::string& name, Object *parent = 0);
    LuaWidget(Object *parent = 0);

    void draw();

    Rect getBoundingRect();
    void mouseMove(int x, int y);
    void mouseEnter();
    void mouseLeave();
    void keyPressed(SDL_keysym key);

    void mouseDown(uint8_t key, int x, int y);
    void mouseUp(uint8_t key, int x, int y);
    void mouseClick(int x, int y);

    void processMain();
    void processLogic(int dt);

private:
    lua_State *m_luaState;
};
}

#endif // LUAWIDGET_H
