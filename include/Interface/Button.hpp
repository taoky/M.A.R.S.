/* Button.hpp

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

#ifndef BUTTON_HPP_INCLUDED
#define BUTTON_HPP_INCLUDED

#include "Interface/Label.hpp"
#include "Interface/UiElement.hpp"
#include "Media/text.hpp"

class Button : public UiElement
{
  public:
    Button(sf::String * text, sf::String * toolTip, bool * key,
           Vector2f const & topLeft, int width, int height,
           int align = TEXT_ALIGN_CENTER, sf::Font * font = NULL);
    ~Button();

    void mouseMoved(Vector2f const & position);
    void mouseLeft(bool down);
    void keyEvent(bool down, Key const & key);

    void draw() const;

    void setFocus(UiElement * toBeFocused, bool isPrevious);
    void clearFocus();

  private:
    bool * key_;
    Label * label_;
    sf::String * toolTip_;
};

#endif
