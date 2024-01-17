/* BurningFragment.hpp

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

#ifndef BURNINGFRAGMENT_HPP_INCLUDED
#define BURNINGFRAGMENT_HPP_INCLUDED

#include <list>
#include <memory>

#include "Particles/Particle.hpp"
#include "System/Color3f.hpp"

class Player;
class SpaceObject;
struct Vector2f;

class BurningFragment : public Particle<BurningFragment>
{
  public:
    BurningFragment(Vector2f const & location, Vector2f const & direction,
                    Vector2f const & velocity, Color3f const & color,
                    Player * damageSource);
    ~BurningFragment();

    void update();
    void draw() const;

    void onCollision(SpaceObject * with, Vector2f const & location,
                     Vector2f const & direction, Vector2f const & velocity);

    friend class Particle<BurningFragment>;

  private:
    Color3f color_;
    float timer1_, timer2_;
    static std::list<std::shared_ptr<BurningFragment>> activeParticles_;
};

#endif // BURNINGFRAGMENT_HPP_INCLUDED
