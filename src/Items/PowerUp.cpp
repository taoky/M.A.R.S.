/* Item.cpp

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

#include "Items/PowerUp.hpp"

#include <GL/gl.h>
#include <cmath>
#include <memory>
#include <vector>

#include "Players/Player.hpp"
#include "SpaceObjects/Ship.hpp"
#include "SpaceObjects/ships.hpp"
#include "System/timer.hpp"

PowerUp::PowerUp(items::PowerUpType type, Vector2f const & location,
                 float radius, float totalLifeTime, int texX, int texY,
                 Color3f const & bgColor)
    : location_(location), radius_(radius), ships_(), collected_(false),
      type_(type), lifeTime_(0.f), totalLifeTime_(totalLifeTime), texX_(texX),
      texY_(texY), bgColor_(bgColor)
{
}

PowerUp::~PowerUp()
{
    for (auto & ship : ships_)
        ship->collectedPowerUps_[type_] = nullptr;
}

void PowerUp::update()
{

    lifeTime_ += timer::frameTime();

    if (!collected_)
    {
        auto const & shipList = ships::getShips();
        for (auto it = shipList.begin(); it != shipList.end(); ++it)
            if ((*it)->getLife() > 0.f &&
                ((*it)->location() - location_).lengthSquare() <
                    std::pow(radius_ * 2.f + (*it)->radius(), 2))
            {
                collected_ = true;
                if (type_ == items::puReverse || type_ == items::puSleep)
                {
                    for (const auto & ite : shipList)
                        if ((*it)->getOwner()->team() !=
                            ite->getOwner()->team())
                            ships_.push_back(ite.get());
                }
                else
                    ships_.push_back(it->get());
            }
        auto it = ships_.begin();
        while (it != ships_.end())
        {
            if ((*it)->collectedPowerUps_[type_] != nullptr)
            {
                (*it)->collectedPowerUps_[type_]->lifeTime_ = 0;
                it = ships_.erase(it);
                lifeTime_ = totalLifeTime_;
            }
            else
            {
                (*it)->collectedPowerUps_[type_] = this;
                refreshLifeTime();
                ++it;
            }
        }

        if (lifeTime_ < M_PI / 4.f)
            radius_ = std::sin(lifeTime_ * 2.f) * 15.f;
        else if (lifeTime_ > totalLifeTime_ - 0.38f)
            radius_ = -400.0 * pow(lifeTime_ + 0.2 - totalLifeTime_, 2) + 25;
        else
            radius_ = 15.f;
    }
    else
    {
        auto it = ships_.begin();
        while (it != ships_.end())
        {
            if (*it && (*it)->getLife() == 0.f)
            {
                (*it)->collectedPowerUps_[type_] = nullptr;
                it = ships_.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (ships_.empty())
            lifeTime_ = totalLifeTime_;
    }
}

void PowerUp::draw() const
{

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(location_.x_, location_.y_, 0);

    bgColor_.gl4f(0.8f);
    glRotatef(fmod(timer::totalTime() * 100.f, 360.f), 0.f, 0.f, 1.f);
    // glow
    glBegin(GL_QUADS);
    const int posX1 = 0;
    const int posY1 = 0;
    glTexCoord2f(posX1 * 0.15625f, posY1 * 0.15625f);
    glVertex2f(-radius_ * 2.f, -radius_ * 2.f);
    glTexCoord2f(posX1 * 0.15625f, (posY1 + 1) * 0.15625f);
    glVertex2f(-radius_ * 2.f, +radius_ * 2.f);
    glTexCoord2f((posX1 + 1) * 0.15625f, (posY1 + 1) * 0.15625f);
    glVertex2f(+radius_ * 2.f, +radius_ * 2.f);
    glTexCoord2f((posX1 + 1) * 0.15625f, posY1 * 0.15625f);
    glVertex2f(+radius_ * 2.f, -radius_ * 2.f);
    glEnd();

    glLoadIdentity();
    glTranslatef(location_.x_, location_.y_, 0);
    float scale(std::sin(timer::totalTime() * 7.f) / 4.f + 1.f);
    glScalef(scale, scale, 1.f);
    glColor3f(1.f, 1.f, 1.f);
    // item layer
    glBegin(GL_QUADS);
    const int posX2 = texX_;
    const int posY2 = texY_;
    glTexCoord2f(posX2 * 0.15625f, posY2 * 0.15625f);
    glVertex2f(-radius_, -radius_);
    glTexCoord2f(posX2 * 0.15625f, (posY2 + 1) * 0.15625f);
    glVertex2f(-radius_, +radius_);
    glTexCoord2f((posX2 + 1) * 0.15625f, (posY2 + 1) * 0.15625f);
    glVertex2f(+radius_, +radius_);
    glTexCoord2f((posX2 + 1) * 0.15625f, posY2 * 0.15625f);
    glVertex2f(+radius_, -radius_);
    glEnd();

    glPopMatrix();
}

auto PowerUp::location() const -> Vector2f const & { return location_; }

auto PowerUp::radius() const -> float { return radius_; }

auto PowerUp::type() const -> items::PowerUpType { return type_; }

auto PowerUp::isDead() const -> bool { return lifeTime_ >= totalLifeTime_; }

auto PowerUp::isCollected() const -> bool { return collected_; }
