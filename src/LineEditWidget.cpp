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

#include "LineEditWidget.h"
#include <SDL.h>
#include "Engine.h"
#include "Font.h"
#include "ActionListener.h"
#include "Action.h"
#include <boost/variant/get.hpp>
#include "ResourceManager.h"
#include "NinePatch.h"

#include "private/LineEditWidget_p.h"

namespace Rangers
{
class LineEditWidget::LineEditWidgetListener: public ActionListener
{
public:
    void actionPerformed(const Action &action)
    {
        if (LineEditWidget *w = dynamic_cast<LineEditWidget *>(action.source()))
        {
            if (action.type() == Action::KEY_PRESSED)
                w->d_func()->keyPressed(boost::get<SDL_keysym>(action.argument()));
        }
    }
};

LineEditWidgetPrivate::LineEditWidgetPrivate()
{
    background = 0;
    cursorVertices = 0;
    cursorBuffer = 0;
    position = 0;
    cursorVisible = false;
    cursorTime = 0;
    stringOffset = 0;
}

void LineEditWidget::draw() const
{
    RANGERS_D(const LineEditWidget);
    if (!prepareDraw())
        return;

    if (d->background)
        d->background->draw();

    d->label.draw();

    if (isFocused() && d->cursorVisible && d->cursorBuffer)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glColor3ub((d->style.color >> 24) & 0xff, (d->style.color >> 16) & 0xff, (d->style.color >> 8) & 0xff);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_ARRAY_BUFFER);

        glBindBuffer(GL_ARRAY_BUFFER, d->cursorBuffer);

        glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
        glLineWidth(1);

        glDrawArrays(GL_LINES, 0, 2);

        glDisableClientState(GL_ARRAY_BUFFER);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    endDraw();
}

LineEditWidget::LineEditWidget(float w, float h, boost::shared_ptr< Font > font, Widget* parent):
    Widget(*(new LineEditWidgetPrivate()), parent)
{
    RANGERS_D(LineEditWidget);
    if (!font)
        font = Engine::instance()->coreFont();
    d->label = Label(L"", this, font, POSITION_X_LEFT, POSITION_Y_TOP);
    d->height = h > font->size() + 4 ? h : font->size() + 4;
    d->width = w;
    d->style.color = 0xffffffff;
    d->init();
}

LineEditWidget::LineEditWidget(Widget* parent): Widget(*(new LineEditWidgetPrivate()), parent)
{
}

LineEditWidget::LineEditWidget(const LineEditStyle& style, Widget* parent): Widget(*(new LineEditWidgetPrivate()), parent)
{
    RANGERS_D(LineEditWidget);
    d->style = style;
    d->init();
}

LineEditWidget::LineEditWidget(LineEditWidgetPrivate &p, Widget *parent): Widget(p, parent)
{
}

LineEditWidget::LineEditWidget(LineEditWidgetPrivate &p, const LineEditWidget& other):  Widget(p, other)
{
    RANGERS_D(LineEditWidget);
    d->style = other.d_func()->style;
    d->label = other.d_func()->label;
    d->text = other.d_func()->text;
    d->init();
}

void LineEditWidgetPrivate::init()
{
    RANGERS_Q(LineEditWidget);
    if (style.background.type == ResourceDescriptor::NINEPATCH)
    {
        background = new NinePatch(boost::get<NinePatchDescriptor>(style.background.resource), q);
    }
    else if (style.background.type == ResourceDescriptor::SPRITE)
    {
        background = new Sprite(boost::get<TextureRegion>(style.background.resource), q);
    }
    if ((style.font.path != L"") && (style.font.size > 0))
    {
        label = Label(text, q, ResourceManager::instance()->loadFont(style.font.path, style.font.size));
        label.setColor(((style.color >> 24) & 0xff) / 255.0f, ((style.color >> 16) & 0xff) / 255.0f, ((style.color >> 8) & 0xff) / 255.0f, ((style.color) & 0xff) / 255.0f);
    }
    if (style.contentRect.valid() && label.font() && background)
    {
        height = std::max(background->normalHeight() - style.contentRect.height + label.font()->size(), background->normalHeight());
        width = std::max(background->normalWidth(), style.contentRect.width);
    }
    else if (label.font() && background)
    {
        height = std::max(background->normalHeight(), (float)label.font()->size());
        width = background->normalWidth();
    }
    else if (style.contentRect.valid() && label.font())
    {
        height = std::max(style.contentRect.height, (float)label.font()->size());
        width = style.contentRect.width;
    }
    label.setOrigin(POSITION_X_LEFT, POSITION_Y_TOP);
    q->addListener(new LineEditWidget::LineEditWidgetListener());
    position = 0;
    cursorTime = 0;
    cursorVisible = false;
    cursorBuffer = 0;
    cursorVertices = 0;
    stringOffset = 0;
    q->markToUpdate();
}

void LineEditWidgetPrivate::updateText()
{
    RANGERS_Q(LineEditWidget);
    if (!label.font())
        return;
    q->lock();
    int maxChars;
    if (position <= stringOffset)
    {
        stringOffset = position > 0 ? position - 1 : 0;
    }
    else
    {
        std::wstring::iterator start = text.begin() + stringOffset;
        std::wstring::iterator end = text.begin() + position;
        while ((maxChars = label.font()->maxChars(start, end, width)) < end - start)
        {
            stringOffset = (end - maxChars) - text.begin() - 1;
            start = text.begin() + stringOffset;
        }
    }
    maxChars = label.font()->maxChars(text.begin() + stringOffset, text.end(), width);
    label.setText(text.substr(stringOffset, maxChars));
    q->unlock();
}

LineEditWidget::LineEditWidget(const Rangers::LineEditWidget& other): Widget(*(new LineEditWidgetPrivate()), other)
{
    RANGERS_D(LineEditWidget);
    d->style = other.d_func()->style;
    d->label = other.d_func()->label;
    d->text = other.d_func()->text;
    d->init();
}

LineEditWidget::~LineEditWidget()
{
    RANGERS_D(LineEditWidget);
    if (d->background)
        delete d->background;
}

LineEditWidget& LineEditWidget::operator=(const Rangers::LineEditWidget& other)
{
    if (this == &other)
        return *this;

    RANGERS_D(LineEditWidget);
    d->style = other.d_func()->style;
    d->label = other.d_func()->label;
    d->text = other.d_func()->text;
    d->init();

    Widget::operator=(other);
    return *this;
}

void LineEditWidget::mouseClick(const Vector& p)
{
    lock();
    RANGERS_D(LineEditWidget);
    Engine::instance()->focusWidget(this);
    unlock();
    Widget::mouseClick(p);
}

void LineEditWidget::processMain()
{
    RANGERS_D(LineEditWidget);

    if (d->label.needUpdate())
        d->label.processMain();

    lock();

    if (d->background)
    {
        d->background->setGeometry(d->width, d->height);
    }

    int cursorPosition = d->label.font()->calculateStringWidth(d->text.begin() + d->stringOffset, d->text.begin() + d->position);
    if (!d->cursorBuffer)
    {
        d->cursorVertices = new Vertex[2];
        glGenBuffers(1, &d->cursorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, d->cursorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2, d->cursorVertices, GL_DYNAMIC_DRAW);
        delete[] d->cursorVertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->cursorBuffer);
    d->cursorVertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!d->style.contentRect.valid())
    {
        d->label.setPosition(0, 0);
        d->cursorVertices[0].x = cursorPosition - 0.5f;
        d->cursorVertices[0].y = d->label.height();
        d->cursorVertices[1].x = cursorPosition - 0.5f;
        d->cursorVertices[1].y = 0;
    }
    else
    {
        d->label.setPosition(d->style.contentRect.x, d->style.contentRect.y);
        d->cursorVertices[0].x = d->style.contentRect.x + cursorPosition - 0.5f;
        d->cursorVertices[0].y = d->label.height();
        d->cursorVertices[1].x = d->style.contentRect.x + cursorPosition - 0.5f;
        d->cursorVertices[1].y = d->style.contentRect.y;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    unlock();
    Widget::processMain();
}

void LineEditWidget::processLogic(int dt)
{
    lock();
    RANGERS_D(LineEditWidget);
    d->cursorTime += dt;
    if (d->cursorTime > 500)
    {
        d->cursorVisible = !d->cursorVisible;
        d->cursorTime = 0;
    }
    unlock();
}


void LineEditWidgetPrivate::keyPressed(const SDL_keysym& key)
{
    RANGERS_Q(LineEditWidget);
    q->lock();

    if (key.sym == SDLK_RETURN);
    else if (key.sym == SDLK_LEFT)
        position--;
    else if (key.sym == SDLK_RIGHT)
        position++;
    else if (key.sym == SDLK_ESCAPE)
        text.clear();
    else if (key.sym == SDLK_BACKSPACE)
    {
        if (text.size())
            if (position > 0)
                text.erase(position - 1, 1);
        position--;
    }
    else if (key.sym == SDLK_DELETE)
    {
        if (text.size())
            if (position < text.length())
                text.erase(position, 1);
    }
    else if (key.unicode && !(key.mod & (KMOD_ALT | KMOD_META | KMOD_CTRL)))
    {
        if (key.unicode == '\t')
            text.insert(position, 1, ' ');
        else
            text.insert(position, 1, key.unicode);
        position++;
    }
    if (position < 0)
        position = 0;
    if (position > text.length())
        position = text.length();
    //label.setText(text);
    updateText();
    q->markToUpdate();

    q->unlock();
}

void LineEditWidget::setText(const std::wstring& s)
{
    lock();
    RANGERS_D(LineEditWidget);
    d->text = s;
    d->position = 0;
    d->stringOffset = 0;
    d->updateText();
    markToUpdate();
    unlock();
}

std::wstring LineEditWidget::text() const
{
    RANGERS_D(const LineEditWidget);
    return d->text;
}

int LineEditWidget::minHeight() const
{
    RANGERS_D(const LineEditWidget);
    if (d->background && d->label.font())
        return std::max(d->label.font()->size(), (int)d->background->normalHeight());

    if (d->label.font())
        return d->label.font()->size();

    return Widget::minHeight();
}

int LineEditWidget::minWidth() const
{
    RANGERS_D(const LineEditWidget);
    std::wstring w = L"W";
    if (d->background && d->label.font())
        return std::max(d->label.font()->calculateStringWidth(w.begin(), w.end()), (int)d->background->normalHeight());

    if (d->label.font())
        return d->label.font()->calculateStringWidth(w.begin(), w.end());

    return Widget::minHeight();
}

int LineEditWidget::preferredHeight() const
{
    RANGERS_D(const LineEditWidget);
    if (d->background && d->label.font() && d->style.contentRect.valid())
        return d->background->normalHeight() + d->label.font()->size() - d->style.contentRect.height;

    return minHeight();
}

int LineEditWidget::maxHeight() const
{
    return preferredHeight();
}
}
