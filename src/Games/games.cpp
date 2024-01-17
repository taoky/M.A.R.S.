/* games.cpp

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

#include "Games/games.hpp"

#include <GL/gl.h>
#include <iostream>
#include <memory>

#include "Games/CannonKeep.hpp"
#include "Games/DeathMatch.hpp"
#include "Games/Game.hpp"
#include "Games/GraveItation.hpp"
#include "Games/MenuGame.hpp"
#include "Games/Rally.hpp"
#include "Games/SpaceBall.hpp"
#include "Games/TeamDeathMatch.hpp"
#include "Games/Tutorial.hpp"
#include "Hud/hud.hpp"
#include "Media/music.hpp"
#include "Menu/menus.hpp"
#include "System/Vector2f.hpp"
#include "System/timer.hpp"
#include "System/window.hpp"

namespace games
{

namespace
{
std::unique_ptr<Game> currentGame_(nullptr);
bool restart_(false);
bool newGame_(true);
GameType newGameType_(gNoGame);
bool fadeOut_(false);
bool fadeIn_(false);
float fadeFactor_(1.f);

void fade()
{
    if (fadeIn_)
    {
        fadeFactor_ -= timer::frameTime() * 2.f;
        if (fadeFactor_ <= 0.f)
        {
            fadeFactor_ = 0.f;
            fadeIn_ = false;
        }
    }
    else if (fadeOut_)
    {
        fadeFactor_ += timer::frameTime() * 3.f;
        if (fadeFactor_ >= 1.f)
        {
            fadeFactor_ = 1.f;
            fadeOut_ = false;
        }
    }

    Vector2f viewPort = window::getViewPort();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(0.0, 0.0, 0.0, fadeFactor_);
    glVertex2f(0.f, 0.f);
    glVertex2f(0.f, viewPort.y_);
    glVertex2f(viewPort.x_, viewPort.y_);
    glVertex2f(viewPort.x_, 0.f);
    glEnd();
}

} // namespace

void update()
{
    music::update();
    if (!fadeIn_ && !fadeOut_)
        currentGame_->update();
    if (restart_ && !fadeOut_)
        restart();
    if (newGame_ && !fadeOut_)
        start(newGameType_);
}

void draw()
{
    window::startDrawSpace();

    currentGame_->draw();

    window::startDrawHUD();

    hud::draw();
    menus::draw();
    hud::drawMessages();

    if (fadeIn_ || fadeOut_)
        fade();
}

void start(GameType const & type)
{
    if (!newGame_)
    {
        newGameType_ = type;
        newGame_ = true;
        fadeOut_ = true;
    }
    else
    {
        if (currentGame_)
        {
            currentGame_.reset();
        }

        switch (type)
        {
        case gMenu:
            currentGame_ = std::make_unique<MenuGame>();
            break;
        case gSpaceBall:
            currentGame_ = std::make_unique<SpaceBall>();
            break;
        case gDeathMatch:
            currentGame_ = std::make_unique<DeathMatch>();
            break;
        case gTeamDeathMatch:
            currentGame_ = std::make_unique<TeamDeathMatch>();
            break;
        case gCannonKeep:
            currentGame_ = std::make_unique<CannonKeep>();
            break;
        case gGraveItation:
            currentGame_ = std::make_unique<GraveItation>();
            break;
        case gTutorial:
            currentGame_ = std::make_unique<Tutorial>();
            break;
        case gRally:
            currentGame_ = std::make_unique<Rally>();
            break;
        default:
            std::cerr << "games::start(): Unknown game type!" << std::endl;
            break;
        }

        newGame_ = false;
        fadeIn_ = true;
    }
}

void restart()
{
    if (!restart_)
    {
        restart_ = true;
        fadeOut_ = true;
    }
    else
    {
        currentGame_->restart();
        restart_ = false;
        fadeIn_ = true;
    }
}

auto active() -> bool { return (!fadeIn_ && !fadeOut_); }

auto ended() -> bool { return currentGame_->ended(); }

auto type() -> GameType
{
    if (currentGame_)
        return currentGame_->type();
    else
        return gNoGame;
}

auto elapsedTime() -> float
{
    if (currentGame_)
        return currentGame_->elapsedTime();
    else
        return 0.f;
}
} // namespace games
