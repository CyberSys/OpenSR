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

#ifndef RANGERS_BUTTON_H
#define RANGERS_BUTTON_H

#include "OpenSR/Widget.h"

namespace Rangers
{
class ButtonPrivate;
class Font;
class Sound;
struct ButtonStyle;

class RANGERS_ENGINE_API Button: public Widget
{
    RANGERS_DECLARE_PRIVATE(Button);
public:
    Button();
    Button(boost::shared_ptr<Texture> texture);
    Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture);

    Button(const std::wstring& texture);
    Button(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture);

    Button(const ButtonStyle& style);

    virtual ~Button();

    virtual void draw() const;
    virtual void processMain();

    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual int preferredWidth() const;
    virtual int preferredHeight() const;

    void setText(const std::wstring& text);
    virtual void setColor(uint32_t color);
    void setFont(boost::shared_ptr<Font> font);
    void setAutoResize(bool autoResize);

    void setClickSound(boost::shared_ptr<Sound> clickSound);
    void setClickSound(const std::wstring& clickSound);
    void setEnterSound(boost::shared_ptr<Sound> enterSound);
    void setEnterSound(const std::wstring& enterSound);
    void setLeaveSound(boost::shared_ptr<Sound> leaveSound);
    void setLeaveSound(const std::wstring& leaveSound);

    std::wstring text() const;
    virtual uint32_t color() const;
    boost::shared_ptr<Font> font() const;
    bool autoResize() const;
    boost::shared_ptr<Sound> clickSound() const;
    boost::shared_ptr<Sound> enterSound() const;
    boost::shared_ptr<Sound> leaveSound() const;

protected:
    Button(ButtonPrivate &p);

    RANGERS_DISABLE_COPY(Button)
};
}

#endif
