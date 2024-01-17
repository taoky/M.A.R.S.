/* SunHeat.hpp

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

#ifndef SUNHEAT_HPP_INCLUDED
#define SUNHEAT_HPP_INCLUDED

#include "DecoObjects/DecoObject.hpp"
#include "Media/texture.hpp"
#include "System/Vector2f.hpp"

class Sun;

/// A heat texture on a Sun.
/// Very decorative.

class SunHeat : public DecoObject
{
  public:
    /// Creates a heat texture on the target Sun.
    SunHeat(Sun * sun);

    /// Draws the texture.
    void draw() const;

  private:
    Vector2f location_;
    float radius_;
};

#endif // SUNHEAT_HPP_INCLUDED
