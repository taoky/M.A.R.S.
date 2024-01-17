/* Fist.cpp

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

#include "Weapons/Fist.hpp"

#include <GL/gl.h>
#include <cmath>

#include "Games/games.hpp"
#include "Media/sound.hpp"
#include "Menu/menus.hpp"
#include "Particles/particles.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Ship.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/timer.hpp"
#include "Teams/Team.hpp"

void Fist::draw(float alpha) const
{
    if (!menus::visible() || games::type() == games::gMenu)
    {
        float time = timer::totalTime();
        if (time - timer_ < 0.1f)
            position_ = (time - timer_) * 20.f;
        else if (time - timer_ < 0.5f)
            position_ = (0.5f - (time - timer_)) * 5.f;
        else
            position_ = 0.f;
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    glBegin(GL_QUADS);
    glTexCoord2f(0.109375f, 0.953125f);
    glVertex2f(0.f, parent_->radius() * 0.5f);
    glTexCoord2f(0.140625f, 0.984375f);
    glVertex2f(0.f, -1.f * parent_->radius() * 0.5f);
    glTexCoord2f(0.234375f, 0.890625f);
    glVertex2f(position_ * parent_->radius() * (position_ + 1) * 1.2f,
               (-1.f * parent_->radius() * 0.5f) * (1 + position_) * 0.7f);
    glTexCoord2f(0.203125f, 0.859375f);
    glVertex2f(position_ * parent_->radius() * (position_ + 1) * 1.2f,
               (parent_->radius() * 0.5f) * (1 + position_) * 0.7f);
    glEnd();

    parent_->getOwner()->team()->color().gl4f(alpha);
    const float posX = 2;
    const float posY = 28;
    glBegin(GL_QUADS);
    glTexCoord2f(posX * 0.125f, posY * 0.03125f);
    glVertex2f(position_ * parent_->radius() * (position_ + 1),
               (parent_->radius() * 0.5f) * (1 + position_));
    glTexCoord2f(posX * 0.125f, (posY + 2) * 0.03125f);
    glVertex2f(position_ * parent_->radius() * (position_ + 1),
               (-1.f * parent_->radius() * 0.5f) * (1 + position_));
    glTexCoord2f((posX + 0.5f) * 0.125f, (posY + 2) * 0.03125f);
    glVertex2f(parent_->radius() * (1.f + position_) * (position_ + 1),
               (-1.f * parent_->radius() * 0.5f) * (1 + position_));
    glTexCoord2f((posX + 0.5f) * 0.125f, posY * 0.03125f);
    glVertex2f(parent_->radius() * (1.f + position_) * (position_ + 1),
               (parent_->radius() * 0.5f) * (1 + position_));
    glEnd();
}

void Fist::fire() const
{
    float time = timer::totalTime();
    if (time - timer_ > 0.5f)
    {
        timer_ = time;
        float angleRad = parent_->rotation() * M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));

        particles::spawn(
            particles::pAmmoFist,
            parent_->location() + faceDirection * parent_->radius(),
            faceDirection, parent_->velocity(), Color3f(), parent_->getOwner());

        parent_->velocity() -= faceDirection * 200.f;
        sound::playSound(sound::Pump, parent_->location());
    }
}

auto Fist::maxDistance() const -> float { return 75.f; }

auto Fist::minDistance() const -> float { return 0.f; }

auto Fist::maxAngle() const -> float { return 10.f; }
