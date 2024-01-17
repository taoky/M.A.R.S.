/* GraveItation.cpp

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

#include "Games/GraveItation.hpp"

#include "Controllers/controllers.hpp"
#include "Games/games.hpp"
#include "Media/music.hpp"
#include "Players/players.hpp"
#include "SpaceObjects/spaceObjects.hpp"
#include "Specials/specials.hpp"
#include "System/Color3f.hpp"
#include "System/settings.hpp"
#include "Teams/GITeam.hpp"
#include "Teams/Team.hpp"
#include "Teams/teams.hpp"
#include "Weapons/weapons.hpp"
#include "Zones/zones.hpp"

class Home;

GraveItation::GraveItation() : Game(games::gGraveItation)
{

    settings::C_EnabledWeapons = weapons::wInsta;
    settings::C_EnabledSpecials = specials::sNoSpecial;

    music::play();

    Team * myTeamL = nullptr;
    Team * myTeamR = nullptr;

    if (settings::C_playerIteamL)
    {
        myTeamL = new GITeam(settings::C_playerITeamColor);
        players::addPlayer(myTeamL, controllers::cPlayer1);
    }
    else if (settings::C_playerIteamR)
    {
        myTeamR = new GITeam(settings::C_playerITeamColor);
        players::addPlayer(myTeamR, controllers::cPlayer1);
    }

    if (settings::C_playerIIteamL)
    {
        if (!myTeamL)
            myTeamL = new GITeam(settings::C_playerIITeamColor);
        players::addPlayer(myTeamL, controllers::cPlayer2);
    }
    else if (settings::C_playerIIteamR)
    {
        if (!myTeamR)
            myTeamR = new GITeam(settings::C_playerIITeamColor);
        players::addPlayer(myTeamR, controllers::cPlayer2);
    }

    if (!myTeamR && !myTeamL)
    {
        Color3f rand = Color3f::random();
        myTeamL = new GITeam(rand.inverted());
        myTeamR = new GITeam(rand);
    }
    else if (!myTeamL)
    {
        myTeamL = new GITeam(myTeamR->color().inverted());
    }
    else if (!myTeamR)
    {
        myTeamR = new GITeam(myTeamL->color().inverted());
    }

    teams::addTeam(myTeamL);
    teams::addTeam(myTeamR);

    for (int i = 0; i < settings::C_botsLeft; ++i)
        players::addPlayer(myTeamL, controllers::cBot);
    for (int i = 0; i < settings::C_botsRight; ++i)
        players::addPlayer(myTeamR, controllers::cBot);

    Home * homeL = spaceObjects::addHome(HOME_LEFT, 100, myTeamL->color());
    Home * homeR = spaceObjects::addHome(HOME_RIGHT, 100, myTeamR->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    spaceObjects::populateSpace(25.f, 5.f, 2);
    zones::createRaster(4, 3);
}

void GraveItation::draw() const
{
    if (settings::C_drawZones)
        zones::draw();
    Game::draw();
}

void GraveItation::restart()
{
    Game::restart();

    Home * homeL =
        spaceObjects::addHome(HOME_LEFT, 100, teams::getTeamL()->color());
    Home * homeR =
        spaceObjects::addHome(HOME_RIGHT, 100, teams::getTeamR()->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    spaceObjects::populateSpace(25.f, 5.f, 2);
    zones::createRaster(4, 3);
}
