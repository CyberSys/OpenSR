/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_AFT_FONT_H
#define RANGERS_AFT_FONT_H

#include "OpenSR/Font.h"
#include "OpenSR/libRanger.h"
#include <map>

namespace Rangers
{
class AFTFont: public Font
{
public:
    AFTFont(const AFT& font);
    ~AFTFont();

    virtual boost::shared_ptr<Texture> renderText(const std::string& text, int width = 0) const;
    virtual boost::shared_ptr<Texture> renderColoredText(const std::string& text, int defaultTextColor = 0xFFFFFF, int selectionTextColor = 0xFFFFFF, int width = 0) const;

    virtual int calculateStringWidth(const std::string& string, int from, int length) const;
    virtual int maxChars(const std::string& string, int from, int length, int width) const;

private:
    AFT m_aft;
    std::map<wchar_t, AFTGlyph*> m_charMap;
};
}

#endif
