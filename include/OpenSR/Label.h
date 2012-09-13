/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_LABEL_H
#define RANGERS_LABEL_H

#include "Sprite.h"

namespace Rangers
{
class Font;
class Object;
class LabelPrivate;
class RANGERS_ENGINE_API Label: public Sprite
{
    RANGERS_DECLARE_PRIVATE(Label)
public:
    Label(Object *parent = 0);
    Label(const std::string& text, Object *parent = 0, boost::shared_ptr<Font> font = boost::shared_ptr<Font>(), SpriteXOrigin xpos = POSITION_X_LEFT, SpriteYOrigin ypos = POSITION_Y_TOP);
    Label(const std::wstring& text, Object *parent = 0, boost::shared_ptr<Font> font = boost::shared_ptr<Font>(), SpriteXOrigin xpos = POSITION_X_LEFT, SpriteYOrigin ypos = POSITION_Y_TOP);
    Label(const Label& other);

    void setText(const std::string& text);
    void setText(const std::wstring& text);
    void setFont(boost::shared_ptr<Font> font);

    std::wstring text() const;
    boost::shared_ptr<Font> font() const;

    void setWordWrap(bool wordWrap);
    void setFixedSize(float w, float h);

    virtual void processMain();

    Label& operator=(const Label& other);

protected:
    Label(LabelPrivate &p, Object *parent = 0);
    Label(LabelPrivate &p, const Label& other);

};
}

#endif
