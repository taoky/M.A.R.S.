/* decoObjects.hpp

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

#ifndef DECOOBJECTS_HPP_INCLUDED
#define DECOOBJECTS_HPP_INCLUDED

#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"

class Planet;
class Ship;
class Ball;
class AmmoRocket;
class Sun;
class Bolt;
class DecoObject;
class SpaceObject;

/// A namespace which handles objects in the space, which can't be interacted
/// with.

namespace decoObjects
{
/// Updates every DecoObject.
void update();

/// Draws every DecoObject. Except for the ShipNames and the SunHeats.
void draw();

/// Draws every SunHeat.
void drawHeat();

/// Draws every ShipName.
void drawNames();

/// Draws an arrow between the given points.
void drawArrow(Vector2f const & from, Vector2f const & to,
               Color3f const & color, float width = 10.f);

/// Adds the CannonKeep Cannon.
void addCannon();

/// Adds a random PlanetSign on the target Planet.
/// \param planet The Planet to be decored.
void addPlanetSign(Planet * planet);

/// Adds a heat texture on the target Sun.
/// \param sun The Sun to be decored.
void addSunHeat(Sun * sun);

/// Adds an ice block at the ship's position.
/// \param ship The ship to be frozen.
void addIce(Ship * ship);

/// Adds an ice block at the ball's position.
/// \param ball The ball to be frozen.
void addIce(Ball * ball);

/// Adds an ice block on the rocket's position.
/// \param rocket The rocket to be frozen.
void addIce(AmmoRocket * rocket);

/// Removes an ice block from the list.
/// \param toBeRemoved The ice block to be removed.
void removeIce(DecoObject const * toBeRemoved);

/// Adds a Bolt between two Ships.
/// \param from The Ship the Bolt is coming from.
/// \param to The Ship the Bolt is going to.
/// \param width The Bolt's width.
void addBolt(SpaceObject * from, SpaceObject * to, float width);

/// Removes a Bolt from the Bolt list.
/// \param toBeRemoved The Bolt to be removed.
void removeBolt(DecoObject const * toBeRemoved);

/// Adds a ShipName to the target Ship.
/// \param ship The Ship to be named.
void addName(Ship * ship);

/// Adds a ShipHighlight to the target Ship.
/// \param ship The Ship to be highlighted.
void addHighlight(Ship * ship);

/// Deletes every DecoObject.
void clear();
}; // namespace decoObjects

#endif // DECOOBJECTS_HPP_INCLUDED
