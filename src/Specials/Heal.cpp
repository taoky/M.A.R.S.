/* Heal.cpp

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

#include "Specials/Heal.hpp"

#include <GL/gl.h>
#include <cmath>
#include <memory>
#include <vector>

#include "Games/games.hpp"
#include "Menu/menus.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Ship.hpp"
#include "SpaceObjects/ships.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/timer.hpp"
#include "Teams/Team.hpp"

void Heal::draw(float alpha) const
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    alpha *= 0.6 + std::sin(timer::totalTime() * 6) * 0.1f;
    parent_->getOwner()->team()->color().brightened().gl4f(alpha);

    const int posX = 1;
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

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);

        const int posX = 1;
        const int posY = 1;

        glBegin(GL_QUADS);
        glTexCoord2f(posX * 0.25f, posY * 0.25f);
        glVertex2f(-radius_, -radius_);
        glTexCoord2f(posX * 0.25f, (posY + 1) * 0.25f);
        glVertex2f(-radius_, radius_);
        glTexCoord2f((posX + 1) * 0.25f, (posY + 1) * 0.25f);
        glVertex2f(radius_, radius_);
        glTexCoord2f((posX + 1) * 0.25f, posY * 0.25f);
        glVertex2f(radius_, -radius_);
        glEnd();

        if (!menus::visible() || games::type() == games::gMenu)
            timer_ -= timer::frameTime();
    }
}

void Heal::activate() const
{
    if (parent_->fragStars_ > 0 && timer_ <= 0.f)
    {
        radius_ = radius();
        auto const & ships = ships::getShips();
        for (const auto & ship : ships)
        {
            if (ship.get() != parent_)
            {
                float distance(
                    (ship->location() - parent_->location()).length());
                if (ship->collidable() &&
                    parent_->getOwner()->team() == ship->getOwner()->team() &&
                    distance <= radius_)
                {
                    ship->heal(parent_->getOwner(),
                               ((radius_ / distance) - 0.8f) *
                                   parent_->fragStars_ * 30);
                    ship->refuel(parent_->getOwner(),
                                 ((radius_ / distance) - 0.8f) *
                                     parent_->fragStars_ * 30);
                }
            }
            else
            {
                parent_->heal(parent_->getOwner(), parent_->fragStars_ * 30);
                parent_->refuel(parent_->getOwner(), parent_->fragStars_ * 30);
            }
        }
        timer_ = 0.5f;
        parent_->fragStars_ = 0;
    }
}

auto Heal::radius() const -> float
{
    return (parent_->fragStars_ > 0 ? parent_->fragStars_ * 50.f + 50.f : 0.f);
}
