/* Shocker.cpp

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

#include "Specials/Shocker.hpp"

#include <GL/gl.h>
#include <cmath>
#include <memory>
#include <vector>

#include "DecoObjects/decoObjects.hpp"
#include "Games/games.hpp"
#include "Menu/menus.hpp"
#include "Particles/particles.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Ball.hpp"
#include "SpaceObjects/Ship.hpp"
#include "SpaceObjects/balls.hpp"
#include "SpaceObjects/ships.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/timer.hpp"
#include "Teams/Team.hpp"

void Shocker::draw(float alpha) const
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    parent_->getOwner()->team()->color().brightened().gl4f(0.8f * alpha);

    const int posX = 3;
    const int posY = 1;

    float scale(4 + std::sin(timer::totalTime() * 6) * 0.3f);

    glBegin(GL_QUADS);
    glTexCoord2f(posX * 0.25f, posY * 0.25f);
    glVertex2f(-parent_->radius() * scale, -parent_->radius() * scale);
    glTexCoord2f(posX * 0.25f, (posY + 1) * 0.25f);
    glVertex2f(-parent_->radius() * scale, parent_->radius() * scale);
    glTexCoord2f((posX + 1) * 0.25f, (posY + 1) * 0.25f);
    glVertex2f(parent_->radius() * scale, parent_->radius() * scale);
    glTexCoord2f((posX + 1) * 0.25f, posY * 0.25f);
    glVertex2f(parent_->radius() * scale, -parent_->radius() * scale);
    glEnd();

    if (timer_ > 0.f)
    {
        if (parent_->getLife() <= 0.f)
            timer_ = 0.f;

        Vector2f direction = Vector2f::randDir() * 5.f;
        particles::spawnMultiple(1, particles::pSpark, parent_->location_,
                                 direction * 60.f, parent_->velocity_,
                                 Color3f(0.9f, 0.9f, 1.f));

        if (!menus::visible() || games::type() == games::gMenu)
        {
            timer_ -= timer::frameTime();
        }
    }
}

void Shocker::activate() const
{
    if (parent_->fragStars_ > 0 && timer_ <= 0.f)
    {
        targets_.clear();
        ballTarget_ = nullptr;

        auto const & ships = ships::getShips();

        if (ships.size() >= 2)
        {

            for (const auto & ship : ships)
            {
                if (ship.get() != parent_)
                {
                    Vector2f direction(ship->location() - parent_->location());
                    float distance(direction.lengthSquare());
                    if (distance <= radius() * radius() && ship->attackable())
                    {
                        targets_.push_back(ship.get());
                    }
                }
            }
        }

        auto ball(balls::getBall());

        if (ball)
        {
            Vector2f direction(ball->location() - parent_->location());
            float distance(direction.lengthSquare());
            if (distance <= radius() * radius())
            {
                ballTarget_ = ball.get();
            }
        }

        int targetCount = targets_.size() + (ballTarget_ == nullptr ? 0 : 1);
        float damage = parent_->fragStars_ / 3.f * 200.f / targetCount;

        for (auto & target : targets_)
        {
            Vector2f direction(target->location() - parent_->location());

            decoObjects::addBolt(parent_, target, 100.f / targetCount);

            target->drainLife(parent_->getOwner(), damage, direction * 10);

            target->velocity_ += direction.normalize() * damage * 5.f;
        }

        if (ballTarget_)
        {
            if (ball->sticky_)
                ball->sticky_ = false;

            Vector2f direction(ballTarget_->location() - parent_->location());

            decoObjects::addBolt(parent_, ball.get(), 100.f / targetCount);
            ball->velocity_ += direction.normalize() * damage * 5.f;
        }

        parent_->fragStars_ = 0;
        timer_ = 0.5f;
    }
}

auto Shocker::radius() const -> float { return 300.f; }
