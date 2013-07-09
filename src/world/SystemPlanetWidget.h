/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_SYSTEM_PLANET_WIDGET_H
#define WORLD_SYSTEM_PLANET_WIDGET_H

#include <OpenSR/Widget.h>
#include <OpenSR/Shader.h>
#include <OpenSR/ShaderProgram.h>

namespace Rangers
{
class Sprite;
namespace World
{
class Planet;
class SystemPlanetWidget: public Widget
{
public:
    SystemPlanetWidget(boost::shared_ptr<Planet> planet = boost::shared_ptr<Planet>());
    virtual ~SystemPlanetWidget();

    virtual void draw() const;

    virtual void processMain();
    virtual void processLogic(int dt);

    boost::shared_ptr<Planet> planet() const;

private:
    void updatePosition();

    Sprite *m_staticSprite;
    Sprite *m_ringBgSprite;
    Sprite *m_ringSprite;

    boost::shared_ptr<Planet> m_planet;

    boost::shared_ptr<Texture> m_texture;
    boost::shared_ptr<Texture> m_cloud;

    uint32_t m_ambientColor;
    float m_speed;
    float m_cloudSpeed;
    float m_size;
    float m_phase;
    float m_cloudPhase;
    float m_solarAngle;
    bool m_hasCloud;
    bool m_useShader;
    bool m_hasRing;
    bool m_hasRingBackground;

    GLuint m_vertexBuffer;

    static Shader m_vertexShader;
    static Shader m_fragmentShader;
    static ShaderProgram m_shader;

    static GLint m_phaseLocation;
    static GLint m_cloudPhaseLocation;
    static GLint m_solarAngleLocation;
    static GLint m_ambientColorLocation;
    static GLint m_cloudEnabledLocation;
    static GLint m_textureLocation;
    static GLint m_cloudLocation;
};
}
}
#endif // WORLD_SYSTEM_PLANET_WIDGET_H
