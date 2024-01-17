/* controllers.cpp

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

#include <algorithm>
#include <vector>

#include "Controllers/BotController.hpp"
#include "Controllers/KeyController.hpp"
#include "Controllers/controllers.hpp"
#include "Players/Player.hpp"
#include "System/settings.hpp"

struct Key;

namespace controllers
{
namespace
{
KeyController * keyControllers1_(nullptr);
KeyController * keyControllers2_(nullptr);
std::vector<BotController *> botControllers_;
} // namespace

void update()
{
    if (keyControllers1_ != nullptr &&
        (settings::C_playerIteamL | settings::C_playerIteamR))
        keyControllers1_->update();
    if (keyControllers2_ != nullptr &&
        (settings::C_playerIIteamL | settings::C_playerIIteamR))
        keyControllers2_->update();

    for (auto & botController : botControllers_)
        botController->update();
}

void singleKeyEvent(Key const & keyCode)
{
    if (keyControllers1_ != nullptr &&
        (settings::C_playerIteamL | settings::C_playerIteamR))
        keyControllers1_->update(keyCode);
    if (keyControllers2_ != nullptr &&
        (settings::C_playerIIteamL | settings::C_playerIIteamR))
        keyControllers2_->update(keyCode);
}

void draw()
{
    for (auto & botController : botControllers_)
        botController->draw();
}

auto addBotController(Player * slave, float strength) -> BotController *
{
    auto * bot = new BotController(slave, strength);
    botControllers_.push_back(bot);
    return bot;
}

auto addKeyController(Player * slave) -> KeyController *
{
    switch (slave->type())
    {
    case cPlayer1:
        keyControllers1_ = new KeyController(slave);
        return keyControllers1_;
    default:
        keyControllers2_ = new KeyController(slave);
        return keyControllers2_;
    }
}

void resetBots()
{
    for (auto & botController : botControllers_)
        botController->reset();
}

void clear()
{
    delete keyControllers1_;
    delete keyControllers2_;
    keyControllers1_ = nullptr;
    keyControllers2_ = nullptr;

    for (auto & botController : botControllers_)
        delete botController;
    botControllers_.clear();
}
} // namespace controllers
