/* MiniAmmoFlubba.cpp

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

#include "Particles/MiniAmmoFlubba.hpp"

#include <GL/gl.h>
#include <math.h>

#include "Media/sound.hpp"
#include "Particles/particles.hpp"
#include "SpaceObjects/SpaceObject.hpp"
#include "SpaceObjects/physics.hpp"
#include "SpaceObjects/spaceObjects.hpp"
#include "System/Vector2f.hpp"
#include "System/randomizer.hpp"
#include "System/timer.hpp"

class Player;

std::list<std::shared_ptr<MiniAmmoFlubba>> MiniAmmoFlubba::activeParticles_;

MiniAmmoFlubba::MiniAmmoFlubba(Vector2f const & location,
                               Vector2f const & direction,
                               Vector2f const & velocity, Color3f const & color,
                               Player * damageSource)
    : Particle<MiniAmmoFlubba>(spaceObjects::oMiniAmmoFlubba, location, 8.f,
                               0.2f, randomizer::random(10.f, 17.f))
{

    setDamageSource(damageSource);

    velocity_ = Vector2f::randDirLen() * 400;

    location_ += Vector2f::randDir() * 5.f;

    radius_ = randomizer::random(2.f, 3.f);

    color_ =
        Color3f(randomizer::random(0.0f, 0.4f), randomizer::random(0.8f, 1.f),
                randomizer::random(0.0f, 0.4f));
}

void MiniAmmoFlubba::update()
{
    float time = timer::frameTime();

    physics::collide(this, STATICS | MOBILES | PARTICLES);
    Vector2f acceleration = physics::attract(this) * 3.f;

    // update Size
    if (lifeTime_ > totalLifeTime_ - 0.2f)
        radius_ = -200.0 * pow(lifeTime_ + 0.1 - totalLifeTime_, 2) + 4;

    location_ += velocity_ * time + acceleration * time * time;
    velocity_ += acceleration * time - 8 * velocity_ * time;

    lifeTime_ += time;

    if (lifeTime_ > totalLifeTime_)
    {
        particles::spawnMultiple(2, particles::pMud, location_, Vector2f(),
                                 Vector2f(), color_);
        sound::playSound(sound::BlubPop, location_);
    }
}

void MiniAmmoFlubba::draw() const
{
    color_.gl4f(0.8f);
    const int posX = 4;
    const int posY = 0;
    glTexCoord2f(posX * 0.125f, posY * 0.125f);
    glVertex2f(location_.x_ - radius_, location_.y_ - radius_);
    glTexCoord2f(posX * 0.125f, (posY + 1) * 0.125f);
    glVertex2f(location_.x_ - radius_, location_.y_ + radius_);
    glTexCoord2f((posX + 1) * 0.125f, (posY + 1) * 0.125f);
    glVertex2f(location_.x_ + radius_, location_.y_ + radius_);
    glTexCoord2f((posX + 1) * 0.125f, posY * 0.125f);
    glVertex2f(location_.x_ + radius_, location_.y_ - radius_);
}

void MiniAmmoFlubba::onCollision(SpaceObject * with, Vector2f const & location,
                                 Vector2f const & direction,
                                 Vector2f const & velocity)
{
    if (!isDead() && with->type() != spaceObjects::oAmmoFlubba &&
        with->type() != spaceObjects::oMiniAmmoFlubba)
    {
        killMe();
    }
}

void MiniAmmoFlubba::shockWave(Vector2f const & location, float strength,
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
