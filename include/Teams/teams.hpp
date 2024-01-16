/* teams.hpp

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

#ifndef TEAMS_HPP_INCLUDED
#define TEAMS_HPP_INCLUDED

#include "Controllers/controllers.hpp"
#include "System/Color3f.hpp"

class Team;
class Home;
class Player;

namespace teams
{
Team * addTeam(Team * newTeam);

void assignHomes(Home * home);
void assignHomes(Home * homeL, Home * homeR);

void update();

void assignZones();

void createShips();

void resetTeamPoints();
void clear();

Team const * getTeamL();
Team const * getTeamR();
std::vector<Team *> const & getAllTeams();
Team const * getEnemy(Team const * checker);

int getFirstPoints();
int getSecondPoints();
} // namespace teams

#endif // TEAMS_HPP_INCLUDED
