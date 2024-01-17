/* ships.cpp

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

#include "SpaceObjects/ships.hpp"

#include <GL/gl.h>
#include <atomic>
#include <memory>
#include <vector>

#include "Media/texture.hpp"
#include "SpaceObjects/Ship.hpp"

struct Vector2f;

extern std::atomic_bool exiting;

namespace ships
{
namespace
{
std::vector<std::shared_ptr<Ship>> shipList_;
}

void addShip(Vector2f const & location, float rotation, Player * owner)
{
    shipList_.push_back(std::make_unique<Ship>(location, rotation, owner));
}

void update()
{
    for (auto & it : shipList_)
        it->update();
}

void draw()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Weapons));

    for (auto & it : shipList_)
        it->drawWeapon();

    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Ships));

    for (auto & it : shipList_)
        it->draw();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto getShips() -> std::vector<std::shared_ptr<Ship>> const &
{
    return shipList_;
}

void clear()
{
    if (!exiting)
        shipList_.clear();
}
} // namespace ships
