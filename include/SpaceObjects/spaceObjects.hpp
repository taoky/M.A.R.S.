/* spaceObjects.hpp

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

#ifndef SPACEOBJECTS_HPP_INCLUDED
#define SPACEOBJECTS_HPP_INCLUDED

#include <memory>
#include <vector>

#define HOME_LEFT 0
#define HOME_RIGHT 1
#define HOME_MIDDLE 2
#define HOME_RALLY 3

#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"

class Home;
class SpaceObject;
struct Color3f;
struct Vector2f;

namespace spaceObjects
{
enum ObjectType
{
    oPlanet,
    oSun,
    oBlackHole,
    oHome,
    oShip,
    oBall,
    oFuel,
    oAmmoAFK47,
    oAmmoROFLE,
    oAmmoShotgun,
    oMud,
    oSmoke,
    oEruption,
    oDust,
    oExplode,
    oFragment,
    oBurningFragment,
    oFragmentFlame,
    oMiniFlame,
    oAmmoFlubba,
    oMiniAmmoFlubba,
    oCannonBall,
    oSpark,
    oAmmoBurner,
    oAmmoH2OMG,
    oHeat,
    oHeatJet,
    oShockWave,
    oHeatBurner,
    oMiniFlameSmoke,
    oAmmoRocket,
    oPowerUpCollect,
    oAmmoFist,
    oNumber,
    oAmmoInsta,
    oStar
};

void update();
void draw();

void addPlanet();
void addPlanet(Vector2f const & location, float radius);
void addSun();
void addBlackHole();
Home * addHome(int where, int life, Color3f const & color);
Home * addHome(Vector2f const & location, int life, Color3f const & color,
               float radius, float mass);

SpaceObject const * getObstacle(Vector2f const & start, Vector2f const & end,
                                bool avoidBall = false,
                                float minDistance = 0.f);
bool isOnLine(Vector2f const & source, Vector2f const & direction,
              Vector2f const & target, float maxAngle);

std::vector<Home *> const & getHomes();
std::vector<std::unique_ptr<SpaceObject>> const & getObjects();

/// Populates the space with planets, suns and black holes.
void populateSpace(float holePercentage, float sunPercentage, int maxObjects);

void clear();
} // namespace spaceObjects

#endif // SPACEOBJECTS_HPP_INCLUDED
