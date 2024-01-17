/* balls.cpp

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

#include "SpaceObjects/balls.hpp"

#include <memory>
#include <vector>

#include "SpaceObjects/Home.hpp"
#include "SpaceObjects/spaceObjects.hpp"

namespace balls
{
namespace
{
std::shared_ptr<Ball> ball_(nullptr);
}

void addBall(Vector2f const & location)
{
    // temporary list of all homes
    std::vector<Home *> const & homes = spaceObjects::getHomes();

    if (homes.size() >= 2 && location == Vector2f(0, 0))
    {
        Vector2f midPoint;
        for (auto home : homes)
            midPoint += home->location();
        midPoint /= homes.size();
        ball_ = std::make_shared<Ball>(midPoint);
    }
    else
        ball_ = std::make_shared<Ball>(location);
}

void update()
{
    if (ball_)
        ball_->update();
}

void draw()
{
    if (ball_)
        ball_->draw();
}

auto getBall() -> std::shared_ptr<Ball> { return ball_; }

void clear()
{
    if (ball_)
    {
        ball_.reset();
    }
}
} // namespace balls
