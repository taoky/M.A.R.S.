/* Game.cpp

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

#include "Games/Game.hpp"

#include <memory>
#include <vector>

#include "Controllers/controllers.hpp"
#include "DecoObjects/decoObjects.hpp"
#include "Hud/hud.hpp"
#include "Items/items.hpp"
#include "Media/announcer.hpp"
#include "Menu/menus.hpp"
#include "Particles/particles.hpp"
#include "Players/players.hpp"
#include "Shaders/postFX.hpp"
#include "SpaceObjects/balls.hpp"
#include "SpaceObjects/physics.hpp"
#include "SpaceObjects/ships.hpp"
#include "SpaceObjects/spaceObjects.hpp"
#include "SpaceObjects/stars.hpp"
#include "System/settings.hpp"
#include "System/timer.hpp"
#include "Teams/Team.hpp"
#include "Teams/teams.hpp"
#include "TrailEffects/trailEffects.hpp"
#include "Zones/zones.hpp"

Game::Game(games::GameType const & type)
    : type_(type), startTime_(timer::totalTime()), ended_(false)
{
    switch (type_)
    {
    case games::gSpaceBall:
        pointLimit_ = settings::C_pointLimitSB;
        break;
    case games::gCannonKeep:
        pointLimit_ = settings::C_pointLimitCK;
        break;
    case games::gDeathMatch:
        pointLimit_ = settings::C_pointLimitDM;
        break;
    case games::gMenu:
        pointLimit_ = 9999999;
        break;
    default:
        pointLimit_ = settings::C_pointLimitTDM;
    }

    hud::init();
    stars::init();
}

Game::~Game()
{
    // Careful!
    // as programs will automatically destruct static objects on exit
    // we will do bad things on uninitialized objects

    items::clear();
    ships::clear();
    balls::clear();
    physics::clear();
    particles::clear();
    spaceObjects::clear();
    controllers::clear();
    teams::clear();
    players::clear();
    zones::clear();
    decoObjects::clear();
    trailEffects::clear();
    timer::resetSlowMotion();
}

void Game::update()
{
    announcer::update();
    hud::update();
    if ((!menus::visible()) || (type_ == games::gMenu))
    {
        spaceObjects::update();
        particles::update();
        items::update();
        postFX::update();
        trailEffects::update();

        if (teams::getFirstPoints() >= pointLimit_)
        {
            if (!ended_)
            {
                Team * best(nullptr);
                int most(0);
                for (const auto & it : teams::getAllTeams())
                    if (most < it->points())
                    {
                        best = it.get();
                        most = it->points();
                    }
                if (best)
                    best->addVictory();
                ended_ = true;
            }
            hud::displayStats();

            if (type_ != games::gDeathMatch)
                hud::displayPoints();
        }
        else
        {
            decoObjects::update();
            ships::update();
            balls::update();
            teams::update();
            controllers::update();
            zones::update();
        }
    }
    else
        startTime_ += timer::frameTime();
}

void Game::draw() const
{
    if (settings::C_StarField)
        particles::drawStars();
    particles::drawLower();
    trailEffects::draw();
    controllers::draw();
    ships::draw();
    balls::draw();
    spaceObjects::draw();
    particles::drawHigher();
    decoObjects::draw();
    items::draw();
}

void Game::restart()
{
    items::clear();
    ships::clear();
    balls::clear();
    physics::clear();
    particles::clear();
    spaceObjects::clear();
    zones::clear();
    decoObjects::clear();
    trailEffects::clear();
    teams::resetTeamPoints();
    players::resetPlayerPoints();
    startTime_ = timer::totalTime();
    controllers::resetBots();
    stars::init();
    hud::displayStats(false);
    hud::displayPoints(false);
    timer::resetSlowMotion();
    ended_ = false;
}

auto Game::type() const -> games::GameType { return type_; }

auto Game::elapsedTime() const -> float
{
    return timer::totalTime() - startTime_;
}

auto Game::ended() const -> bool { return ended_; }
