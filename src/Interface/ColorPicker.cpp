/* ColorPicker.cpp

Copyright (c) 2010 - 2011 by Felix Lauer and Simon Schneegans

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "Interface/ColorPicker.hpp"

#include <GL/gl.h>

#include "Interface/ColorPickerWindow.hpp"
#include "Interface/Label.hpp"
#include "Interface/UiWindow.hpp"
#include "Locales/Locale.hpp"
#include "Locales/locales.hpp"
#include "Media/sound.hpp"
#include "Media/text.hpp"
#include "Menu/menus.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"

namespace sf
{
class String;
} // namespace sf

ColorPicker::ColorPicker(const sf::String * text, Color3f * value,
                         Vector2f const & topLeft, int width, int labelWidth)
    : UiElement(topLeft, width, 16), colorWindow_(nullptr),
      currentValue_(value), labelWidth_(labelWidth), opened_(false)
{

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0, 0));
    label_->setParent(this);

    colorWindow_ = new ColorPickerWindow(this, currentValue_);
}

ColorPicker::~ColorPicker()
{
    delete label_;
    delete colorWindow_;
}

void ColorPicker::mouseMoved(Vector2f const & position)
{
    UiElement::mouseMoved(position);
    label_->mouseMoved(position);
}

void ColorPicker::mouseLeft(bool down)
{
    UiElement::mouseLeft(hovered_ && down);
    if (!pressed_ && hovered_ && focused_)
    {
        hovered_ = false;
        sound::playSound(sound::Click);
        menus::showWindow(colorWindow_);
    }
}

void ColorPicker::keyEvent(bool down, Key const & key)
{
    if (key.navi_ == Key::nConfirm)
    {
        pressed_ = down;
        if (!pressed_)
        {
            hovered_ = false;
            sound::playSound(sound::Click);
            menus::showWindow(colorWindow_);
        }
    }
}

void ColorPicker::draw() const
{
    UiElement::draw();

    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    Vector2f origin = getTopLeft();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    // dark background
    currentValue_->gl3f();
    glVertex2f(labelWidth_ * mirror + origin.x_, origin.y_);
    glVertex2f(width() + origin.x_, origin.y_);
    glVertex2f(width() + origin.x_, height_ + origin.y_);
    glVertex2f(labelWidth_ * mirror + origin.x_, height_ + origin.y_);

    // glossy bottom
    glColor4f(1.0, 1.0, 1.0, 0.0);
    glVertex2f(labelWidth_ * mirror + origin.x_, height_ * 0.7f + origin.y_);
    glVertex2f(width() + origin.x_, height_ * 0.7f + origin.y_);
    glColor4f(1.0, 1.0, 1.0, 0.06);
    glVertex2f(width() + origin.x_, height_ + origin.y_);
    glVertex2f(labelWidth_ * mirror + origin.x_, height_ + origin.y_);

    if (pressed_ && hovered_)
    {
        // bottom glow
        glColor4f(0.5, 0.25, 0.4, 0.0);
        glVertex2f(labelWidth_ * mirror + origin.x_, origin.y_);
        glVertex2f(origin.x_ + width(), origin.y_);
        glColor4f(0.5, 0.25, 0.4, 0.4);
        glVertex2f(origin.x_ + width(), origin.y_ + height_);
        glVertex2f(labelWidth_ * mirror + origin.x_, origin.y_ + height_);
    }
    else if (hovered_)
    {
        glColor4f(0.5, 0.25, 0.4, 0.0);
        glVertex2f(labelWidth_ * mirror + origin.x_,
                   height_ * 0.5f + origin.y_);
        glVertex2f(origin.x_ + width(), height_ * 0.5f + origin.y_);
        glColor4f(0.5, 0.25, 0.4, 0.4);
        glVertex2f(origin.x_ + width(), origin.y_ + height_);
        glVertex2f(labelWidth_ * mirror + origin.x_, origin.y_ + height_);

        // glossy top
        glColor4f(1.0, 1.0, 1.0, 0.2);
        glVertex2f(labelWidth_ * mirror + origin.x_, origin.y_);
        glVertex2f(width() + origin.x_, origin.y_);
        glColor4f(1.0, 1.0, 1.0, 0.05);
        glVertex2f(width() + origin.x_, height_ * 0.5f + origin.y_);
        glVertex2f(labelWidth_ * mirror + origin.x_,
                   height_ * 0.5f + origin.y_);
    }
    else
    {
        // glossy top
        glColor4f(1.0, 1.0, 1.0, 0.2);
        glVertex2f(labelWidth_ * mirror + origin.x_, origin.y_);
        glVertex2f(width() + origin.x_, origin.y_);
        glColor4f(1.0, 1.0, 1.0, 0.05);
        glVertex2f(width() + origin.x_, height_ * 0.5f + origin.y_);
        glVertex2f(labelWidth_ * mirror + origin.x_,
                   height_ * 0.5f + origin.y_);
    }
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(1.f);

    glColor4f(1.0, 0.4, 0.8, 0.3f + hoveredFadeTime_ * 0.7f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(labelWidth_ * mirror + origin.x_, origin.y_ + height_);
    glVertex2f(labelWidth_ * mirror + origin.x_, origin.y_);
    glVertex2f(origin.x_ + width(), origin.y_);
    glVertex2f(origin.x_ + width(), origin.y_ + height_);
    glEnd();

    text::drawFooText();
    // draw Label
    label_->draw();
}

void ColorPicker::setFocus(UiElement * toBeFocused, bool isPrevious)
{
    UiElement::setFocus(this, isPrevious);
    label_->setFocus(this, isPrevious);
}

void ColorPicker::clearFocus()
{
    UiElement::clearFocus();
    label_->clearFocus();
}
