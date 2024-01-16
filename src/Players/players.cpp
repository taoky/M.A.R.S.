/* players.cpp

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

#include "Players/players.hpp"

#include "Players/BotPlayer.hpp"
#include "Players/LocalPlayer.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Home.hpp"
#include "System/generateName.hpp"
#include "System/settings.hpp"
#include "Teams/Team.hpp"
#include "defines.hpp"

#include <climits>
#include <memory>

extern std::atomic_bool exiting;

namespace players
{
namespace
{
std::vector<std::shared_ptr<Player>> allPlayers_;
std::shared_ptr<Player> playerI_;
std::shared_ptr<Player> playerII_;
bool initialized_(false);

void initLocalPlayers()
{
    playerI_ = std::make_shared<LocalPlayer>(controllers::cPlayer1);
    playerII_ = std::make_shared<LocalPlayer>(controllers::cPlayer2);
    initialized_ = true;
}
} // namespace

void addPlayer(Team * team, controllers::ControlType type,
               Color3f const & color)
{
    switch (type)
    {
    case controllers::cPlayer1:
        if (!initialized_)
            initLocalPlayers();
        team->addMember(playerI_.get());
        allPlayers_.push_back(playerI_);
        break;
    case controllers::cPlayer2:
        if (!initialized_)
            initLocalPlayers();
        team->addMember(playerII_.get());
        allPlayers_.push_back(playerII_);
        break;
    default:
        Player * bot =
            new BotPlayer(generateName::bot(((long)team % INT_MAX) / 97), color,
                          rand() % SHIP_GRAPHICS_COUNT + 1);
        team->addMember(bot);
        allPlayers_.push_back(std::unique_ptr<Player>(bot));
    }
}

void createShips()
{
    // temporary list of all homes
    std::vector<Home *> const & homes = spaceObjects::getHomes();
    // temporary lists of all inhabitants of all
    for (std::vector<Home *>::const_iterator homeIt = homes.begin();
         homeIt != homes.end(); ++homeIt)
    {
        std::vector<Player *> inhabitants;
        for (auto playIt = allPlayers_.begin(); playIt != allPlayers_.end();
             ++playIt)
        {
            if ((*playIt)->team()->home() == (*homeIt))
                inhabitants.push_back(playIt->get());
        }
        (*homeIt)->createShips(inhabitants);
    }
}

Player const * getPlayerI() { return playerI_.get(); }

Player const * getPlayerII() { return playerII_.get(); }

void resetPlayerPoints()
{
    for (auto it = allPlayers_.begin();
         it != allPlayers_.end(); ++it)
        (*it)->resetPoints();
}

void clear()
{
    if (!exiting)
        allPlayers_.clear();
    initialized_ = false;
}
} // namespace players
