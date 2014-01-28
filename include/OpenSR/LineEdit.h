/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_LINEEDIT_H
#define RANGERS_LINEEDIT_H

#include "OpenSR/Widget.h"

namespace Rangers
{
class Sprite;
class Font;
struct LineEditStyle;
class LineEditPrivate;

class RANGERS_ENGINE_API LineEdit: public Widget
{
    RANGERS_DECLARE_PRIVATE(LineEdit)
public:
    LineEdit();
    LineEdit(float w, float h = 0, boost::shared_ptr<Font> font = boost::shared_ptr<Font>());
    LineEdit(boost::shared_ptr<LineEditStyle> style);

    virtual void draw() const;
    virtual void processMain();
    virtual void processLogic(int dt);

    virtual void mouseMove(const Vector &p);

    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual int preferredHeight() const;
    virtual int maxHeight() const;

    std::wstring text() const;
    void setText(const std::wstring &s);

protected:
    LineEdit(LineEditPrivate &p);

    RANGERS_DISABLE_COPY(LineEdit)
};
}

#endif
