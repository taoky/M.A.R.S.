/* items.cpp

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

#include "Items/items.hpp"

#include <GL/gl.h>
#include <atomic>
#include <cmath>
#include <list>
#include <stdlib.h>
#include <vector>

#include "Items/CannonControl.hpp"
#include "Items/PUFuel.hpp"
#include "Items/PUHealth.hpp"
#include "Items/PUReverse.hpp"
#include "Items/PUShield.hpp"
#include "Items/PUSleep.hpp"
#include "Items/PowerUp.hpp"
#include "Media/texture.hpp"
#include "SpaceObjects/Home.hpp"
#include "SpaceObjects/SpaceObject.hpp"
#include "SpaceObjects/spaceObjects.hpp"
#include "System/Vector2f.hpp"
#include "System/randomizer.hpp"
#include "System/settings.hpp"
#include "System/timer.hpp"
#include "defines.hpp"

extern std::atomic_bool exiting;

namespace items
{

namespace
{
CannonControl * cannonControl_(nullptr);
std::list<std::unique_ptr<PowerUp>> powerUps_;

void spawnPowerUp()
{
    int tries(0);
    bool newPowerUpFits(false);
    Vector2f position(0.f, 0.f);

    while (!newPowerUpFits && ++tries < 500)
    {
        // 100 is min distance between edge and planet
        int randx = rand() % (SPACE_X_RESOLUTION - 2 * (50)) + 50;
        int randy = rand() % (SPACE_Y_RESOLUTION - 2 * (50)) + 50;
        position.x_ = randx;
        position.y_ = randy;

        // check for collisions with other objects
        newPowerUpFits = true;
        for (const auto & it : spaceObjects::getObjects())
            if ((it->location() - position).lengthSquare() <
                std::pow(it->radius() + 50, 2))
            {
                newPowerUpFits = false;
                break;
            }

        if (newPowerUpFits)
            break;
    }
    if (newPowerUpFits)
    {
        if (randomizer::random(0, 4) == 0)
        {
            // rare powerUps
            switch (randomizer::random(0, 1))
            {
            case 0:
                powerUps_.push_back(std::make_unique<PUReverse>(position));
                break;
            case 1:
                powerUps_.push_back(std::make_unique<PUSleep>(position));
                break;
            }
        }
        else
        {
            // common powerUps
            switch (randomizer::random(0, 2))
            {
            case 0:
                powerUps_.push_back(std::make_unique<PUShield>(position));
                break;
            case 1:
                powerUps_.push_back(std::make_unique<PUHealth>(position));
                break;
            case 2:
                powerUps_.push_back(std::make_unique<PUFuel>(position));
                break;
            }
        }
    }
}
} // namespace

void update()
{
    if (cannonControl_)
        cannonControl_->update();

    if (settings::C_powerUpRate > 0)
    {
        static float spawnTimer(1.f);
        spawnTimer -= timer::frameTime();
        if (spawnTimer <= 0.f)
        {
            spawnPowerUp();
            spawnTimer = randomizer::random(0.5f, 1.5f) *
                         ((101 - settings::C_powerUpRate) * 0.23f + 1.5f);
        }
    }

    auto it = powerUps_.begin();
    while (it != powerUps_.end())
    {
        (*it)->update();
        if ((*it)->isDead())
        {
            it->reset();
            it = powerUps_.erase(it);
        }
        else
            ++it;
    }
}

void draw()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::PowerUps));

    if (cannonControl_)
        cannonControl_->draw();

    for (auto & powerUp : powerUps_)
        powerUp->draw();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void addCannonControl()
{
    // temporary list of all homes
    std::vector<Home *> const & homes = spaceObjects::getHomes();

    if (homes.size() >= 2)
    {
        Vector2f midPoint;
        for (auto home : homes)
            midPoint += home->location();
        midPoint /= homes.size();
        cannonControl_ = new CannonControl(midPoint);
    }
    else
        cannonControl_ = new CannonControl(Vector2f());
}

auto getCannonControl() -> CannonControl * { return cannonControl_; }

auto getPowerUps() -> std::list<std::unique_ptr<PowerUp>> const &
{
    return powerUps_;
}

void clear()
{
    if (cannonControl_)
    {
        delete cannonControl_;
        cannonControl_ = nullptr;
    }
    if (!exiting)
        powerUps_.clear();
}
} // namespace items
