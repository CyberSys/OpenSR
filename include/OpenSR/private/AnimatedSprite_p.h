/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_ANIMATED_SPRITE_P_H
#define RANGERS_ANIMATED_SPRITE_P_H

#include "Sprite_p.h"

namespace Rangers
{
class AnimatedSpritePrivate: public SpritePrivate
{
    RANGERS_DECLARE_PUBLIC(AnimatedSprite)
public:
    AnimatedSpritePrivate();

    int m_animationTime;
    int m_currentFrame;
    int m_frameDuration;
    bool m_singleShot;
    bool m_animationStarted;

};
}

#endif