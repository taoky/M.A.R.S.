/* SpaceBall.cpp

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

#include "Games/SpaceBall.hpp"

#include "Media/music.hpp"
#include "Players/players.hpp"
#include "SpaceObjects/balls.hpp"
#include "System/settings.hpp"
#include "Teams/DMTeam.hpp"
#include "Teams/SBTeam.hpp"
#include "Teams/teams.hpp"

SpaceBall::SpaceBall() : Game(games::gSpaceBall)
{

    settings::C_EnabledWeapons = settings::C_EnabledWeaponsByUser;
    settings::C_EnabledSpecials = settings::C_EnabledSpecialsByUser;

    music::play();

    Team * myTeamL = NULL;
    Team * myTeamR = NULL;

    if (settings::C_playerIteamL)
    {
        myTeamL = new SBTeam(settings::C_playerITeamColor);
        players::addPlayer(myTeamL, controllers::cPlayer1);
    }
    else if (settings::C_playerIteamR)
    {
        myTeamR = new SBTeam(settings::C_playerITeamColor);
        players::addPlayer(myTeamR, controllers::cPlayer1);
    }

    if (settings::C_playerIIteamL)
    {
        if (!myTeamL)
            myTeamL = new SBTeam(settings::C_playerIITeamColor);
        players::addPlayer(myTeamL, controllers::cPlayer2);
    }
    else if (settings::C_playerIIteamR)
    {
        if (!myTeamR)
            myTeamR = new SBTeam(settings::C_playerIITeamColor);
        players::addPlayer(myTeamR, controllers::cPlayer2);
    }

    if (!myTeamR && !myTeamL)
    {
        Color3f rand = Color3f::random();
        myTeamL = new SBTeam(rand.inverted());
        myTeamR = new SBTeam(rand);
    }
    else if (!myTeamL)
    {
        myTeamL = new SBTeam(myTeamR->color().inverted());
    }
    else if (!myTeamR)
    {
        myTeamR = new SBTeam(myTeamL->color().inverted());
    }

    teams::addTeam(myTeamL);
    teams::addTeam(myTeamR);

    for (int i = 0; i < settings::C_botsLeft; ++i)
        players::addPlayer(myTeamL, controllers::cBot);
    for (int i = 0; i < settings::C_botsRight; ++i)
        players::addPlayer(myTeamR, controllers::cBot);

    Home * homeL = spaceObjects::addHome(HOME_LEFT, settings::C_pointLimitSB,
                                         myTeamL->color());
    Home * homeR = spaceObjects::addHome(HOME_RIGHT, settings::C_pointLimitSB,
                                         myTeamR->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    balls::addBall();

    spaceObjects::populateSpace(5.f, 10.f, 4);

    zones::detectTacticalZones();
    zones::createRaster(4, 3);
}

void SpaceBall::draw() const
{
    if (settings::C_drawZones)
        zones::draw();
    Game::draw();
}

void SpaceBall::restart()
{
    Game::restart();

    Home * homeL = spaceObjects::addHome(HOME_LEFT, settings::C_pointLimitSB,
                                         teams::getTeamL()->color());
    Home * homeR = spaceObjects::addHome(HOME_RIGHT, settings::C_pointLimitSB,
                                         teams::getTeamR()->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    balls::addBall();

    spaceObjects::populateSpace(5.f, 10.f, 4);

    zones::detectTacticalZones();
    zones::createRaster(4, 3);
}
