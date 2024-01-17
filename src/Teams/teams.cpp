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

#include <algorithm>
#include <atomic>
#include <climits>
#include <iostream>
#include <memory>

#include "Teams/Team.hpp"

extern std::atomic_bool exiting;

namespace teams
{
namespace
{
std::vector<std::unique_ptr<Team>> allTeams_;
}

auto addTeam(Team * newTeam) -> Team *
{
    allTeams_.push_back(std::unique_ptr<Team>(newTeam));
    return newTeam;
}

void assignHomes(Home * home)
{
    if (allTeams_.size() > 0)
        for (auto & allTeam : allTeams_)
            allTeam->setHome(home);
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
    for (auto & allTeam : allTeams_)
        allTeam->update();
}

auto getTeamL() -> Team const * { return allTeams_[0].get(); }

auto getTeamR() -> Team const * { return allTeams_[1].get(); }

auto getAllTeams() -> std::vector<std::unique_ptr<Team>> const &
{
    return allTeams_;
}

auto getEnemy(Team const * checker) -> Team const *
{
    return checker == allTeams_[0].get() ? allTeams_[1].get()
                                         : allTeams_[0].get();
}

auto getFirstPoints() -> int
{
    int highest(INT_MIN);
    for (auto & allTeam : allTeams_)
        if (allTeam->points() > highest)
            highest = allTeam->points();
    return highest;
}

auto getSecondPoints() -> int
{
    int first(INT_MIN);
    int second(INT_MIN);
    for (auto & allTeam : allTeams_)
        if (allTeam->points() >= first)
        {
            second = first;
            first = allTeam->points();
        }
        else if (allTeam->points() > second)
        {
            second = allTeam->points();
        }
    if (second == INT_MIN)
        second = 0;
    return second;
}

void resetTeamPoints()
{
    for (auto & allTeam : allTeams_)
        allTeam->resetPoints();
}

void clear()
{
    if (!exiting)
        allTeams_.clear();
}
} // namespace teams
