/* window.hpp

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

#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Media/music.hpp"
#include "System/Key.hpp"
#include "System/Vector2f.hpp"

namespace sf
{
class Drawable;
class Shader;
} // namespace sf

/// A namespace handling the main window of the game.

namespace window
{
/// Opens the window.
/// Should be called once at the beginning.
/// Returns false, if something went wrong.
bool open();

/// Recreates the window.
/// Can be called to apply settings::C_fullScreen.
void create();

/// Closes the window.
/// Should be called once at the end.
void close();

/// Starts the main game loop.
/// Should be called, when the window is opened a game is created.
void mainLoop();

/// Starts a draw cycle.
/// The active OpenGL target is set to the window or the post effect backbuffer,
/// depending on the use of shaders.
void startDrawSpace();

/// Finishs drawing the space and starts drawing the HUD.
/// Applies post effects and sets the projection matrix to represent the screen
/// in pixels.
void startDrawHUD();

/// Draws a SFML drawable in the window.
void draw(sf::Drawable const & toBeDrawn, sf::RenderStates const & states,
          sf::Shader * shader = nullptr);

int isKeyDown(Key const & key);

Vector2f const getMousePosition();

void screenShot();

/// Shows or hides the mouse cursor.
void showCursor(bool show);

/// Converts the given location from space coordinates (max (SPACE_X_RESOLUTION,
/// SPACE_Y_RESOLUTION)) to pixel coordinates on the screen.
Vector2f const coordToPixel(Vector2f const & spaceCoord);

/// Converts the given location on screen to space coordinates (max
/// (SPACE_X_RESOLUTION, SPACE_Y_RESOLUTION)).
Vector2f const PixelToCoord(Vector2f const & screenCoord);

/// Returns a vector representing the size of the visible game area on screen.
/// In pixels.
Vector2f const & getViewPort();
} // namespace window

#endif // WINDOW_HPP_INCLUDED
