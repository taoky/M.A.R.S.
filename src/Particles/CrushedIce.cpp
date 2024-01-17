/* CrushedIce.cpp

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

#include "Particles/CrushedIce.hpp"

#include <GL/gl.h>

#include "SpaceObjects/physics.hpp"
#include "SpaceObjects/spaceObjects.hpp"
#include "System/Vector2f.hpp"
#include "System/randomizer.hpp"
#include "System/settings.hpp"
#include "System/timer.hpp"

class Player;

std::list<std::shared_ptr<CrushedIce>> CrushedIce::activeParticles_;

CrushedIce::CrushedIce(Vector2f const & location, Vector2f const & direction,
                       Vector2f const & velocity, Color3f const & color,
                       Player * damageSource)
    : Particle<CrushedIce>(spaceObjects::oDust, location, 4, 0,
                           randomizer::random(0.5f, 1.5f) *
                               settings::C_globalParticleLifeTime / 100.f)
{

    velocity_ = Vector2f::randDir() * 20 * randomizer::random(1.f, 2.f) *
                randomizer::random(1.f, 2.f);

    color_ = Color3f::random();
    color_.s(0.0);
}

void CrushedIce::update()
{
    float time = timer::frameTime();

    color_.v(-0.7 / totalLifeTime_ * lifeTime_ + 0.7);
    physics::collide(this, STATICS);
    Vector2f acceleration = physics::attract(this) * 5.f;

    location_ += velocity_ * time + acceleration * time * time;
    velocity_ += acceleration * time;

    lifeTime_ += time;
}

void CrushedIce::draw() const
{
    color_.gl4f(1.0f);
    const int posX = 5;
    const int posY = 1;
    glTexCoord2f(posX * 0.125f, posY * 0.125f);
    glVertex2f(location_.x_ - radius_, location_.y_ - radius_);
    glTexCoord2f(posX * 0.125f, (posY + 1) * 0.125f);
    glVertex2f(location_.x_ - radius_, location_.y_ + radius_);
    glTexCoord2f((posX + 1) * 0.125f, (posY + 1) * 0.125f);
    glVertex2f(location_.x_ + radius_, location_.y_ + radius_);
    glTexCoord2f((posX + 1) * 0.125f, posY * 0.125f);
    glVertex2f(location_.x_ + radius_, location_.y_ - radius_);
}

void CrushedIce::shockWave(Vector2f const & location, float strength,
                           float radius)
{
    for (auto & activeParticle : activeParticles_)
    {
        Vector2f direction(activeParticle->location_ - location);
        float distance = direction.length();
        if (distance < radius && direction != Vector2f())
        {
            float intensity = radius - distance;
            direction = direction.normalize();
            direction *= intensity;
            activeParticle->velocity_ += direction;
        }
    }
}
