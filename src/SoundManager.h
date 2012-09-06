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

#ifndef RANGERS_SOUNDMANAGER_H
#define RANGERS_SOUNDMANAGER_H

#include "config.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace Rangers
{
class Sound;
class RANGERS_ENGINE_API SoundManager
{
public:
    SoundManager();
    virtual ~SoundManager();
    static SoundManager* instance();
    
    boost::shared_ptr<Sound> loadSound(const std::wstring& path);
    
private:
    static SoundManager *m_soundInstance;
    std::map<std::wstring, boost::shared_ptr<Sound> > m_soundCache;
};
}

#endif // RANGERS_SOUNDMANAGER_H
