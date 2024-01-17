/* PUReverse.hpp

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

#ifndef PUREVERSE_HPP_INCLUDED
#define PUREVERSE_HPP_INCLUDED

#include <SFML/System.hpp>

#include "Items/PowerUp.hpp"
#include "Items/items.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/randomizer.hpp"

struct Vector2f;

class PUReverse : public PowerUp
{
  public:
    PUReverse(Vector2f const & location)
        : PowerUp(items::puReverse, location, 15.f,
                  randomizer::random(8.f, 12.f), 3, 0, Color3f(1.f, 0.2f, 0.8f))
    {
    }

    void draw() const;

  private:
    void refreshLifeTime();
};

#endif // PUREVERSE_HPP_INCLUDED
