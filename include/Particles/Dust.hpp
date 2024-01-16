/* Dust.hpp

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

#ifndef DUST_HPP_INCLUDED
#define DUST_HPP_INCLUDED

#include "Particles/Particle.hpp"

#include "System/Color3f.hpp"

class Dust : public Particle<Dust>
{
  public:
    Dust(Vector2f const & location, Vector2f const & direction,
         Vector2f const & velocity, Color3f const & color,
         Player * damageSource);

    void update();
    void draw() const;

    static void shockWave(Vector2f const & location, float strength,
                          float radius);

    friend class Particle<Dust>;

  private:
    Color3f color_;
    static std::list<std::unique_ptr<Dust>> activeParticles_;
};

#endif // DUST_HPP_INCLUDED
