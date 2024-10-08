/* TacticalZone.hpp

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

#ifndef TACTICALZONE_HPP_INCLUDED
#define TACTICALZONE_HPP_INCLUDED

#include "System/Vector2f.hpp"
#include "Zones/Zone.hpp"

class SpaceObject;

/// Zone: TacticalZone.
/// A round Zone.

class TacticalZone : public Zone
{
  public:
    /// Ctor, which creates the Zone.
    TacticalZone(Vector2f const & location, float radius);

    /// Returns true, if the given SpaceObject is inside this Zone.
    bool isInside(SpaceObject const & toBeChecked) const;

    /// Updates the Zone.
    void update();

    /// Draws the zone.
    /// Only for debugging information.
    void draw() const;

    /// Returns a random point inside this Zone.
    Vector2f getRandomPoint() const;

    /// Getter for the loaction.
    Vector2f const & location() const;

    /// Returns true, if this Zone is covered.
    /// A Zone is covered, when the percentage of a teams ships being inside
    /// the Zone is larger than the percentage this zone does to the total
    /// tactical zone area of a team.
    bool covered() const;

    friend class BotController;

  private:
    float radius_;
    Vector2f location_;
    short homeSide_;
    bool covered_;
    float shipCount_;
};

#endif // TACTICALZONE_HPP_INCLUDED
