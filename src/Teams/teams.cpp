/* teams.cpp

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

#include "Teams/teams.hpp"

#include "SpaceObjects/Home.hpp"
#include "Teams/Team.hpp"

#include <climits>
#include <memory>

extern std::atomic_bool exiting;

namespace teams
{
namespace
{
std::vector<std::unique_ptr<Team>> allTeams_;
}

Team * addTeam(Team * newTeam)
{
    allTeams_.push_back(std::unique_ptr<Team>(newTeam));
    return newTeam;
}

void assignHomes(Home * home)
{
    if (allTeams_.size() > 0)
        for (auto it = allTeams_.begin();
             it != allTeams_.end(); ++it)
            (*it)->setHome(home);
    else
        std::cout << "Cant assign Home Planet! No Teams are specified!\n";
}

void assignHomes(Home * homeL, Home * homeR)
{
    if (allTeams_.size() == 2)
    {
        allTeams_[0]->setHome(homeL);
        allTeams_[1]->setHome(homeR);
    }
    else
        std::cout << "Cant assign two Home Planets! There have to be exactly "
                     "two Teams specified!\n";
}

void update()
{
    for (auto it = allTeams_.begin();
         it != allTeams_.end(); ++it)
        (*it)->update();
}

Team const * getTeamL() { return allTeams_[0].get(); }

Team const * getTeamR() { return allTeams_[1].get(); }

std::vector<std::unique_ptr<Team>> const & getAllTeams() { return allTeams_; }

Team const * getEnemy(Team const * checker)
{
    return checker == allTeams_[0].get() ? allTeams_[1].get() : allTeams_[0].get();
}

int getFirstPoints()
{
    int highest(INT_MIN);
    for (auto it = allTeams_.begin();
         it != allTeams_.end(); ++it)
        if ((*it)->points() > highest)
            highest = (*it)->points();
    return highest;
}

int getSecondPoints()
{
    int first(INT_MIN);
    int second(INT_MIN);
    for (auto it = allTeams_.begin();
         it != allTeams_.end(); ++it)
        if ((*it)->points() >= first)
        {
            second = first;
            first = (*it)->points();
        }
        else if ((*it)->points() > second)
        {
            second = (*it)->points();
        }
    if (second == INT_MIN)
        second = 0;
    return second;
}

void resetTeamPoints()
{
    for (auto it = allTeams_.begin();
         it != allTeams_.end(); ++it)
        (*it)->resetPoints();
}

void clear()
{
    if (!exiting)
        allTeams_.clear();
}
} // namespace teams
