/* Heal.hpp

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

#ifndef HEAL_HPP_INCLUDED
#define HEAL_HPP_INCLUDED

#include <SFML/System/String.hpp>

#include "Specials/Special.hpp"
#include "Specials/specials.hpp"

class Ship;

/// Special: Heal.
/// A healing wave.

class Heal : public Special
{
  public:
    /// Ctor which constructs the special.
    Heal(Ship * parent)
        : Special(specials::sHeal, parent, sf::String("HEAL")), radius_(0.f){};

    /// Heals nearby friends.
    void activate() const;

    float radius() const;

    /// Draws the special.
    void draw(float alpha) const;

  private:
    mutable float radius_;
};

#endif // HEAL_HPP_INCLUDED
