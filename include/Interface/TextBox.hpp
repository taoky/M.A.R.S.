/* TextBox.hpp

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

#ifndef TEXTBOX_HPP_INCLUDED
#define TEXTBOX_HPP_INCLUDED

#include <vector>

#include "Interface/Label.hpp"
#include "Interface/UiElement.hpp"
#include "Interface/VerticalSlider.hpp"
#include "System/Color3f.hpp"

class VerticalSlider;
namespace sf
{
class String;
} // namespace sf
struct Vector2f;

class TextBox : public UiElement
{
  public:
    TextBox(sf::String * text, Vector2f const & topLeft, int width, int height,
            Color3f const & color = Color3f(0.7f, 0.7f, 0.7f));
    ~TextBox();

    void mouseMoved(Vector2f const & position);
    void mouseWheelMoved(Vector2f const & position, int delta);
    void mouseLeft(bool down);

    void draw() const;

    bool isTabable() const { return false; }

  private:
    Color3f color_;
    std::vector<sf::String *> texts_;
    VerticalSlider * slider_;
    mutable float position_;
    mutable float scrollSpeed_;
};

#endif
