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

#include "JSONHelper.h"
#include "Log.h"
#include "Texture.h"
#include <boost/shared_ptr.hpp>
#include "ResourceManager.h"
#include <libRanger.h>
#include <vector>

//TODO: Need error handling.
namespace Rangers
{
TextureRegion JSONHelper::parseTextureRegion(const Json::Value& object, bool &error)
{
    error = false;
    if (!object.isObject())
    {
        Log::warning() << "Invalid JSON object.";
        error = true;
        return TextureRegion(boost::shared_ptr<Texture>(), 0, 0, 0, 0);
    }
    std::wstring textureName = fromUTF8(object.get("texture", "").asString().c_str());
    boost::shared_ptr<Texture> texture;

    if (!textureName.empty())
        texture = ResourceManager::instance()->loadTexture(textureName);

    if (!texture)
        return TextureRegion(boost::shared_ptr<Texture>(), 0, 0, 0, 0);

    int x, y, width, height;

    x = object.get("x", 0).asInt();
    y = object.get("y", 0).asInt();
    width = object.get("width", texture->width()).asInt();
    height = object.get("height", texture->height()).asInt();

    return TextureRegion(texture, x, y, width, height);
}

FontDescriptor JSONHelper::parseFont(const Json::Value& object, bool &error)
{
    error = false;
    FontDescriptor font;
    if (!object.isObject())
    {
        Log::warning() << "Invalid JSON object.";
        error = true;
        return font;
    }
    Json::Value::Members members = object.getMemberNames();
    if ((std::find(members.begin(), members.end(), "file") == members.end())
            || (std::find(members.begin(), members.end(), "size") == members.end()))
    {
        Log::warning() << "Invalid JSON font.";
        error = true;
        return font;
    }
    font.path = fromUTF8(object.get("file", "").asString().c_str());
    font.size = object.get("size", "").asInt();

    return font;
}

NinePatchDescriptor JSONHelper::parseNinePatch(const Json::Value& object, bool &error)
{
    error = false;
    NinePatchDescriptor result;
    result.rows = 0;
    result.columns = 0;
    if (!object.isObject())
    {
        Log::warning() << "Invalid JSON object.";
        error = true;
        return result;
    }
    result.rows = object.get("rows", 0).asInt();
    result.columns = object.get("columns", 0).asInt();
    Json::Value array = object.get("sizeableRows", 0);
    Json::Value::iterator end = array.end();
    for (Json::Value::iterator i = array.begin(); i != end; ++i)
    {
        result.sizeableRows.push_back((*i).asInt());
    }
    array = object.get("sizeableColumns", 0);
    end = array.end();
    for (Json::Value::iterator i = array.begin(); i != end; ++i)
    {
        result.sizeableColumns.push_back((*i).asInt());
    }
    array = object.get("regions", 0);
    end = array.end();
    for (Json::Value::iterator i = array.begin(); i != end; ++i)
    {
        result.regions.push_back(parseTextureRegion(*i, error));
    }
    return result;
}

NinePatchDescriptor JSONHelper::parseNinePatch(const std::string& json, bool &error)
{
    error = false;
    NinePatchDescriptor result;
    result.rows = 0;
    result.columns = 0;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json, root))
    {
        Log::error() << "Error parsing JSON: " << reader.getFormatedErrorMessages();
        error = true;
        return result;
    }
    Json::Value::Members members = root.getMemberNames();
    if (std::find(members.begin(), members.end(), "NinePatch") == members.end())
    {
        Log::error() << "Invalid NinePatch JSON";
        error = true;
        return result;
    }

    return parseNinePatch(*root.get("NinePatch", 0).begin(), error);
}

std::map<std::wstring, ResourceDescriptor> JSONHelper::parseResources(const Json::Value& object, bool &error)
{
    std::map<std::wstring, ResourceDescriptor> result;
    error = false;
    Json::Value::Members members = object.getMemberNames();
    if (std::find(members.begin(), members.end(), "NinePatch") != members.end())
    {
        Json::Value ninepatches = object.get("NinePatch", Json::Value());
        Json::Value::iterator end = ninepatches.end();
        for (Json::Value::iterator it = ninepatches.begin(); it != end; ++it)
        {
            if (result.find(fromUTF8(it.memberName())) != result.end())
            {
                error = true;
                Log::error() << "Dublicated JSON resource: " << fromUTF8(it.memberName());
                return std::map<std::wstring, ResourceDescriptor>();
            }

            NinePatchDescriptor ninepatch = parseNinePatch(*it, error);
            if (error)
            {
                return std::map<std::wstring, ResourceDescriptor>();
            }
            ResourceDescriptor desc;
            desc.type = ResourceDescriptor::NINEPATCH;
            desc.resource = ResourceDescriptor::Resource(ninepatch);
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    if (std::find(members.begin(), members.end(), "Sprite") != members.end())
    {
        Json::Value sprites = object.get("Sprite", Json::Value());
        Json::Value::iterator end = sprites.end();
        for (Json::Value::iterator it = sprites.begin(); it != end; ++it)
        {
            if (result.find(fromUTF8(it.memberName())) != result.end())
            {
                error = true;
                Log::error() << "Dublicated JSON resource: \"" << fromUTF8(it.memberName()) << "\"";
                return std::map<std::wstring, ResourceDescriptor>();
            }

            TextureRegion sprite = parseTextureRegion(*it, error);
            if (error)
            {
                return std::map<std::wstring, ResourceDescriptor>();
            }
            ResourceDescriptor desc;
            desc.type = ResourceDescriptor::SPRITE;
            desc.resource = ResourceDescriptor::Resource(sprite);
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    if (std::find(members.begin(), members.end(), "Font") != members.end())
    {
        Json::Value sprites = object.get("Font", Json::Value());
        Json::Value::iterator end = sprites.end();
        for (Json::Value::iterator it = sprites.begin(); it != end; ++it)
        {
            if (result.find(fromUTF8(it.memberName())) != result.end())
            {
                error = true;
                Log::error() << "Dublicated JSON resource: \"" << fromUTF8(it.memberName()) << "\"";
                return std::map<std::wstring, ResourceDescriptor>();
            }

            FontDescriptor font = parseFont(*it, error);
            if (error)
            {
                return std::map<std::wstring, ResourceDescriptor>();
            }
            ResourceDescriptor desc;
            desc.type = ResourceDescriptor::FONT;
            desc.resource = ResourceDescriptor::Resource(font);
            result[fromUTF8(it.memberName())] = desc;
        }
    }
    return result;
}

Skin JSONHelper::parseSkin(const std::string& json, bool &error)
{
    error = false;
    Skin skin;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json, root))
    {
        Log::error() << "Error parsing JSON: " << reader.getFormatedErrorMessages();
        error = true;
        return Skin();
    }
    std::map<std::wstring, ResourceDescriptor> resources = parseResources(root, error);
    Json::Value::Members members = root.getMemberNames();
    if (std::find(members.begin(), members.end(), "ScrollBarStyle") != members.end())
    {
        Json::Value style = root.get("ScrollBarStyle", Json::Value());
        Json::Value::Members styleMembers = style.getMemberNames();
        if ((std::find(styleMembers.begin(), styleMembers.end(), "up-button") == styleMembers.end())
                || (std::find(styleMembers.begin(), styleMembers.end(), "down-button") == styleMembers.end())
                || (std::find(styleMembers.begin(), styleMembers.end(), "scroll") == styleMembers.end()))
        {
            error = true;
            Log::error() << "Invalid JSON ScrollBarStyle.";
            return Skin();
        }
        skin.scrollStyle.downButton = parseButtonStyle(style.get("down-button", Json::Value()), resources, error);
        skin.scrollStyle.upButton = parseButtonStyle(style.get("up-button", Json::Value()), resources, error);
        skin.scrollStyle.scroll = parseButtonStyle(style.get("scroll", Json::Value()), resources, error);
    }
    if (std::find(members.begin(), members.end(), "ButtonStyle") != members.end())
    {
        Json::Value style = root.get("ButtonStyle", Json::Value());
        Json::Value::Members styleMembers = style.getMemberNames();

        skin.buttonStyle = parseButtonStyle(style, resources, error);
    }
    if (std::find(members.begin(), members.end(), "LineEditStyle") != members.end())
    {
        Json::Value style = root.get("LineEditStyle", Json::Value());
        Json::Value::Members styleMembers = style.getMemberNames();

        skin.lineEditStyle = parseLineEditStyle(style, resources, error);
    }
    return skin;
}

ButtonStyle JSONHelper::parseButtonStyle(const Json::Value& object, const std::map< std::wstring, ResourceDescriptor >& resources, bool& error)
{
    error = false;
    ButtonStyle style;
    Json::Value::Members members = object.getMemberNames();
    if (std::find(members.begin(), members.end(), "normal") == members.end())
    {
        error = true;
        Log::error() << "Invalid JSON ButtonStyle";
        return ButtonStyle();
    }
    std::wstring normal = fromUTF8(object.get("normal", "").asString().c_str());
    if (resources.find(normal) == resources.end())
    {
        error = true;
        Log::error() << "No such JSON resource: \"" << normal << "\"";
        return ButtonStyle();
    }
    style.normal = resources.at(normal);
    if (std::find(members.begin(), members.end(), "hovered") != members.end())
    {
        std::wstring hovered = fromUTF8(object.get("hovered", "").asString().c_str());
        if (resources.find(hovered) == resources.end())
        {
            error = true;
            Log::error() << "No such JSON resource: \"" << hovered << "\"";
            return ButtonStyle();
        }
        style.hovered = resources.at(hovered);
    }
    if (std::find(members.begin(), members.end(), "pressed") != members.end())
    {
        std::wstring pressed = fromUTF8(object.get("pressed", "").asString().c_str());
        if (resources.find(pressed) == resources.end())
        {
            error = true;
            Log::error() << "No such JSON resource: \"" << pressed << "\"";
            return ButtonStyle();
        }
        style.pressed = resources.at(pressed);
    }
    if (std::find(members.begin(), members.end(), "font") != members.end())
    {
        std::wstring font = fromUTF8(object.get("font", "").asString().c_str());
        if (resources.find(font) == resources.end())
        {
            error = true;
            Log::error() << "No such JSON resource: \"" << font << "\"";
            return ButtonStyle();
        }
        const ResourceDescriptor& d = resources.at(font);
        if (d.type != ResourceDescriptor::FONT)
        {
            error = true;
            Log::error() << "Invalid JSON button style.";
            return ButtonStyle();
        }
        style.font = boost::get<FontDescriptor>(d.resource);
    }
    if (std::find(members.begin(), members.end(), "color") != members.end())
    {
        std::istringstream ss(object.get("color", "").asString());
        uint32_t color;
        char c;
        ss >> c >> std::hex >> color;
        if (ss.fail())
        {
            error = true;
            Log::error() << "Invalid JSON color.";
            return ButtonStyle();
        }
        style.color = color;
    }
    if (std::find(members.begin(), members.end(), "content-rect") != members.end())
    {
        Json::Value contentRect = object.get("content-rect", Json::Value());
        style.contentRect.x = contentRect.get("x", 0).asInt();
        style.contentRect.y = contentRect.get("y", 0).asInt();
        style.contentRect.width = contentRect.get("width", 0).asInt();
        style.contentRect.height = contentRect.get("height", 0).asInt();
    }
    return style;
}

LineEditStyle JSONHelper::parseLineEditStyle(const Json::Value& object, const std::map< std::wstring, ResourceDescriptor >& resources, bool& error)
{
    error = false;
    LineEditStyle style;
    Json::Value::Members members = object.getMemberNames();
    if (std::find(members.begin(), members.end(), "background") == members.end())
    {
        error = true;
        Log::error() << "Invalid JSON LineEditStyle";
        return LineEditStyle();
    }
    std::wstring background = fromUTF8(object.get("background", "").asString().c_str());
    if (resources.find(background) == resources.end())
    {
        error = true;
        Log::error() << "No such JSON resource: \"" << background << "\"";
        return LineEditStyle();
    }
    style.background = resources.at(background);
    if (std::find(members.begin(), members.end(), "font") != members.end())
    {
        std::wstring font = fromUTF8(object.get("font", "").asString().c_str());
        if (resources.find(font) == resources.end())
        {
            error = true;
            Log::error() << "No such JSON resource: \"" << font << "\"";
            return LineEditStyle();
        }
        const ResourceDescriptor& d = resources.at(font);
        if (d.type != ResourceDescriptor::FONT)
        {
            error = true;
            Log::error() << "Invalid JSON LineEditStyle.";
            return LineEditStyle();
        }
        style.font = boost::get<FontDescriptor>(d.resource);
    }
    if (std::find(members.begin(), members.end(), "color") != members.end())
    {
        std::istringstream ss(object.get("color", "").asString());
        uint32_t color;
        char c;
        ss >> c >> std::hex >> color;
        if (ss.fail())
        {
            error = true;
            Log::error() << "Invalid JSON color.";
            return  LineEditStyle();
        }
        style.color = color;
    }
    if (std::find(members.begin(), members.end(), "content-rect") != members.end())
    {
        Json::Value contentRect = object.get("content-rect", Json::Value());
        style.contentRect.x = contentRect.get("x", 0).asInt();
        style.contentRect.y = contentRect.get("y", 0).asInt();
        style.contentRect.width = contentRect.get("width", 0).asInt();
        style.contentRect.height = contentRect.get("height", 0).asInt();
    }
    return style;
}

}
