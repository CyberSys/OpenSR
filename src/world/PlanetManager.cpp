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

#include "PlanetManager.h"
#include "Types.h"
#include <libRanger.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Log.h>
#include <OpenSR/JSONHelper.h>
#include <OpenSR/Engine.h>
#include <OpenSR/Texture.h>
#include <json/value.h>

namespace Rangers
{
namespace World
{
PlanetManager& PlanetManager::instance()
{
    static PlanetManager manager;
    return manager;
}


PlanetManager::PlanetManager()
{
    std::wstring stylesPath = fromUTF8(Engine::instance().properties()->get<std::string>("world.planetStyles", "ORC/planets.json").c_str());
    if (!stylesPath.empty())
        loadStyles(stylesPath);
}

//TODO: Planet rings
/*! This function must be run in OpenGL thread (main thread) !*/
boost::shared_ptr<Texture> PlanetManager::getPlanetImage(boost::shared_ptr<PlanetStyle> style, int size)
{
    if (!style)
        return boost::shared_ptr<Texture>();

    boost::shared_ptr<Texture> texture;
    boost::shared_ptr<Texture> cloud;

    texture = ResourceManager::instance().loadTexture(style->texture);

    if (!texture)
        return boost::shared_ptr<Texture>();

    if (style->hasCloud)
        cloud = ResourceManager::instance().loadTexture(style->cloud);

    unsigned char *tex = new unsigned char[texture->width() * texture->height() * 4];
    unsigned char *cld;
    unsigned char *img = new unsigned char[size * size * 4];

    if (style->hasCloud && cloud)
    {
        cld = new unsigned char[cloud->width() * cloud->height() * 4];
        glBindTexture(GL_TEXTURE_2D, cloud->openGLTexture());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_BYTE, cld);
    }


    glBindTexture(GL_TEXTURE_2D, texture->openGLTexture());
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_BYTE, tex);


    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            float tx = j / (float(size) - 1.0f) - 0.5f;
            float ty = i / (float(size) - 1.0f) - 0.5f;
            float tr = tx * tx + ty * ty;

            float x = sqrt(0.25f - tx * tx - ty * ty);
            float r = sqrt(x * x + tx * tx + ty * ty);
            float lambda = atan(tx / x);
            float theta = acos(-ty / r);

            int tex_x = (0.5f + (1.0f / (2.0f * M_PI) * lambda)) * (texture->width() - 1);
            int tex_y = (0.5f - cos(theta) / 2.0f) * (texture->height() - 1);
            if (tr <= 0.25 && tex_x < texture->width() && tex_y < texture->height() && tex_x >= 0 && tex_y >= 0)
            {
                float a = tex[(tex_y * texture->width() + tex_x) * 4 + 3] / 255.0f;
                float b = tex[(tex_y * texture->width() + tex_x) * 4 + 2] / 255.0f;
                float g = tex[(tex_y * texture->width() + tex_x) * 4 + 1] / 255.0f;
                float r = tex[(tex_y * texture->width() + tex_x) * 4] / 255.0f;
                float y = 0.299f * r + 0.587f * g + 0.114f * b;
                float u = -0.14713f * r - 0.28886f * g + 0.436f * b;
                float v = 0.615f * r - 0.51499f * g - 0.10001f * b;
                y = y * (0.5f + a / 2.0f);
                r = y + 1.13983f * v;
                g = y - 0.39465f * u - 0.58060f * v;
                b = y + 2.03211f * u;
                a = 1.0f;

                if (style->hasCloud && cloud)
                {
                    int cld_x = tex_x * (float(cloud->width()) / texture->width());
                    int cld_y = tex_y * (float(cloud->height()) / texture->height());
                    float ca = cld[(cld_y * cloud->width() + cld_x) * 4 + 3] / 255.0f;
                    float cb = cld[(cld_y * cloud->width() + cld_x) * 4 + 2] / 255.0f;
                    float cg = cld[(cld_y * cloud->width() + cld_x) * 4 + 1] / 255.0f;
                    float cr = cld[(cld_y * cloud->width() + cld_x) * 4] / 255.0f;

                    float outa = ca + a * (1.0f - ca);
                    float outr = (cr * ca + r * a * (1 - ca));
                    float outg = (cg * ca + g * a * (1 - ca));
                    float outb = (cb * ca + b * a * (1 - ca));

                    r = outr;
                    g = outg;
                    b = outb;
                    a = outa;
                }

                r *= 1.3f + ((style->ambientColor >> 16) & 0xff) / 255.0f * 0.5f;
                g *= 1.3f + ((style->ambientColor >> 8) & 0xff) / 255.0f * 0.5f;
                b *= 1.3f + (style->ambientColor & 0xff) / 255.0f * 0.5f;

                if (tr >= 0.23)
                    a = (0.25f - tr) * 50.0f;
                else
                    a = 1.0f;

                r = r > 1.0f ? 1.0f : r < 0.0f ? 0.0f : r;
                g = g > 1.0f ? 1.0f : g < 0.0f ? 0.0f : g;
                b = b > 1.0f ? 1.0f : b < 0.0f ? 0.0f : b;
                ((uint32_t*)img)[i * size + j] = ((char(a * 255.0) << 24) & 0xff000000) | ((char(b * 255.0) << 16) & 0xff0000) |
                                                 ((char(g * 255.0) << 8) & 0xff00) | (char(r * 255.0) & 0xff);
            }
            else
                ((uint32_t*)img)[i * size + j] = 0;
        }
    }
    Texture *result = new Texture(size, size, TEXTURE_R8G8B8A8, img);
    delete[] img;
    delete[] tex;
    return boost::shared_ptr<Texture>(result);
}

boost::shared_ptr<PlanetStyle> PlanetManager::style(const std::wstring& name)
{
    uint32_t hash = textHash32(name);
    return style(hash);
}

boost::shared_ptr<PlanetStyle> PlanetManager::style(uint32_t id)
{
    return m_styles.at(id);
}

void PlanetManager::loadStyles(const std::wstring& styleFile)
{
    boost::shared_ptr<std::istream> json = ResourceManager::instance().getFileStream(styleFile);
    if (!json)
        return;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(*json, root))
    {
        Log::error() << "Error parsing planet styles JSON: " << reader.getFormatedErrorMessages();
        return;
    }
    if (!root.isObject())
    {
        Log::error() << "Invalid planet styles JSON";
        return;
    }
    Json::Value::Members members = root.getMemberNames();
    Json::Value::Members::const_iterator end = members.end();
    for (Json::Value::Members::const_iterator i = members.begin(); i != end; ++i)
    {
        Json::Value jsonStyle = root[(*i)];

        if (!jsonStyle.isObject())
        {
            Log::error() << "Invalid planet styles JSON";
            return;
        }
        bool error = false;
        PlanetStyle *style = new PlanetStyle();
        style->ambientColor = JSONHelper::parseColor(jsonStyle.get("ambientColor", "#FFFFFF").asString(), error);
        style->texture = fromUTF8(jsonStyle.get("texture", "").asCString());
        style->speed = jsonStyle.get("speed", 0.0f).asDouble();
        style->hasCloud = jsonStyle.get("hasCloud", false).asBool();
        style->id = fromUTF8((*i).c_str());
        style->hasRing = jsonStyle.get("hasRing", false).asBool();
        style->hasRingBackground = jsonStyle.get("hasRingBackground", false).asBool();

        if (style->hasCloud)
        {
            style->cloud = fromUTF8(jsonStyle.get("cloud", "").asCString());
            style->cloudSpeed = jsonStyle.get("cloudSpeed", 0.0f).asDouble();
        }
        if (style->hasRing)
        {
            style->ring = fromUTF8(jsonStyle.get("ring", "").asCString());
            style->ringOffsetX = jsonStyle.get("ringOffsetX", 0.0f).asDouble();
            style->ringOffsetY = jsonStyle.get("ringOffsetY", 0.0f).asDouble();
        }
        if (style->hasRingBackground)
        {
            style->ringBackground = fromUTF8(jsonStyle.get("ringBackground", "").asCString());
            style->ringBgOffsetX = jsonStyle.get("ringBgOffsetX", 0.0f).asDouble();
            style->ringBgOffsetY = jsonStyle.get("ringBgOffsetY", 0.0f).asDouble();
        }
        m_styles[textHash32(style->id)] = boost::shared_ptr<PlanetStyle>(style);
    }
    Log::info() << "Loaded " << members.size() << " planet styles";
}
}
}
