/* Team.cpp

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

#include "Teams/Team.hpp"

#include <algorithm>
#include <iterator>
#include <set>
#include <utility>

#include "Controllers/BotController.hpp"
#include "Controllers/KeyController.hpp"
#include "Controllers/controllers.hpp"
#include "Games/games.hpp"
#include "Players/BotPlayer.hpp"
#include "Players/LocalPlayer.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Home.hpp"
#include "SpaceObjects/Ship.hpp"
#include "System/timer.hpp"
#include "Zones/zones.hpp"

Team::Team(Color3f const & color)
    : evaluationTimer_(0.f), home_(nullptr), color_(color), homeZone_(nullptr),
      teamZone_(nullptr), victories_(0), points_(0)
{
}

void Team::update()
{
    evaluationTimer_ += timer::frameTime();
    if (evaluationTimer_ > 0.2f)
    {
        evaluationTimer_ = 0.f;
        evaluate();
    }
}

void Team::addMember(Player * player)
{
    members_.push_back(player);
    player->team_ = this;

    if (player->type() == controllers::cBot)
    {
        auto * bot = dynamic_cast<BotPlayer *>(player);
        if (bot)
            botControllers_.push_back(bot->controller_);
    }
    else
    {
        auto * localPlayer = dynamic_cast<LocalPlayer *>(player);
        if (localPlayer)
            keyControllers_.push_back(localPlayer->controller_);
    }
}

void Team::setHome(Home * home)
{
    home_ = home;
    homeZone_ = zones::addHomeZone(home->location());
    teamZone_ = zones::addTeamZone(home->location());
}

void Team::addJob(Job const & job)
{
    jobMap_.insert(
        std::make_pair(job, std::multimap<short, BotController *>()));
}

void Team::resetPoints() const { points_ = 0; }

void Team::addPoint() const { ++points_; }

void Team::subtractPoint() const { --points_; }

void Team::addVictory() const { ++victories_; }

void Team::addStars() const
{
    for (auto member : members_)
        ++member->ship()->fragStars_;
}

void Team::evaluate()
{
    jobMap_.clear();

    if (botControllers_.size() > 0 && games::elapsedTime() > 4.f)
    {
        createJobs();

        for (auto & botController : botControllers_)
            botController->evaluate();

        for (auto & keyController : keyControllers_)
            keyController->evaluate();

        for (auto & botController : botControllers_)
            botController->applyForJob(jobMap_);

        std::set<BotController *> unemployedBots(botControllers_.begin(),
                                                 botControllers_.end());
        bool botsLeft(true);

        while (botsLeft)
        {
            std::multimap<Job, std::multimap<short, BotController *>>::iterator
                mostWantedJob;
            BotController * mostWanting = nullptr;
            short highestDesire(0);

            for (auto it = jobMap_.begin(); it != jobMap_.end(); ++it)
            {
                auto bot = it->second.rbegin();
                while (bot != it->second.rend() &&
                       unemployedBots.find(bot->second) == unemployedBots.end())
                    ++bot;
                if (bot != it->second.rend() && bot->first > highestDesire)
                {
                    highestDesire = bot->first;
                    mostWantedJob = it;
                    mostWanting = bot->second;
                }
            }

            if (mostWanting)
            {
                unemployedBots.erase(mostWanting);
                mostWanting->assignJob(mostWantedJob->first);
                jobMap_.erase(mostWantedJob);
            }
            else
            {
                botsLeft = false;
            }
        }

        for (auto unemployedBot : unemployedBots)
            unemployedBot->assignJob(Job(Job::jLand, 10));
    }
}
