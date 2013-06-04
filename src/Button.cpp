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

#include "OpenSR/Button.h"
#include "OpenSR/Sprite.h"
#include "OpenSR/Action.h"
#include "OpenSR/ResourceManager.h"
#include "OpenSR/Texture.h"
#include "OpenSR/NinePatch.h"
#include "OpenSR/Label.h"
#include "OpenSR/Font.h"
#include "OpenSR/Sound.h"
#include "OpenSR/SoundManager.h"

#include "OpenSR/private/Button_p.h"

namespace Rangers
{
void ButtonPrivate::ButtonListener::actionPerformed(const Action &action)
{
    boost::shared_ptr<Button> q = boost::static_pointer_cast<Button>(action.source());
    ButtonPrivate *d = q->d_func();
    switch (action.type())
    {
    case Action::MOUSE_ENTER:
        if (d->hoverSprite)
            d->sprite = d->hoverSprite;
        if (d->enterSound)
            d->enterSound->play();
        break;

    case Action::MOUSE_LEAVE:
        if (d->normalSprite)
            d->sprite = d->normalSprite;
        if (d->leaveSound)
            d->leaveSound->play();
        break;

    case Action::MOUSE_UP:
        if (!d->leftMouseButtonPressed)
        {
            if (d->hoverSprite)
                d->sprite = d->hoverSprite;
            else if (d->normalSprite)
                d->sprite = d->normalSprite;
        }
        break;

    case Action::MOUSE_DOWN:
        if (d->leftMouseButtonPressed)
        {
            if (d->pressedSprite)
                d->sprite = d->pressedSprite;
        }
        break;

    case Action::MOUSE_CLICK:
        if (d->clickSound)
            d->clickSound->play();
        q->action(Action(q, Action::BUTTON_CLICKED));
        break;
    }
}

ButtonPrivate::ButtonPrivate(): WidgetPrivate()
{
    autoResize = false;
    buttonListener = boost::shared_ptr<ButtonListener>(new ButtonListener());
}

Button::Button(Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
}

Button::Button(boost::shared_ptr<Texture> texture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);
    d->initFromStyle();

    if (texture)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(texture));
        addChild(d->normalSprite);
    }

    d->sprite = d->normalSprite;

    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);
    d->initFromStyle();

    if (texture)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(texture));
        addChild(d->normalSprite);
    }
    if (hoverTexture)
    {
        d->hoverSprite = boost::shared_ptr<Sprite>(new Sprite(hoverTexture));
        addChild(d->hoverSprite);
    }

    d->sprite = d->normalSprite;

    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
}

Button::Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);
    d->initFromStyle();

    if (texture)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(texture));
        addChild(d->normalSprite);
    }
    if (hoverTexture)
    {
        d->hoverSprite = boost::shared_ptr<Sprite>(new Sprite(hoverTexture));
        addChild(d->hoverSprite);
    }
    if (pressTexture)
    {
        d->pressedSprite = boost::shared_ptr<Sprite>(new Sprite(pressTexture));
        addChild(d->pressedSprite);
    }

    d->sprite = d->normalSprite;

    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
}

Button::Button(const std::wstring& texture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);
    d->initFromStyle();

    boost::shared_ptr<Texture> main = ResourceManager::instance().loadTexture(texture);

    if (main)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(main));
        addChild(d->normalSprite);
    }

    d->sprite = d->normalSprite;

    if (main)
    {
        d->width = main->width();
        d->height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);
    d->initFromStyle();

    boost::shared_ptr<Texture> main = ResourceManager::instance().loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance().loadTexture(hoverTexture);

    if (main)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(main));
        addChild(d->normalSprite);
    }
    if (hover)
    {
        d->hoverSprite = boost::shared_ptr<Sprite>(new Sprite(hover));
        addChild(d->hoverSprite);
    }

    d->sprite = d->normalSprite;

    if (main)
    {
        d->width = main->width();
        d->height = main->height();
    }
}

Button::Button(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);
    d->initFromStyle();

    boost::shared_ptr<Texture> main = ResourceManager::instance().loadTexture(texture);
    boost::shared_ptr<Texture> hover = ResourceManager::instance().loadTexture(hoverTexture);
    boost::shared_ptr<Texture> pressed = ResourceManager::instance().loadTexture(pressTexture);

    if (main)
    {
        d->normalSprite = boost::shared_ptr<Sprite>(new Sprite(main));
        addChild(d->normalSprite);
    }
    if (hover)
    {
        d->hoverSprite = boost::shared_ptr<Sprite>(new Sprite(hover));
        addChild(d->hoverSprite);
    }

    if (pressed)
    {
        d->pressedSprite = boost::shared_ptr<Sprite>(new Sprite(pressed));
        addChild(d->pressedSprite);
    }

    d->sprite = d->normalSprite;

    if (main)
    {
        d->width = main->width();
        d->height = main->height();
    }
}

Button::Button(const ButtonStyle& style, Widget *parent):
    Widget(*(new ButtonPrivate()), parent)
{
    RANGERS_D(Button);
    d->style = style;
    d->initFromStyle();
    if (d->sprite)
    {
        d->width = d->sprite->width();
        d->height = d->sprite->height();
    }
}


Button::Button(ButtonPrivate &p, Widget *parent): Widget(p, parent)
{
}

bool Button::autoResize() const
{
    RANGERS_D(const Button);
    return d->autoResize;
}

void Button::setAutoResize(bool autoResize)
{
    lock();
    RANGERS_D(Button);
    d->autoResize = autoResize;
    d->calcAutoRresize();
    unlock();
}

void Button::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(Button);
    d->text = text;
    d->label->setText(text);
    d->calcAutoRresize();
    markToUpdate();
    unlock();
}

void Button::setColor(uint32_t color)
{
    lock();
    RANGERS_D(Button);
    d->label->setColor(color);
    unlock();
}

void Button::setFont(boost::shared_ptr<Font> font)
{
    lock();
    RANGERS_D(Button);
    d->label->setFont(font);
    d->calcAutoRresize();
    markToUpdate();
    unlock();
}

uint32_t Button::color() const
{
    RANGERS_D(const Button);
    return d->label->color();
}

std::wstring Button::text() const
{
    RANGERS_D(const Button);
    return d->text;
}

void ButtonPrivate::calcAutoRresize()
{
    RANGERS_Q(Button);
    if (!autoResize)
        return;
    if (!normalSprite)
        return;

    q->lock();
    float labelWidth = 0.0f;
    float labelHeight = 0.0f;
    if (label->font())
    {
        labelWidth = label->font()->calculateStringWidth(text.begin(), text.end());
        labelHeight = label->font()->size();
    }
    if (!style.contentRect.valid())
    {
        q->setGeometry(std::max(labelWidth, normalSprite->normalWidth()), std::max(labelHeight, normalSprite->normalHeight()));
    }
    else
    {
        float width = normalSprite->normalWidth() + labelWidth - style.contentRect.width;
        float height = normalSprite->normalHeight() + labelHeight - style.contentRect.height;
        q->setGeometry(std::max(width, normalSprite->normalWidth()), std::max(height, normalSprite->normalHeight()));
    }
    q->unlock();
}

int Button::minHeight() const
{
    RANGERS_D(const Button);
    if (d->normalSprite)
        return std::max(d->label->height(), d->normalSprite->normalHeight());
    return Widget::minHeight();
}

int Button::minWidth() const
{
    RANGERS_D(const Button);
    if (d->normalSprite)
        return std::max(d->label->width(), d->normalSprite->normalWidth());
    return Widget::minWidth();
}

int Button::preferredHeight() const
{
    RANGERS_D(const Button);
    if (!d->normalSprite)
        return Widget::preferredHeight();

    if (d->style.contentRect.valid())
        return d->normalSprite->normalHeight() + d->label->height() - d->style.contentRect.height;

    return minHeight();
}

int Button::preferredWidth() const
{
    RANGERS_D(const Button);
    if (!d->normalSprite)
        return Widget::preferredWidth();

    if (d->style.contentRect.valid())
        return d->normalSprite->normalWidth() + d->label->width() - d->style.contentRect.width;

    return minWidth();
}

boost::shared_ptr<Font> Button::font() const
{
    RANGERS_D(const Button);
    return d->label->font();
}

void ButtonPrivate::initFromStyle()
{
    RANGERS_Q(Button);
    if (style.normal.type == ResourceDescriptor::NINEPATCH)
    {
        normalSprite = boost::shared_ptr<Sprite>(new NinePatch(boost::get<NinePatchDescriptor>(style.normal.resource)));
        q->addChild(normalSprite);
    }
    else if (style.normal.type == ResourceDescriptor::SPRITE)
    {
        normalSprite = boost::shared_ptr<Sprite>(new Sprite(boost::get<TextureRegionDescriptor>(style.normal.resource)));
        q->addChild(normalSprite);
    }
    if (style.hovered.type == ResourceDescriptor::NINEPATCH)
    {
        hoverSprite = boost::shared_ptr<Sprite>(new NinePatch(boost::get<NinePatchDescriptor>(style.hovered.resource)));
        q->addChild(hoverSprite);
    }
    else if (style.hovered.type == ResourceDescriptor::SPRITE)
    {
        hoverSprite = boost::shared_ptr<Sprite>(new Sprite(boost::get<TextureRegionDescriptor>(style.hovered.resource)));
        q->addChild(hoverSprite);
    }
    if (style.pressed.type == ResourceDescriptor::NINEPATCH)
    {
        pressedSprite = boost::shared_ptr<Sprite>(new NinePatch(boost::get<NinePatchDescriptor>(style.pressed.resource)));
        q->addChild(pressedSprite);
    }
    else if (style.pressed.type == ResourceDescriptor::SPRITE)
    {
        pressedSprite = boost::shared_ptr<Sprite>(new Sprite(boost::get<TextureRegionDescriptor>(style.pressed.resource)));
        q->addChild(pressedSprite);
    }
    if ((style.font.path != L"") && (style.font.size > 0))
    {
        label = boost::shared_ptr<Label>(new Label(text, 0, ResourceManager::instance().loadFont(style.font.path, style.font.size)));
    }
    else
    {
        label = boost::shared_ptr<Label>(new Label());
    }
    label->setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    q->addChild(label);
    q->setColor(style.color);
    sprite = normalSprite;

    if (style.enterSound != L"")
        enterSound = SoundManager::instance().loadSound(style.enterSound);
    if (style.leaveSound != L"")
        leaveSound = SoundManager::instance().loadSound(style.leaveSound);
    if (style.clickSound != L"")
        clickSound = SoundManager::instance().loadSound(style.clickSound);

    q->addListener(buttonListener);
    q->markToUpdate();
}

Button::~Button()
{
    RANGERS_D(const Button);
    /*if (d->normalSprite)
        delete d->normalSprite;
    if (d->hoverSprite)
        delete d->hoverSprite;
    if (d->pressedSprite)
        delete d->pressedSprite;
    delete d->label;*/
}

void Button::draw() const
{
    RANGERS_D(const Button);
    if (!prepareDraw())
        return;
    if (d->sprite)
        d->sprite->draw();
    d->label->draw();
    endDraw();
}

void Button::setSounds(boost::shared_ptr<Sound> clickSound, boost::shared_ptr<Sound> leaveSound, boost::shared_ptr<Sound> enterSound)
{
    lock();
    RANGERS_D(Button);
    d->clickSound = clickSound;
    d->enterSound = enterSound;
    d->leaveSound = leaveSound;
    unlock();
}

void Button::setSounds(const std::wstring& clickSound, const std::wstring& leaveSound, const std::wstring& enterSound)
{
    lock();
    RANGERS_D(Button);
    d->clickSound = SoundManager::instance().loadSound(clickSound);
    d->enterSound = SoundManager::instance().loadSound(enterSound);
    d->leaveSound = SoundManager::instance().loadSound(leaveSound);
    unlock();
}

void Button::processMain()
{
    lock();
    RANGERS_D(Button);
    if (d->label->needUpdate())
        d->label->processMain();
    if (d->normalSprite)
        d->normalSprite->setGeometry(d->width, d->height);
    if (d->hoverSprite)
        d->hoverSprite->setGeometry(d->width, d->height);
    if (d->pressedSprite)
        d->pressedSprite->setGeometry(d->width, d->height);
    if (!d->style.contentRect.valid() || (!d->normalSprite))
    {
        d->label->setPosition(int((d->width - d->label->width()) / 2), int((d->height - d->label->height()) / 2));
    }
    else
    {
        float x, y;
        if (d->label->width() < d->style.contentRect.width)
            x = int(d->style.contentRect.width - d->label->width()) / 2 + d->style.contentRect.x;
        else
            x = d->style.contentRect.x;
        if (d->label->height() < d->style.contentRect.height)
            y = int(d->style.contentRect.height - d->label->height()) / 2 + d->style.contentRect.y;
        else
            y = d->style.contentRect.y;
        d->label->setPosition(x, y);
    }
    unlock();
    Widget::processMain();
}

}

