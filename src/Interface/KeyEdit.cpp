/* KeyEdit.cpp

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

#include "Interface/KeyEdit.hpp"

#include <GL/gl.h>
#include <algorithm>

#include "Interface/Label.hpp"
#include "Interface/toolTip.hpp"
#include "Locales/Locale.hpp"
#include "Locales/locales.hpp"
#include "Media/sound.hpp"
#include "Media/text.hpp"
#include "Menu/menus.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/generateName.hpp"

namespace sf
{
class String;
} // namespace sf

KeyEdit::KeyEdit(sf::String * text, sf::String * toolTip, Key * value,
                 Vector2f const & topLeft, int width, int labelWidth)
    : UiElement(topLeft, width, 20), value_(value), toolTip_(toolTip),
      labelWidth_(labelWidth)
{

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0, 0));
    label_->setParent(this);
}

KeyEdit::~KeyEdit() { delete label_; }

void KeyEdit::mouseMoved(Vector2f const & position)
{
    UiElement::mouseMoved(position);
    label_->mouseMoved(position);

    if (hovered_ && toolTip_)
        toolTip::show(toolTip_);
}

void KeyEdit::mouseLeft(bool down)
{
    if (down && hovered_)
    {
        menus::clearFocus();
        setFocus(this, false);
        pressed_ = true;
        menus::fixKeyboardOn(this);
        sound::playSound(sound::Click);
    }
}

void KeyEdit::keyEvent(bool down, Key const & key)
{
    if (pressed_)
    {
        if (down && (key.navi_ != Key::nAbort) && key.strength_ >= 95)
        {
            *value_ = key;
            pressed_ = false;
            menus::unFixKeyboard();
        }
        else if (down && (key.navi_ == Key::nAbort))
        {
            menus::unFixKeyboard();
            pressed_ = false;
        }
    }
    else if (down && (key.navi_ == Key::nConfirm))
    {
        menus::fixKeyboardOn(this);
        pressed_ = true;
    }
}

void KeyEdit::draw() const
{
    UiElement::draw();
    Vector2f origin = getTopLeft();

    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    // draw Button
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    if (isTopMost())
        glColor4f(0.3 * focusedFadeTime_, 0.1 * focusedFadeTime_,
                  0.2 * focusedFadeTime_, 0.8);
    else
        glColor4f(0.0, 0.0, 0.0, 0.8);
    glVertex2f(origin.x_ + labelWidth_ * mirror, origin.y_ + 2);
    glVertex2f(width() + origin.x_, origin.y_ + 2);
    glVertex2f(width() + origin.x_, height_ + origin.y_ - 2);
    glVertex2f(origin.x_ + labelWidth_ * mirror, height_ + origin.y_ - 2);

    // glossy bottom
    glColor4f(1.0, 1.0, 1.0, 0.0);
    glVertex2f(origin.x_ + labelWidth_ * mirror, origin.y_ + 2);
    glVertex2f(width() + origin.x_, origin.y_ + 2);
    if (pressed_)
        glColor4f(1.0, 1.0, 1.0, 0.1);
    else
        glColor4f(1.0, 1.0, 1.0, 0.06);
    glVertex2f(width() + origin.x_, height_ + origin.y_ - 2);
    glVertex2f(origin.x_ + labelWidth_ * mirror, height_ + origin.y_ - 2);

    if (!pressed_)
    {
        // glossy top
        glColor4f(1.0, 1.0, 1.0, 0.2);
        glVertex2f(origin.x_ + labelWidth_ * mirror, origin.y_ + 2);
        glVertex2f(width() + origin.x_, origin.y_ + 2);
        glColor4f(1.0, 1.0, 1.0, 0.05);
        glVertex2f(width() + origin.x_, height_ * 0.5f + origin.y_);
        glVertex2f(origin.x_ + labelWidth_ * mirror,
                   height_ * 0.5f + origin.y_);
    }
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(1.f);

    glColor4f(1.0, 0.4, 0.8, 0.3f + hoveredFadeTime_ * 0.7f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(origin.x_ + labelWidth_ * mirror, origin.y_ + 2);
    glVertex2f(width() + origin.x_, origin.y_ + 2);
    glVertex2f(width() + origin.x_, height_ + origin.y_ - 2);
    glVertex2f(origin.x_ + labelWidth_ * mirror, height_ + origin.y_ - 2);
    glEnd();

    float highlight(std::max(hoveredFadeTime_, focusedFadeTime_));
    Color3f color(Color3f(0.7f, 0.7f, 0.7f) * (1 - highlight) +
                  highlight *
                      (Color3f(1.f, 0.6f, 0.8f) * (1 - hoveredFadeTime_) +
                       Color3f(1, 1, 1) * hoveredFadeTime_));

    if (pressed_)
        text::drawScreenText(
            "...",
            origin + Vector2f((width() + labelWidth_ * mirror) / 2, 1) +
                Vector2f(1, 1),
            12.f, TEXT_ALIGN_CENTER, color);
    else
        text::drawScreenText(
            generateName::key(*value_),
            origin + Vector2f((width() + labelWidth_ * mirror) / 2, 1), 12.f,
            TEXT_ALIGN_CENTER, color);

    // draw Label
    label_->draw();
}

void KeyEdit::setFocus(UiElement * toBeFocused, bool isPrevious)
{
    UiElement::setFocus(this, isPrevious);
    label_->setFocus(this, isPrevious);
}

void KeyEdit::clearFocus()
{
    UiElement::clearFocus();
    pressed_ = false;
    menus::unFixKeyboard();
    label_->clearFocus();
}
