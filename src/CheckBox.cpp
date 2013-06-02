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

#include "CheckBox.h"
#include "Sprite.h"
#include "Action.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "NinePatch.h"

#include "private/CheckBox_p.h"

namespace Rangers
{
CheckBoxPrivate::CheckBoxPrivate()
{
    checked = false;
    checkBoxListener = boost::shared_ptr<CheckBoxListener>(new CheckBoxListener());
}

void CheckBoxPrivate::CheckBoxListener::actionPerformed(const Action& action)
{
    boost::shared_ptr<CheckBox> q = boost::static_pointer_cast<CheckBox>(action.source());
    CheckBoxPrivate *d = q->d_func();
    switch (action.type())
    {
    case Action::MOUSE_ENTER:
        if (d->hovered && d->checkedHovered)
        {
            if (d->checked)
                d->sprite = d->checkedHovered;
            else
                d->sprite = d->hovered;
        }
        break;

    case Action::MOUSE_LEAVE:
        if (d->checked)
            d->sprite = d->checkedNormal;
        else
            d->sprite = d->normal;
        break;

    case Action::MOUSE_CLICK:
        q->setChecked(!d->checked);
        q->action(Action(q, Action::CHECKBOX_TOGGLED, Action::Argument(d->checked)));
        break;
    }
}

CheckBox::CheckBox(const CheckBoxStyle& style, const std::wstring &text, Widget *parent): Widget(*(new CheckBoxPrivate()), parent)
{
    RANGERS_D(CheckBox);
    d->style = style;
    if (d->style.normal.type == ResourceDescriptor::NINEPATCH)
    {
        d->normal = boost::shared_ptr<Sprite>(new NinePatch(boost::get<NinePatchDescriptor>(d->style.normal.resource)));
        addChild(d->normal);
    }
    else if (d->style.normal.type == ResourceDescriptor::SPRITE)
    {
        d->normal = boost::shared_ptr<Sprite>(new Sprite(boost::get<TextureRegionDescriptor>(d->style.normal.resource)));
        addChild(d->normal);
    }

    if (d->style.checkedNormal.type == ResourceDescriptor::NINEPATCH)
    {
        d->checkedNormal = boost::shared_ptr<Sprite>(new NinePatch(boost::get<NinePatchDescriptor>(d->style.checkedNormal.resource)));
        addChild(d->checkedNormal);
    }
    else if (d->style.checkedNormal.type == ResourceDescriptor::SPRITE)
    {
        d->checkedNormal = boost::shared_ptr<Sprite>(new Sprite(boost::get<TextureRegionDescriptor>(d->style.checkedNormal.resource)));
        addChild(d->checkedNormal);
    }

    if (d->style.hovered.type == ResourceDescriptor::NINEPATCH)
    {
        d->hovered = boost::shared_ptr<Sprite>(new NinePatch(boost::get<NinePatchDescriptor>(d->style.hovered.resource)));
        addChild(d->hovered);
    }
    else if (d->style.hovered.type == ResourceDescriptor::SPRITE)
    {
        d->hovered = boost::shared_ptr<Sprite>(new Sprite(boost::get<TextureRegionDescriptor>(d->style.hovered.resource)));
        addChild(d->hovered);
    }

    if (d->style.checkedHovered.type == ResourceDescriptor::NINEPATCH)
    {
        d->checkedHovered = boost::shared_ptr<Sprite>(new NinePatch(boost::get<NinePatchDescriptor>(d->style.checkedHovered.resource)));
        addChild(d->checkedHovered);
    }
    else if (d->style.checkedHovered.type == ResourceDescriptor::SPRITE)
    {
        d->checkedHovered = boost::shared_ptr<Sprite>(new Sprite(boost::get<TextureRegionDescriptor>(d->style.checkedHovered.resource)));
        addChild(d->checkedHovered);
    }

    if ((d->style.font.path != L"") && (d->style.font.size > 0))
    {
        d->label = boost::shared_ptr<Label>(new Label(text, 0, ResourceManager::instance().loadFont(d->style.font.path, d->style.font.size)));
    }
    else
    {
        d->label = boost::shared_ptr<Label>(new Label());
    }
    d->label->setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    addChild(d->label);
    setColor(d->style.color);
    d->sprite = d->normal;

    addListener(d->checkBoxListener);
    markToUpdate();
}

CheckBox::~CheckBox()
{
    RANGERS_D(CheckBox);
    /*if (d->normal)
        delete d->normal;
    if (d->checkedNormal)
        delete d->checkedNormal;
    if (d->hovered)
        delete d->hovered;
    if (d->checkedHovered)
        delete d->checkedHovered;
    delete d->label;*/
}

void CheckBox::setColor(uint32_t color)
{
    lock();
    RANGERS_D(CheckBox);
    d->label->setColor(color);
    unlock();
}

uint32_t CheckBox::color() const
{
    RANGERS_D(const CheckBox);
    return d->label->color();
}

void CheckBox::setChecked(bool checked)
{
    lock();
    RANGERS_D(CheckBox);
    d->checked = checked;
    if ((d->sprite == d->hovered) || (d->sprite == d->checkedHovered))
    {
        if (d->checked)
            d->sprite = d->checkedHovered;
        else
            d->sprite = d->hovered;
    }
    else
    {
        if (d->checked)
            d->sprite = d->checkedNormal;
        else
            d->sprite = d->normal;
    }
    unlock();
    markToUpdate();
}

void CheckBox::setText(const std::wstring& text)
{
    lock();
    RANGERS_D(CheckBox);
    d->label->setText(text);
    markToUpdate();
    unlock();
}

bool CheckBox::checked() const
{
    RANGERS_D(const CheckBox);
    return d->checked;
}

std::wstring CheckBox::text() const
{
    RANGERS_D(const CheckBox);
    return d->label->text();
}

void CheckBox::draw() const
{
    RANGERS_D(const CheckBox);
    if (!prepareDraw())
        return;
    if (d->sprite)
        d->sprite->draw();
    d->label->draw();
    endDraw();
}

void CheckBox::processMain()
{
    lock();
    RANGERS_D(CheckBox);
    if (!d->sprite)
        return;

    if (d->sprite->needUpdate())
        d->sprite->processMain();
    if (d->label->needUpdate())
        d->label->processMain();

    d->width = d->sprite->width() + d->label->width() + 5;
    d->height = d->sprite->height();
    d->label->setPosition(d->sprite->width() + 5, int(d->sprite->height() - d->label->height()) / 2);
    unlock();
    Widget::processMain();
}
}

