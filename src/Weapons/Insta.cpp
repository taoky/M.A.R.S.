/* Insta.cpp

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

#include "Weapons/Insta.hpp"

#include <GL/gl.h>
#include <cfloat>
#include <cmath>

#include "Media/sound.hpp"
#include "Particles/particles.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Ship.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/timer.hpp"
#include "Teams/Team.hpp"

void Insta::draw(float alpha) const
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    parent_->getOwner()->team()->color().brightened().gl4f(alpha);
    const int posX = 2;
    const int posY = 31;
    glBegin(GL_QUADS);
    glTexCoord2f(posX * 0.125f, posY * 0.03125f);
    glVertex2f(0, parent_->radius() * 0.2f);
    glTexCoord2f(posX * 0.125f, (posY + 1) * 0.03125f);
    glVertex2f(0, -1.f * parent_->radius() * 0.2f);
    glTexCoord2f((posX + 1) * 0.125f, (posY + 1) * 0.03125f);
    glVertex2f(parent_->radius() * 4.5f, -1.f * parent_->radius() * 0.2f);
    glTexCoord2f((posX + 1) * 0.125f, posY * 0.03125f);
    glVertex2f(parent_->radius() * 4.5f, parent_->radius() * 0.2f);
    glEnd();
}

void Insta::fire() const
{
    float time = timer::totalTime();
    if (time - timer_ > 3.0)
    {
        timer_ = time;
        float angleRad = parent_->rotation() * M_PI / 180;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        Color3f tmp = parent_->getOwner()->team()->color().brightened();
        particles::spawn(
            particles::pAmmoInsta,
            parent_->location() + faceDirection * parent_->radius(),
            faceDirection, parent_->velocity(), tmp, parent_->getOwner());
        sound::playSound(sound::Sniper, parent_->location());
    }
}

auto Insta::maxDistance() const -> float { return FLT_MAX; }

auto Insta::minDistance() const -> float { return 500.f; }

auto Insta::maxAngle() const -> float { return 30.f; }
