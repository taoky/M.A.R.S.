/* Heat.cpp

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

#include "Particles/Heat.hpp"

#include "System/randomizer.hpp"
#include "System/settings.hpp"
#include "System/timer.hpp"

std::list<std::unique_ptr<Heat>> Heat::activeParticles_;

Heat::Heat(Vector2f const & location, Vector2f const & direction,
           Vector2f const & velocity, Color3f const & color,
           Player * damageSource)
    : Particle<Heat>(spaceObjects::oHeat,
                     location + Vector2f::randDirLen() * 2.f, 4, 0,
                     randomizer::random(0.8f, 2.0f) *
                         settings::C_globalParticleLifeTime / 100.f)
{
}

void Heat::update()
{
    float time = timer::frameTime();
    Vector2f acceleration = physics::attract(this);

    // update Size
    radius_ = lifeTime_ * 10.f + 10;

    location_ = location_ + velocity_ * time - 2.f * acceleration * time * time;
    velocity_ = velocity_ - 2.f * acceleration * time + velocity_ * (-2) * time;

    lifeTime_ += time;
}

void Heat::draw() const
{
    glColor4f(1.f, 1.f, 1.f, 1.f - lifeTime_ / totalLifeTime_);
    const int posX = 3;
    const int posY = 1;
    glTexCoord2f(posX * 0.125f, posY * 0.125f);
    glVertex2f(location_.x_ - radius_, location_.y_ - radius_);
    glTexCoord2f(posX * 0.125f, (posY + 2) * 0.125f);
    glVertex2f(location_.x_ - radius_, location_.y_ + radius_);
    glTexCoord2f((posX + 2) * 0.125f, (posY + 2) * 0.125f);
    glVertex2f(location_.x_ + radius_, location_.y_ + radius_);
    glTexCoord2f((posX + 2) * 0.125f, posY * 0.125f);
    glVertex2f(location_.x_ + radius_, location_.y_ - radius_);
}
