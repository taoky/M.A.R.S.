/* LanguageButton.cpp

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

#include "Interface/LanguageButton.hpp"

#include <GL/gl.h>
#include <algorithm>

#include "Interface/Label.hpp"
#include "Locales/Locale.hpp"
#include "Locales/locales.hpp"
#include "Media/sound.hpp"
#include "Media/text.hpp"
#include "Media/texture.hpp"
#include "Menu/ChooseLanguage.hpp"
#include "Menu/menus.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"

namespace sf
{
class String;
} // namespace sf

LanguageButton::LanguageButton(sf::String * text, Vector2f const & topLeft,
                               int width, int labelWidth)
    : UiElement(topLeft, width, 16), labelWidth_(labelWidth)
{

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0, 0));
    label_->setParent(this);
}

LanguageButton::~LanguageButton() { delete label_; }

void LanguageButton::mouseMoved(Vector2f const & position)
{
    UiElement::mouseMoved(position);
    label_->mouseMoved(position);
}

void LanguageButton::mouseWheelMoved(Vector2f const & position, int delta)
{
    if (hovered_)
    {
        while (delta > 0)
        {
            ChooseLanguage::previous();
            --delta;
        }
        while (delta < 0)
        {
            ChooseLanguage::next();
            ++delta;
        }
    }
}

void LanguageButton::mouseLeft(bool down)
{
    UiElement::mouseLeft(hovered_ && down);
    if (!pressed_ && hovered_ && focused_)
    {
        hovered_ = false;
        sound::playSound(sound::Click);
        menus::showWindow(ChooseLanguage::get());
    }
}

void LanguageButton::keyEvent(bool down, Key const & key)
{
    if (key.navi_ == Key::nConfirm)
    {
        pressed_ = down;
        if (!pressed_)
        {
            hovered_ = false;
            sound::playSound(sound::Click);
            menus::showWindow(ChooseLanguage::get());
        }
    }
}

void LanguageButton::draw() const
{
    UiElement::draw();

    Vector2f origin = getTopLeft();
    int boxBegin(labelWidth_ + origin.x_);
    if (!locales::getCurrentLocale().LTR_)
        boxBegin -= 2 * labelWidth_;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    // dark background
    if (isTopMost())
        glColor4f(0.3 * focusedFadeTime_, 0.1 * focusedFadeTime_,
                  0.2 * focusedFadeTime_, 0.8);
    else
        glColor4f(0.0, 0.0, 0.0, 0.8);
    glVertex2f(boxBegin, origin.y_);
    glVertex2f(boxBegin, height_ + origin.y_);
    glColor4f(0.0, 0.0, 0.0, 0.8);
    glVertex2f(boxBegin * 0.5f + width() * 0.5f + origin.x_ * 0.5f,
               height_ + origin.y_);
    glVertex2f(boxBegin * 0.5f + width() * 0.5f + origin.x_ * 0.5f, origin.y_);
    glVertex2f(boxBegin * 0.5f + width() * 0.5f + origin.x_ * 0.5f,
               height_ + origin.y_);
    glVertex2f(boxBegin * 0.5f + width() * 0.5f + origin.x_ * 0.5f, origin.y_);
    if (isTopMost())
        glColor4f(0.3 * focusedFadeTime_, 0.1 * focusedFadeTime_,
                  0.2 * focusedFadeTime_, 0.8);
    else
        glColor4f(0.0, 0.0, 0.0, 0.8);
    glVertex2f(width() + origin.x_, origin.y_);
    glVertex2f(width() + origin.x_, height_ + origin.y_);

    // glossy bottom
    glColor4f(1.0, 1.0, 1.0, 0.0);
    glVertex2f(boxBegin, height_ * 0.7f + origin.y_);
    glVertex2f(width() + origin.x_, height_ * 0.7f + origin.y_);
    glColor4f(1.0, 1.0, 1.0, 0.06);
    glVertex2f(width() + origin.x_, height_ + origin.y_);
    glVertex2f(boxBegin, height_ + origin.y_);

    if (pressed_ && hovered_)
    {
        // bottom glow
        glColor4f(0.5, 0.25, 0.4, 0.0);
        glVertex2f(boxBegin, origin.y_);
        glVertex2f(origin.x_ + width(), origin.y_);
        glColor4f(0.5, 0.25, 0.4, 0.4);
        glVertex2f(origin.x_ + width(), origin.y_ + height_);
        glVertex2f(boxBegin, origin.y_ + height_);
    }
    else if (hovered_)
    {
        glColor4f(0.5, 0.25, 0.4, 0.0);
        glVertex2f(boxBegin, height_ * 0.5f + origin.y_);
        glVertex2f(origin.x_ + width(), height_ * 0.5f + origin.y_);
        glColor4f(0.5, 0.25, 0.4, 0.4);
        glVertex2f(origin.x_ + width(), origin.y_ + height_);
        glVertex2f(boxBegin, origin.y_ + height_);

        // glossy top
        glColor4f(1.0, 1.0, 1.0, 0.2);
        glVertex2f(boxBegin, origin.y_);
        glVertex2f(width() + origin.x_, origin.y_);
        glColor4f(1.0, 1.0, 1.0, 0.05);
        glVertex2f(width() + origin.x_, height_ * 0.5f + origin.y_);
        glVertex2f(boxBegin, height_ * 0.5f + origin.y_);
    }
    else
    {
        // glossy top
        glColor4f(1.0, 1.0, 1.0, 0.2);
        glVertex2f(boxBegin, origin.y_);
        glVertex2f(width() + origin.x_, origin.y_);
        glColor4f(1.0, 1.0, 1.0, 0.05);
        glVertex2f(width() + origin.x_, height_ * 0.5f + origin.y_);
        glVertex2f(boxBegin, height_ * 0.5f + origin.y_);
    }
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(1.f);

    glColor4f(1.0, 0.4, 0.8, 0.3f + hoveredFadeTime_ * 0.7f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(boxBegin, origin.y_ + height_);
    glVertex2f(boxBegin, origin.y_);
    glVertex2f(origin.x_ + width(), origin.y_);
    glVertex2f(origin.x_ + width(), origin.y_ + height_);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Widgets));

    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    int x(3), y(3);
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
    glTexCoord2f(x * 0.25f, y * 0.25f + 0.25f);
    glVertex2f(origin.x_ - 18.f * mirror + width(), origin.y_ + 16.f);
    glTexCoord2f(x * 0.25f + 0.25f, y * 0.25f + 0.25f);
    glVertex2f(origin.x_ - 2.f * mirror + width(), origin.y_ + 16.f);
    glTexCoord2f(x * 0.25f + 0.25f, y * 0.25f);
    glVertex2f(origin.x_ - 2.f * mirror + width(), origin.y_ - 0.f);
    glTexCoord2f(x * 0.25f, y * 0.25f);
    glVertex2f(origin.x_ - 18.f * mirror + width(), origin.y_ - 0.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    float highlight(std::max(hoveredFadeTime_, focusedFadeTime_));
    Color3f color(Color3f(0.7f, 0.7f, 0.7f) * (1 - highlight) +
                  highlight *
                      (Color3f(1.f, 0.6f, 0.8f) * (1 - hoveredFadeTime_) +
                       Color3f(1, 1, 1) * hoveredFadeTime_));

    text::drawFooText();
    text::drawScreenText(
        locales::getCurrentLocale().name_,
        origin + Vector2f((width() + labelWidth_ * mirror) * 0.5f, 1), 12.f,
        TEXT_ALIGN_CENTER, color);

    // draw Label
    label_->draw();
}

void LanguageButton::setFocus(UiElement * toBeFocused, bool isPrevious)
{
    UiElement::setFocus(this, isPrevious);
    label_->setFocus(this, isPrevious);
}

void LanguageButton::clearFocus()
{
    UiElement::clearFocus();
    label_->clearFocus();
}
