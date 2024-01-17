/* trailEffects.cpp

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

#include "TrailEffects/trailEffects.hpp"

#include <algorithm>
#include <atomic>
#include <memory>
#include <set>
#include <vector>

#include "TrailEffects/FloatingTrail.hpp"
#include "TrailEffects/PersistantTrail.hpp"
#include "TrailEffects/Trail.hpp"

struct Color3f;

extern std::atomic_bool exiting;

namespace trailEffects
{
namespace
{
std::vector<std::unique_ptr<Trail>> trails_;
std::set<SpaceObject *> toBeDetached_;
} // namespace

void update()
{
    auto it = trails_.begin();
    while (it != trails_.end())
    {
        if ((*it)->isDead())
        {
            it->reset();
            it = trails_.erase(it);
        }
        else
        {
            if (toBeDetached_.find((*it)->target()) != toBeDetached_.end())
                (*it)->detach();
            (*it)->update();
            ++it;
        }
    }
    toBeDetached_.clear();
}

void draw()
{
    for (auto & trail : trails_)
        trail->draw();
}

auto attach(SpaceObject * target, float timeStep, float duration, float width,
            Color3f const & color, bool persistant) -> Trail *
{
    Trail * trail;
    if (persistant)
        trail = new PersistantTrail(target, timeStep, duration, width, color);
    else
        trail = new FloatingTrail(target, timeStep, duration, width, color);
    trails_.push_back(std::unique_ptr<Trail>(trail));
    return trail;
}

void detach(SpaceObject * target) { toBeDetached_.insert(target); }

auto count() -> int { return trails_.size(); }

void clear()
{
    if (!exiting)
        trails_.clear();
}
} // namespace trailEffects
