/* Weapon.cpp

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

#include "Weapons/Weapon.hpp"

#include "SpaceObjects/Ship.hpp"
#include <memory>

Weapon::Weapon(weapons::WeaponType type, Ship * parent, sf::String name)
    : parent_(parent), timer_(0), name_(name), type_(type)
{
}

void Weapon::next()
{
    parent_->currentWeapon_ = std::unique_ptr<Weapon>(weapons::createNext(type_, parent_));
    // delete this;
}

void Weapon::previous()
{
    parent_->currentWeapon_ = std::unique_ptr<Weapon>(weapons::createPrev(type_, parent_));
    // delete this;
}
