/* GITeam.cpp

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

#include "Teams/GITeam.hpp"

#include "Players/Player.hpp"
#include "SpaceObjects/ships.hpp"
#include "Teams/teams.hpp"

void GITeam::createJobs()
{
    checkEnemies();

    for (int i = 0; i < botControllers_.size(); ++i)
    {
        addJob(Job(Job::jLand, 1));
        addJob(Job(Job::jCharge, 1));
    }
}

void GITeam::checkEnemies()
{
    std::vector<Ship *> ships = ships::getShips();
    bool existAny(false);

    for (std::vector<Ship *>::const_iterator it = ships.begin();
         it != ships.end(); ++it)
        if ((*it)->getOwner()->team() != this && (*it)->attackable())
        {
            existAny = true;
            break;
        }

    if (existAny)
    {
        for (int i = 0; i < botControllers_.size(); ++i)
            addJob(Job(Job::jAttackAny, 90));
    }
    else
    {
        for (int i = 0; i < botControllers_.size(); ++i)
            addJob(Job(Job::jEscape, 3));
    }
}
