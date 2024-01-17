/* ROFLE.cpp

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

#include "Weapons/ROFLE.hpp"

#include <GL/gl.h>
#include <cfloat>
#include <cmath>

#include "Media/sound.hpp"
#include "Particles/particles.hpp"
#include "SpaceObjects/Ship.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/timer.hpp"

void ROFLE::draw(float alpha) const
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 0.4, 0.4, alpha);
    const int posX = 0;
    const int posY = 31;
    glBegin(GL_QUADS);
    glTexCoord2f(posX * 0.125f, posY * 0.03125f);
    glVertex2f(0, parent_->radius() * 0.2f);
    glTexCoord2f(posX * 0.125f, (posY + 1) * 0.03125f);
    glVertex2f(0, -1.f * parent_->radius() * 0.2f);
    glTexCoord2f((posX + 1) * 0.125f, (posY + 1) * 0.03125f);
    glVertex2f(parent_->radius() * 6.f, -1.f * parent_->radius() * 0.2f);
    glTexCoord2f((posX + 1) * 0.125f, posY * 0.03125f);
    glVertex2f(parent_->radius() * 6.f, parent_->radius() * 0.2f);
    glEnd();
}

void ROFLE::fire() const
{
    float time = timer::totalTime();
    if (time - timer_ <= 3.0)
        return;
    timer_ = time;
    float angleRad = parent_->rotation() * M_PI / 180;
    Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
    particles::spawn(particles::pAmmoROFLE,
                     parent_->location() + faceDirection * parent_->radius(),
                     faceDirection, parent_->velocity(), Color3f(),
                     parent_->getOwner());
    sound::playSound(sound::Sniper, parent_->location());
}

auto ROFLE::maxDistance() const -> float { return FLT_MAX; }

auto ROFLE::minDistance() const -> float { return 0.f; }

auto ROFLE::maxAngle() const -> float { return 5.f; }
