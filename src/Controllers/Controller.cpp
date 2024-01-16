/* Controller.cpp

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

#include "Controllers/Controller.hpp"

#include "Controllers/BotController.hpp"
#include "Controllers/KeyController.hpp"
#include "Items/items.hpp"
#include "Players/Player.hpp"

Controller::Controller(Player * slave) : type_(slave->type()), slave_(slave) {}

void Controller::slaveUp(int up) const
{
    if (slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep])
        slave_->ship_->up_ = up;
    else
        slave_->ship_->up_ = 0;
}

void Controller::slaveLeft(int left) const
{
    if (!slave_->ship_->weaponChange_ && !slave_->ship_->specialChange_ &&
        slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep])
    {
        if (left && slave_->ship_->right_)
            slave_->ship_->right_ = 0;
        else
            slave_->ship_->left_ = left;
    }
    else
        slave_->ship_->left_ = 0;
}

void Controller::slaveRight(int right) const
{
    if (!slave_->ship_->weaponChange_ && !slave_->ship_->specialChange_ &&
        slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep])
    {
        if (right && slave_->ship_->left_)
            slave_->ship_->left_ = 0;
        else
            slave_->ship_->right_ = right;
    }
    else
        slave_->ship_->right_ = 0;
}

void Controller::slaveFire(int fire) const
{
    if (fire > 50 && !slave_->ship_->docked_ && slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep] &&
        slave_->ship_->frozen_ <= 0.f)
        slave_->ship_->currentWeapon_->fire();
}

void Controller::slaveSpecial(int special) const
{
    if (special > 50 && !slave_->ship_->docked_ && slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep] &&
        slave_->ship_->frozen_ <= 0.f)
        slave_->ship_->currentSpecial_->activate();
}

// single key presses
void Controller::slaveUp() const {}

void Controller::slaveLeft() const
{
    if (slave_->ship_->weaponChange_ && slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep])
        slave_->ship_->currentWeapon_->previous();
    else if (slave_->ship_->specialChange_ && slave_->ship_->visible_ &&
             !slave_->ship_->collectedPowerUps_[items::puSleep])
        slave_->ship_->currentSpecial_->previous();
}

void Controller::slaveRight() const
{
    if (slave_->ship_->weaponChange_ && slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep])
        slave_->ship_->currentWeapon_->next();
    else if (slave_->ship_->specialChange_ && slave_->ship_->visible_ &&
             !slave_->ship_->collectedPowerUps_[items::puSleep])
        slave_->ship_->currentSpecial_->next();
}

void Controller::slaveFire() const
{
    if (slave_->ship_->docked_ && slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep] &&
        slave_->ship_->frozen_ <= 0.f)
    {
        slave_->ship_->weaponChange_ = !slave_->ship_->weaponChange_;
        slave_->ship_->specialChange_ = false;
    }
}

void Controller::slaveSpecial() const
{
    if (slave_->ship_->docked_ && slave_->ship_->visible_ &&
        !slave_->ship_->collectedPowerUps_[items::puSleep] &&
        slave_->ship_->frozen_ <= 0.f)
    {
        slave_->ship_->specialChange_ = !slave_->ship_->specialChange_;
        slave_->ship_->weaponChange_ = false;
    }
}

Ship * Controller::ship() const { return slave_->ship_; }

controllers::ControlType Controller::type() const { return type_; }
