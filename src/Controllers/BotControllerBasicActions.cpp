/* BotControllerBasicActions.cpp

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

#include "Controllers/BotController.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Home.hpp"
#include "SpaceObjects/Ship.hpp"
#include "System/Vector2f.hpp"
#include "System/randomizer.hpp"
#include "Teams/Team.hpp"

void BotController::charge()
{
    Vector2f direction =
        ship()->location() - slave_->team()->home()->location();
    turnTo(direction + ship()->location());
    slaveFire(0);
    slaveUp(0);
    switchWeapon();
    if (weaponChangeTimer_ > 0.5f)
        switchSpecial();
}

void BotController::land()
{
    moveTo(slave_->team()->home()->location(), 50.f, true, 0.f, true);
}

void BotController::switchWeapon()
{
    if (weaponChangeTimer_ <= 0.f)
    {
        slaveFire();
        slaveLeft();
        slaveFire();

        if (randomizer::random(0, 2) == 0)
            weaponChangeTimer_ = randomizer::random(20.f, 60.f);
        else
            weaponChangeTimer_ = randomizer::random(0.25f, 0.35f);
    }
}

void BotController::switchSpecial()
{
    if (specialChangeTimer_ <= 0.f)
    {
        slaveSpecial();
        slaveLeft();
        slaveSpecial();

        if (randomizer::random(0, 2) == 0)
            specialChangeTimer_ = randomizer::random(20.f, 60.f);
        else
            specialChangeTimer_ = randomizer::random(0.25f, 0.35f);
    }
}
