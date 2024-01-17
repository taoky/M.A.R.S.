/* RadioGroup.cpp

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

#include "Interface/RadioGroup.hpp"

#include <algorithm>

#include "Interface/RadioButton.hpp"
#include "Menu/menus.hpp"
#include "System/Vector2f.hpp"

RadioGroup::RadioGroup() : UiElement(Vector2f(), 0, 0) {}

RadioGroup::~RadioGroup()
{
    for (auto & radioButton : radioButtons_)
        delete radioButton;
}

void RadioGroup::mouseMoved(Vector2f const & position)
{
    for (auto & radioButton : radioButtons_)
        radioButton->mouseMoved(position);
}

void RadioGroup::mouseLeft(bool down)
{
    for (auto & radioButton : radioButtons_)
        radioButton->mouseLeft(down);
}

void RadioGroup::keyEvent(bool down, Key const & key)
{
    if (focusedButton_)
        focusedButton_->keyEvent(down, key);
}

auto RadioGroup::tabNext() -> bool
{
    if (radioButtons_.size() > 0)
    {
        if (focusedButton_)
        {
            int i(0);
            while (i < radioButtons_.size() &&
                   radioButtons_[i] != focusedButton_)
                ++i;
            ++i;

            if (i < radioButtons_.size())
            {
                menus::clearFocus();
                focusedButton_ = radioButtons_[i];
                focusedButton_->setFocus(focusedButton_, false);
                return false;
            }
        }
        else
        {
            menus::clearFocus();
            focusedButton_ = radioButtons_[0];
            focusedButton_->setFocus(focusedButton_, false);
            return false;
        }
    }
    return true;
}

auto RadioGroup::tabPrevious() -> bool
{
    if (radioButtons_.size() > 0)
    {
        if (focusedButton_)
        {
            int i(radioButtons_.size() - 1);
            while (i >= 0 && radioButtons_[i] != focusedButton_)
                --i;
            --i;

            if (i >= 0)
            {
                menus::clearFocus();
                focusedButton_ = radioButtons_[i];
                focusedButton_->setFocus(focusedButton_, false);
                return false;
            }
        }
        else
        {
            menus::clearFocus();
            focusedButton_ = radioButtons_[radioButtons_.size() - 1];
            focusedButton_->setFocus(focusedButton_, false);
            return false;
        }
    }
    return true;
}

void RadioGroup::setFocus(UiElement * toBeFocused, bool isPrevious)
{
    UiElement::setFocus(this, isPrevious);
    auto * toFocus = dynamic_cast<RadioButton *>(toBeFocused);
    if (toFocus)
    {
        focusedButton_ = toFocus;
    }
    else if (!focusedButton_)
    {
        if (isPrevious)
            tabPrevious();
        else
            tabNext();
    }
}

void RadioGroup::clearFocus()
{
    UiElement::clearFocus();
    focusedButton_ = nullptr;
    for (auto & radioButton : radioButtons_)
        radioButton->clearFocus();
}

void RadioGroup::draw() const
{
    for (auto radioButton : radioButtons_)
        radioButton->draw();
}

void RadioGroup::addRadioButton(RadioButton * toBeAdded)
{
    ;
    toBeAdded->setParent(this);
    radioButtons_.push_back(toBeAdded);
}

void RadioGroup::allOff()
{
    for (auto & radioButton : radioButtons_)
        *(radioButton->value_) = false;
}
