/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#include "Button.h"
#include "Sprite.h"
#include "Log.h"
#include "Action.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "NinePatch.h"
#include "Label.h"
#include "Font.h"

namespace Rangers
{

Button::Button(Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_label(0),
    m_autoResize(false)
{
}

Button::Button(boost::shared_ptr<Texture> texture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_label(0),
    m_autoResize(false)
{
    if (texture)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        m_width = texture->width();
        m_height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_label(0),
    m_autoResize(false)
{
    if (texture)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hoverTexture));
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        m_width = texture->width();
        m_height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_label(0),
    m_autoResize(false)
{
    if (texture)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(texture));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hoverTexture)
    {
        m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hoverTexture));
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressTexture)
    {
        m_style.pressed.resource = ResourceDescriptor::Resource(TextureRegion(pressTexture));
        m_style.pressed.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (texture)
    {
        m_width = texture->width();
        m_height = texture->height();
    }
}

Button::Button(const std::wstring& texture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_label(0),
    m_autoResize(false)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    if (main)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        m_width = main->width();
        m_height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_label(0),
    m_autoResize(false)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance()->loadTexture(hoverTexture);
    if (main)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hover));
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        m_width = main->width();
        m_height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture, Widget *parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_label(0),
    m_autoResize(false)
{
    boost::shared_ptr<Texture> main = ResourceManager::instance()->loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance()->loadTexture(hoverTexture);
    boost::shared_ptr<Texture> pressed = ResourceManager::instance()->loadTexture(pressTexture);
    if (main)
    {
        m_style.normal.resource = ResourceDescriptor::Resource(TextureRegion(main));
        m_style.normal.type = ResourceDescriptor::SPRITE;
    }
    if (hover)
    {
        m_style.hovered.resource = ResourceDescriptor::Resource(TextureRegion(hover));
        m_style.hovered.type = ResourceDescriptor::SPRITE;
    }
    if (pressed)
    {
        m_style.pressed.resource = ResourceDescriptor::Resource(TextureRegion(pressed));
        m_style.pressed.type = ResourceDescriptor::SPRITE;
    }
    init();
    if (main)
    {
        m_width = main->width();
        m_height = main->height();
    }
}

Button::Button(const ButtonStyle& style, Widget* parent):
    Widget(parent), m_hoverSprite(0), m_pressedSprite(0), m_sprite(0), m_normalSprite(0), m_style(style), m_label(0),
    m_autoResize(false)
{
    init();
    if (m_sprite)
    {
        m_width = m_sprite->width();
        m_height = m_sprite->height();
    }
}

bool Button::autoResize() const
{
    return m_autoResize;
}

void Button::setAutoResize(bool autoResize)
{
    m_autoResize = autoResize;
    calcAutoRresize();
}

void Button::setText(const std::wstring& text)
{
    m_text = text;
    m_label->setText(text);
    calcAutoRresize();
    markToUpdate();
}

void Button::setColor(int color)
{
    m_label->setColor(((m_style.color >> 16) & 0xff) / 255.0f, ((m_style.color >> 8) & 0xff) / 255.0f, (m_style.color & 0xff) / 255.0f, ((m_style.color >> 24) & 0xff) / 255.0f);
}

void Button::setFont(boost::shared_ptr<Font> font)
{
    m_label->setFont(font);
    calcAutoRresize();
    markToUpdate();
}

std::wstring Button::text() const
{
    if (!m_label)
        return std::wstring();
    return m_text;
}

void Button::calcAutoRresize()
{
    if (!m_autoResize)
        return;
    if (!m_label)
        return;
    if (!m_normalSprite)
        return;

    float labelWidth = 0.0f;
    float labelHeight = 0.0f;
    if (m_label->font())
    {
        labelWidth = m_label->font()->calculateStringWidth(m_text.begin(), m_text.end());
        labelHeight = m_label->font()->size();
    }
    if ((m_style.contentRect.width < 0) || (m_style.contentRect.height < 0))
    {
        setGeometry(std::max(labelWidth, m_normalSprite->normalWidth()), std::max(labelHeight, m_normalSprite->normalHeight()));
    }
    else
    {
        float width = m_normalSprite->normalWidth() + labelWidth - m_style.contentRect.width;
        float height = m_normalSprite->normalHeight() + labelHeight - m_style.contentRect.height;
        setGeometry(std::max(width, m_normalSprite->normalWidth()), std::max(height, m_normalSprite->normalHeight()));
    }
}

int Button::color() const
{
    //TODO: Color in Object;
    //return m_label
    return 0;
}

boost::shared_ptr<Font> Button::font() const
{
    if (!m_label)
        return boost::shared_ptr<Font>();
    return m_label->font();
}

void Button::init()
{
    if (m_style.normal.type == ResourceDescriptor::NINEPATCH)
    {
        m_normalSprite = new NinePatch(boost::get<NinePatchDescriptor>(m_style.normal.resource), this);
    }
    else if (m_style.normal.type == ResourceDescriptor::SPRITE)
    {
        m_normalSprite = new Sprite(boost::get<TextureRegion>(m_style.normal.resource), this);
    }
    if (m_style.hovered.type == ResourceDescriptor::NINEPATCH)
    {
        m_hoverSprite = new NinePatch(boost::get<NinePatchDescriptor>(m_style.hovered.resource), this);
    }
    else if (m_style.hovered.type == ResourceDescriptor::SPRITE)
    {
        m_hoverSprite = new Sprite(boost::get<TextureRegion>(m_style.hovered.resource), this);
    }
    if (m_style.pressed.type == ResourceDescriptor::NINEPATCH)
    {
        m_pressedSprite = new NinePatch(boost::get<NinePatchDescriptor>(m_style.pressed.resource), this);
    }
    else if (m_style.pressed.type == ResourceDescriptor::SPRITE)
    {
        m_pressedSprite = new Sprite(boost::get<TextureRegion>(m_style.pressed.resource), this);
    }
    if ((m_style.font.path != L"") && (m_style.font.size > 0))
    {
        if (!m_label)
            m_label = new Label(m_text, this, ResourceManager::instance()->loadFont(m_style.font.path, m_style.font.size));
        m_label->setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
        setColor(m_style.color);
    }
    m_sprite = m_normalSprite;
    markToUpdate();
}

Button::~Button()
{
    if (m_normalSprite)
        delete m_normalSprite;
    if (m_hoverSprite);
    delete m_hoverSprite;
    if (m_pressedSprite)
        delete m_pressedSprite;
    if (m_label)
        delete m_label;
}

void Button::draw() const
{
    if (!prepareDraw())
        return;
    if (m_sprite)
        m_sprite->draw();
    if (m_label)
        m_label->draw();
    endDraw();
}

void Button::processMain()
{
    lock();
    if (m_normalSprite)
        m_normalSprite->setGeometry(m_width, m_height);
    if (m_hoverSprite)
        m_hoverSprite->setGeometry(m_width, m_height);
    if (m_pressedSprite)
        m_pressedSprite->setGeometry(m_width, m_height);
    if (m_label)
    {
        if ((m_style.contentRect.width < 0) || (m_style.contentRect.height < 0) || (!m_normalSprite))
        {
            m_label->setPosition(int((m_width - m_label->width()) / 2), int((m_height - m_label->height()) / 2));
        }
        else
        {
            m_label->setPosition(m_style.contentRect.x, m_style.contentRect.y);
        }
    }
    unlock();
    Widget::processMain();
}

void Button::mouseEnter()
{
    lock();
    if (m_hoverSprite)
        m_sprite = m_hoverSprite;
    unlock();
    Widget::mouseEnter();
}

void Button::mouseLeave()
{
    lock();
    if (m_normalSprite)
        m_sprite = m_normalSprite;
    unlock();
    Widget::mouseLeave();
}

void Button::mouseDown(uint8_t key, const Vector &p)
{
    lock();
    Widget::mouseDown(key, p);
    if (m_leftMouseButtonPressed)
    {
        if (m_pressedSprite)
            m_sprite = m_pressedSprite;
    }
    unlock();
}

void Button::mouseUp(uint8_t key, const Vector &p)
{
    lock();
    if (m_leftMouseButtonPressed)
    {
        if (m_hoverSprite)
            m_sprite = m_hoverSprite;
        else if (m_normalSprite)
            m_sprite = m_normalSprite;
    }
    Widget::mouseUp(key, p);
    unlock();
}

void Button::mouseClick(const Vector &p)
{
    action(Action(this, Action::BUTTON_CLICKED, Action::Argument()));
}

Button::Button(const Button& other)
{
    if (m_normalSprite)
        delete m_normalSprite;
    if (m_hoverSprite);
    delete m_hoverSprite;
    if (m_pressedSprite)
        delete m_pressedSprite;
    Button(other.m_style);
}

Button& Button::operator=(const Button& other)
{
    if (this == &other)
        return *this;
    Widget::operator=(other);
    if (m_normalSprite)
        delete m_normalSprite;
    if (m_hoverSprite);
    delete m_hoverSprite;
    if (m_pressedSprite)
        delete m_pressedSprite;
    m_style = other.m_style;
    m_normalSprite = 0;
    m_hoverSprite = 0;
    m_pressedSprite = 0;
    init();
    return *this;
}

}

