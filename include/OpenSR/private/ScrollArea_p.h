/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_SCROLL_AREA_P_H
#define RANGERS_SCROLL_AREA_P_H

#include "Widget_p.h"

#include "Button.h"
#include "ActionListener.h"

namespace Rangers
{
class WidgetNode;
class ScrollAreaPrivate: public WidgetPrivate, ActionListener
{
    RANGERS_DECLARE_PUBLIC(ScrollArea)
public:
    ScrollAreaPrivate();

    virtual void actionPerformed(const Action &action);

    Button *top, *bottom, *left, *right;
    Button *vScroll, *hScroll;
    float vSize, vPosition;
    float hSize, hPosition;
    WidgetNode *node;
    float scrollStart;
    enum {NONE, VERTICAL, HORIZONTAL} scrollDrag;
    void updateScrollPosition();
};
}

#endif