/* FireWall.hpp

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

#ifndef FIREWALL_HPP_INCLUDED
#define FIREWALL_HPP_INCLUDED

#include "Specials/Special.hpp"

/// Special: FireWall.
/// A fire wave.

class FireWall : public Special
{
  public:
    /// Ctor which constructs the special.
    FireWall(Ship * parent)
        : Special(specials::sFireWall, parent, sf::String("FIRE WALL")),
          burnTimer_(0.f){};

    /// Blasts away nearby ships.
    void activate() const;

    float radius() const;

    /// Draws the special.
    void draw(float alpha) const;

  private:
    mutable float burnTimer_;
};

#endif // FIREWALL_HPP_INCLUDED
