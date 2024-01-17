/* Freezer.cpp

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

#include "Specials/Freezer.hpp"

#include <GL/gl.h>
#include <cmath>
#include <list>
#include <memory>
#include <vector>

#include "DecoObjects/decoObjects.hpp"
#include "Games/games.hpp"
#include "Items/items.hpp"
#include "Menu/menus.hpp"
#include "Particles/AmmoRocket.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Ball.hpp"
#include "SpaceObjects/Ship.hpp"
#include "SpaceObjects/balls.hpp"
#include "SpaceObjects/ships.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/timer.hpp"
#include "Teams/Team.hpp"

void Freezer::draw(float alpha) const
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    parent_->getOwner()->team()->color().brightened().gl4f(0.8f * alpha);
    ;

    const int posX = 2;
    const int posY = 0;

    glBegin(GL_QUADS);
    glTexCoord2f(posX * 0.25f, posY * 0.25f);
    glVertex2f(-parent_->radius() * 4, -parent_->radius() * 4);
    glTexCoord2f(posX * 0.25f, (posY + 1) * 0.25f);
    glVertex2f(-parent_->radius() * 4, parent_->radius() * 4);
    glTexCoord2f((posX + 1) * 0.25f, (posY + 1) * 0.25f);
    glVertex2f(parent_->radius() * 4, parent_->radius() * 4);
    glTexCoord2f((posX + 1) * 0.25f, posY * 0.25f);
    glVertex2f(parent_->radius() * 4, -parent_->radius() * 4);
    glEnd();

    if (timer_ > 0.f)
    {
        float alpha(0.f);
        if (timer_ > 0.4f)
            alpha = std::pow(0.5f - timer_, 2) * 100.f;
        else
            alpha = -2.5f * (0.5f - timer_) + 1.25f;

        float scale(1.f - timer_);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);

        const int posX = 2;
        const int posY = 1;

        glBegin(GL_QUADS);
        glTexCoord2f(posX * 0.25f, posY * 0.25f);
        glVertex2f(-radius_ * scale, -radius_ * scale);
        glTexCoord2f(posX * 0.25f, (posY + 1) * 0.25f);
        glVertex2f(-radius_ * scale, radius_ * scale);
        glTexCoord2f((posX + 1) * 0.25f, (posY + 1) * 0.25f);
        glVertex2f(radius_ * scale, radius_ * scale);
        glTexCoord2f((posX + 1) * 0.25f, posY * 0.25f);
        glVertex2f(radius_ * scale, -radius_ * scale);
        glEnd();

        if (!menus::visible() || games::type() == games::gMenu)
            timer_ -= timer::frameTime() * 2.f;
    }
}

void Freezer::activate() const
{
    if (parent_->fragStars_ > 0 && timer_ <= 0.f)
    {

        radius_ = radius();
        const float strength = parent_->fragStars_ * 14.f;

        auto const & ships = ships::getShips();
        for (const auto & ship : ships)
        {
            if ((ship.get()) != parent_ && ship->collidable() &&
                !ship->collectedPowerUps_[items::puShield])
            {
                float distance(
                    (ship->location() - parent_->location()).length());
                if (distance <= radius_)
                {
                    ship->setDamageSource(parent_->getOwner());
                    ship->velocity_ = Vector2f();
                    ship->mass_ = 9999999999.f;
                    if (ship->frozen_ <= 0)
                        decoObjects::addIce(ship);
                    ship->frozen_ = strength;
                }
            }
        }
        auto ball = balls::getBall();

        if (ball && ball->visible_)
        {
            float distance((ball->location() - parent_->location()).length());
            if (distance <= radius_)
            {
                ball->velocity_ = Vector2f();
                ball->mass_ = 9999999999.f;
                if (ball->frozen_ <= 0)
                    decoObjects::addIce(ball);
                ball->frozen_ = strength;
            }
        }

        for (auto & activeParticle : AmmoRocket::activeParticles_)
        {
            float distance(
                (activeParticle->location() - parent_->location()).length());
            if (distance <= radius_)
            {
                activeParticle->velocity_ =
                    activeParticle->velocity_ * 0.00001f;
                activeParticle->mass_ = 9999999999.f;
                if (activeParticle->frozen_ <= 0)
                    decoObjects::addIce(activeParticle);
                activeParticle->frozen_ = strength;
            }
        }

        timer_ = 0.5f;
        parent_->fragStars_ = 0;
    }
}

auto Freezer::radius() const -> float
{
    return (parent_->fragStars_ > 0 ? parent_->fragStars_ * 50.f + 150.f : 0.f);
}
