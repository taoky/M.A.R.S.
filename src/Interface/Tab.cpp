/* Tab.cpp

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

#include "Interface/Tab.hpp"

#include <GL/gl.h>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>

#include "Interface/Label.hpp"
#include "Interface/TabList.hpp"
#include "Locales/Locale.hpp"
#include "Locales/locales.hpp"
#include "Menu/menus.hpp"

namespace sf
{
class String;
} // namespace sf

Tab::Tab(sf::String * name, int width, bool * activated)
    : UiElement(Vector2f(), width, 20), focusedWidget_(nullptr), name_(name),
      activated_(activated), active_(false)
{
}

Tab::~Tab()
{
    for (auto & widget : widgets_)
        delete widget;
}

void Tab::mouseMoved(Vector2f const & position)
{
    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);
    Vector2f topLeftAbs(getTopLeft() + topLeft_ * mirror - Vector2f(0.f, 10.f));
    if (locales::getCurrentLocale().LTR_)
    {
        if ((!sf::Mouse::isButtonPressed(sf::Mouse::Left) || pressed_) &&
            topLeftAbs.x_ + width_ > position.x_ &&
            topLeftAbs.y_ + height_ > position.y_ &&
            topLeftAbs.x_ < position.x_ && topLeftAbs.y_ < position.y_)
            hovered_ = true;
        else
            hovered_ = false;
    }
    else
    {
        if ((!sf::Mouse::isButtonPressed(sf::Mouse::Left) || pressed_) &&
            topLeftAbs.x_ - width_ < position.x_ &&
            topLeftAbs.y_ + height_ > position.y_ &&
            topLeftAbs.x_ > position.x_ && topLeftAbs.y_ < position.y_)
            hovered_ = true;
        else
            hovered_ = false;
    }

    if (active_)
        for (auto & widget : widgets_)
            widget->mouseMoved(position);
}

void Tab::mouseWheelMoved(Vector2f const & position, int delta)
{
    if (active_)
        for (auto & widget : widgets_)
            widget->mouseWheelMoved(position, delta);
}

void Tab::mouseLeft(bool down)
{
    pressed_ = hovered_ && down;

    if (!pressed_ && hovered_)
    {
        auto * parent(dynamic_cast<TabList *>(parent_));
        if (parent)
            parent->activate(this);
    }
    if (active_)
        for (auto & widget : widgets_)
            widget->mouseLeft(down);
}

void Tab::keyEvent(bool down, Key const & key)
{
    if (focusedWidget_)
        focusedWidget_->keyEvent(down, key);
}

auto Tab::tabNext() -> bool
{
    if (!focusedWidget_ && widgets_.size() > 0)
    {
        int i(0);
        while (i < widgets_.size() && !widgets_[i]->isTabable())
            ++i;
        if (i < widgets_.size())
        {
            focusedWidget_ = widgets_[i];
            focusedWidget_->setFocus(focusedWidget_, false);
            return false;
        }
        else
            return true;
    }
    else if (focusedWidget_->tabNext())
    {
        int i(0);
        while (widgets_[i] != focusedWidget_ && i < widgets_.size() - 1)
            ++i;
        if (i < widgets_.size() - 1)
            ++i;
        while (!widgets_[i]->isTabable() && i < widgets_.size() - 1)
            ++i;

        if (widgets_[i]->isTabable() && widgets_[i] != focusedWidget_)
        {
            menus::clearFocus();
            focusedWidget_ = widgets_[i];
            focusedWidget_->setFocus(focusedWidget_, false);
            return false;
        }
        else
        {
            focusedWidget_ = nullptr;
            return true;
        }
    }
    return false;
}

auto Tab::tabPrevious() -> bool
{
    if (!focusedWidget_ && widgets_.size() > 0)
    {
        int i(widgets_.size() - 1);
        while (i >= 0 && !widgets_[i]->isTabable())
            --i;
        if (i >= 0)
        {
            focusedWidget_ = widgets_[i];
            focusedWidget_->setFocus(focusedWidget_, true);
            return false;
        }
        else
            return true;
    }
    else if (focusedWidget_->tabPrevious())
    {
        int i(widgets_.size() - 1);
        while (widgets_[i] != focusedWidget_ && i > 0)
            --i;
        if (i > 0)
            --i;
        while (!widgets_[i]->isTabable() && i > 0)
            --i;

        if (widgets_[i]->isTabable() && widgets_[i] != focusedWidget_)
        {
            menus::clearFocus();
            focusedWidget_ = widgets_[i];
            focusedWidget_->setFocus(focusedWidget_, true);
            return false;
        }
        else
        {
            focusedWidget_ = nullptr;
            return true;
        }
    }
    return false;
}

void Tab::textEntered(sf::Uint32 keyCode)
{
    if (active_ && focusedWidget_)
        focusedWidget_->textEntered(keyCode);
}

void Tab::draw() const
{
    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    Vector2f origin = getTopLeft() + topLeft_ * mirror - Vector2f(0.f, 10.f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!focusedWidget_ && parent_->isFocused() && isTopMost() && active_)
    {
        glBegin(GL_QUADS);
        glColor4f(1, 0.5, 0.8, 0.7);
        glVertex2f(origin.x_ + width(), origin.y_);
        glVertex2f(origin.x_, origin.y_);
        glColor4f(0.5, 0.25, 0.4, 0.0);
        glVertex2f(origin.x_, origin.y_ + height_);
        glVertex2f(origin.x_ + width(), origin.y_ + height_);
        glEnd();
    }

    if (active_)
    {
        if (isTopMost())
        {
            glBegin(GL_QUADS);
            glColor4f(1, 0.5, 0.8, 0.7);
            glVertex2f(origin.x_ + width(), origin.y_);
            glVertex2f(origin.x_, origin.y_);
            glColor4f(0.5, 0.25, 0.4, 0.0);
            glVertex2f(origin.x_, origin.y_ + height_ * 0.5);
            glVertex2f(origin.x_ + width(), origin.y_ + height_ * 0.5);
            glEnd();
        }

        glLineWidth(2.f);
        glBegin(GL_LINE_STRIP);
        if (isTopMost())
            glColor4f(1.f, 0.5f, 0.8f, 1.0f);
        else
            glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
        glVertex2f(origin.x_, origin.y_ + height_);
        glVertex2f(origin.x_, origin.y_);
        glVertex2f(origin.x_ + width(), origin.y_);
        glVertex2f(origin.x_ + width(), origin.y_ + height_);
        glEnd();
    }
    else
    {
        glLineWidth(2.f);
        glBegin(GL_LINES);
        if (isTopMost())
            glColor4f(1.f, 0.5f, 0.8f, 1.0f);
        else
            glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
        glVertex2f(origin.x_, origin.y_ + height_);
        glVertex2f(origin.x_ + width(), origin.y_ + height_);
        glEnd();
    }

    // draw Label
    label_->draw();

    if (active_)
        for (auto widget : widgets_)
            widget->draw();
}

void Tab::setFocus(UiElement * toBeFocused, bool isPrevious)
{
    UiElement::setFocus(this, isPrevious);
    if (toBeFocused != this)
        focusedWidget_ = toBeFocused;
    else
        focusedWidget_ = nullptr;
    label_->setFocus(this, isPrevious);
}

void Tab::clearFocus()
{
    UiElement::clearFocus();
    focusedWidget_ = nullptr;
    for (auto & widget : widgets_)
        widget->clearFocus();
    label_->clearFocus();
}

void Tab::addWidget(UiElement * toBeAdded)
{
    toBeAdded->setParent(this);
    widgets_.push_back(toBeAdded);
}

auto Tab::getTopLeft() const -> Vector2f
{
    if (locales::getCurrentLocale().LTR_)
        return UiElement::getTopLeft() - topLeft_ + Vector2f(0.f, 10.f);
    else
        return UiElement::getTopLeft() - Vector2f(-topLeft_.x_, topLeft_.y_) +
               Vector2f(0.f, 10.f);
}
