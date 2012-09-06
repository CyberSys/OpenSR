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

#include "SoundManager.h"
#include "Sound.h"
#include <SDL_mixer.h>


namespace Rangers
{
SoundManager* SoundManager::m_soundInstance = 0;

SoundManager::SoundManager()
{
    if (m_soundInstance)
        delete m_soundInstance;

    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
}

SoundManager::~SoundManager()
{
    Mix_CloseAudio();
}

SoundManager* SoundManager::instance()
{
    if (!m_soundInstance)
        m_soundInstance = new SoundManager();

    return m_soundInstance;
}

boost::shared_ptr<Sound> SoundManager::loadSound(const std::wstring& path)
{
    std::map<std::wstring, boost::shared_ptr<Sound> >::const_iterator it = m_soundCache.find(path);
    if (it != m_soundCache.end())
        return it->second;

    boost::shared_ptr<Sound> sound = boost::shared_ptr<Sound>(new Sound(path));
    m_soundCache[path] = sound;
    return sound;
}

}
