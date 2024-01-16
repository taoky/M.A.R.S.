/* players.hpp

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

#ifndef PLAYERS_HPP_INCLUDED
#define PLAYERS_HPP_INCLUDED

#include "Controllers/controllers.hpp"
#include "System/Color3f.hpp"

class Team;
class Home;
class Player;

namespace players
{
void addPlayer(Team * team, controllers::ControlType type,
               Color3f const & = Color3f::random());

void createShips();

void resetPlayerPoints();
void clear();

Player const * getPlayerI();
Player const * getPlayerII();
} // namespace players

#endif // PLAYERS_HPP_INCLUDED
