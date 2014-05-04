/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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


#ifndef WORLD_PLANETMANAGER_H
#define WORLD_PLANETMANAGER_H

#include "OpenSR/World/world-global.h"

#include <OpenSR/Types.h>
#include <map>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace Rangers
{
class Texture;
class Shader;
class ShaderProgram;
namespace World
{
struct PlanetStyle;
class RANGERS_WORLD_API PlanetManager
{
public:
    PlanetManager();
    virtual ~PlanetManager();

    //! Get static planet image from planet style
    boost::shared_ptr<Texture> getPlanetImage(boost::shared_ptr<PlanetStyle> style, int size);
    //! Get static planet image from planet style id
    boost::shared_ptr<Texture> getPlanetImage(uint32_t style, int size);

    //! Drop planet image cache
    void dropImageCache();

    bool serialize(std::ostream &stream) const;
    bool deserialize(std::istream &stream);

private:
    PlanetManager(const PlanetManager& other);

    boost::shared_ptr<Texture> renderHardware(int size, boost::shared_ptr<PlanetStyle> style);
    boost::shared_ptr<Texture> renderSoftware(int size, boost::shared_ptr<PlanetStyle> style);

    std::map<uint64_t, boost::shared_ptr<Texture> > m_imageCache;
    boost::shared_ptr<Shader> m_vertexShader;
    boost::shared_ptr<Shader> m_fragmentShader;
    boost::shared_ptr<ShaderProgram> m_shader;
    bool m_useShader;
    GLuint m_vertexBuffer;
    GLuint m_framebuffer;

    GLint m_phaseLocation;
    GLint m_cloudPhaseLocation;
    GLint m_solarAngleLocation;
    GLint m_ambientColorLocation;
    GLint m_cloudEnabledLocation;
    GLint m_textureLocation;
    GLint m_cloudLocation;
    GLint m_texPixelSizeLocation;
    GLint m_pixelSizeLocation;
};
}
}

#endif // WORLD_PLANETMANAGER_H
