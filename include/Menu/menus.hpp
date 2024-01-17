/* menus.hpp

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

#ifndef MENUS_HPP_INCLUDED
#define MENUS_HPP_INCLUDED

#include <SFML/Config.hpp>
#include <SFML/Window.hpp>

#include "System/Key.hpp"
#include "System/Vector2f.hpp"

class UiWindow;
class UiElement;
struct Vector2f;

namespace menus
{

void draw();

// events
void mouseMoved(Vector2f const & position);
void mouseWheelMoved(Vector2f const & position, int delta);
void mouseLeft(bool down);
void keyEvent(bool down, Key const & key);
void textEntered(sf::Uint32 keyCode);

void showMain();
void showPause();

void showWindow(UiWindow * toBeShown);
void hideWindow();
void hideMenu();

void clearFocus();
void fixKeyboardOn(UiElement * target);
void unFixKeyboard();
bool keyboardFixed();

void reload();

bool visible();
} // namespace menus

#endif // MENUS_HPP_INCLUDED
