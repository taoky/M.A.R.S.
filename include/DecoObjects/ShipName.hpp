/* ShipName.hpp

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

#ifndef SHIPNAME_HPP_INCLUDED
#define SHIPNAME_HPP_INCLUDED

#include "DecoObjects/DecoObject.hpp"

class Ship;

/// An object displaying the players name at his Ship.

class ShipName : public DecoObject
{
  public:
    ShipName(Ship * ship) : ship_(ship) {}

    void draw() const;

  private:
    Ship * ship_;
};

#endif // SHIPNAME_HPP_INCLUDED
