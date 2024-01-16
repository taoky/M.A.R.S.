/* BotControllerTeamActions.cpp

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

#include "Controllers/BotController.hpp"

#include "Games/games.hpp"
#include "Items/CannonControl.hpp"
#include "Items/items.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Home.hpp"
#include "SpaceObjects/Ship.hpp"
#include "SpaceObjects/balls.hpp"
#include "SpaceObjects/ships.hpp"
#include "System/settings.hpp"
#include "System/window.hpp"
#include "Teams/Team.hpp"
#include "Teams/teams.hpp"
#include "Zones/RasterZone.hpp"
#include "Zones/TacticalZone.hpp"

void BotController::kickBallToEnemy()
{
    Vector2f ballLocation = balls::getBall()->location_;
    if (balls::getBall()->atStart())
    {
        moveTo(ballLocation, 0.f, false, 0.f);
        ballLocation = calcPath(ballLocation, false);
        if (ballLocation == balls::getBall()->location_)
            shootPoint(ballLocation);
    }
    else
    {
        Vector2f shipLocation = ship()->location();
        Vector2f ballVelocity = balls::getBall()->velocity_;
        Vector2f targetPlanetLocation =
            teams::getEnemy(slave_->team())->home()->location();
        float shipRotation = ship()->rotation_ * M_PI / 180.f;
        Vector2f shipDirection(std::cos(shipRotation), std::sin(shipRotation));
        shipDirection = shipDirection.normalize();
        targetPlanetLocation = calcPath(targetPlanetLocation, false);
        ballLocation = calcPath(ballLocation, false);

        // is ship, ball and target planet in row?
        if (spaceObjects::isOnLine(shipLocation, ballLocation - shipLocation,
                                   targetPlanetLocation, 20.f))
        {
            // if so, is the ship correctly oriented
            if (shipDirection * (ballLocation - shipLocation) > 0.8f)
            {
                // kick ball
                moveTo(targetPlanetLocation, 0.f, false);
                if (ballLocation == balls::getBall()->location_)
                    shootPoint(ballLocation);
            }
            else
                turnTo(ballLocation);
        }
        else
        {
            // move behind the ball
            Vector2f aimPosition =
                ballLocation +
                (ballLocation - targetPlanetLocation).normalize() * 30 +
                ballVelocity * 0.5f;

            if ((balls::getBall()->location_ - shipLocation).lengthSquare() <
                5000.f)
            {
                bool ballIsCloseToPlanet(false);
                for (std::vector<SpaceObject *>::const_iterator it =
                         spaceObjects::getObjects().begin();
                     it != spaceObjects::getObjects().end(); ++it)
                    if ((*it)->type() != spaceObjects::oBlackHole &&
                        ((*it)->location() - aimPosition).lengthSquare() <
                            std::pow((*it)->radius(), 2))
                    {
                        ballIsCloseToPlanet = true;
                        break;
                    }
                // if ball lies on planet surface, shoot it
                if (ballIsCloseToPlanet)
                {
                    shootPoint(balls::getBall()->location_, false);
                    moveTo(balls::getBall()->location_, 0.f, false);
                }
                else
                    moveTo(aimPosition, 0.2f, true);
            }
            else
                moveTo(aimPosition, 0.2f, true);
        }
        shootEnemies();
    }
}

void BotController::kickBallOutHome()
{
    Vector2f shipLocation = ship()->location();
    Vector2f ballLocation = balls::getBall()->location();
    float shipRotation = ship()->rotation_ * M_PI / 180;
    Vector2f shipDirection =
        Vector2f(std::cos(shipRotation), std::sin(shipRotation));
    Vector2f homeLocation = slave_->team()->home()->location();
    shipDirection = shipDirection.normalize();
    Vector2f ballShip = shipLocation - ballLocation;

    // is ship between ball and planet?
    if (spaceObjects::isOnLine(ballLocation, ballShip, homeLocation, 45.f))
    {
        // wait a bit and face ball
        if (ballShip.lengthSquare() > 2500.f)
        {
            if (turnTo(ballLocation))
                shootPoint(ballLocation, false);
        }
        else
        {
            moveTo(shipLocation - ballShip * 2.f, 0, false);
            if (spaceObjects::isOnLine(ballLocation, ballShip, shipLocation,
                                       15.f))
                shootPoint(ballLocation, false);
        }
    }
    else
    {
        // go to planet surface, facing the ball
        Vector2f direction = (ballLocation - homeLocation).normalize();
        direction = direction * (slave_->team()->home()->radius() +
                                 ship()->radius() + 30.f);
        Vector2f location = slave_->team()->home()->location() + direction;
        moveTo(location, 0.2f, true);
    }
}

void BotController::waitForBall()
{
    Vector2f ballLocation = balls::getBall()->location_;
    Vector2f targetPlanetLocation =
        teams::getEnemy(slave_->team())->home()->location();
    targetPlanetLocation = calcPath(targetPlanetLocation, false);
    ballLocation = calcPath(ballLocation, false);

    Vector2f planetBall = (ballLocation - targetPlanetLocation).normalize();
    Vector2f aimPosition = ballLocation + planetBall * 50;
    moveTo(aimPosition, 1.5f, true);

    shootEnemies();
}

void BotController::attackAny()
{
    if (target_ && !target_->attackable())
    {
        aggroTable_[target_] = 0.f;
        target_ = NULL;
    }

    if (target_)
    {
        if ((ship()->location() - target_->location()).lengthSquare() >
            std::pow(ship()->currentWeapon_->minDistance(), 2))
        {
            moveTo(target_->location(), 0.f);
            shootEnemy(target_);
        }
        else
        {
            escape();
            shootEnemy(target_);
        }
    }
    else
    {
        auto const & ships = ships::getShips();
        float maxDistance(FLT_MAX);
        for (auto it = ships.begin(); it != ships.end(); ++it)
        {
            if ((*it)->owner_->team() != slave_->team() && (*it)->attackable())
            {
                float distance =
                    ((*it)->location() - ship()->location_).lengthSquare() *
                    (*it)->getLife();
                if (distance < maxDistance)
                {
                    maxDistance = distance;
                    target_ = it->get();
                }
            }
        }
        if (target_)
            aggroTable_[target_] = 100.f;
    }
}

void BotController::attackTarget()
{
    if (currentJob_.object_)
    {
        moveTo(static_cast<Ship *>(currentJob_.object_)->location(), 0.f);
        shootEnemy(static_cast<Ship *>(currentJob_.object_));
    }
}

void BotController::protectZone()
{
    if (nextRoutePoint_.x_ == FLT_MAX)
    {
        toCover_ = static_cast<TacticalZone *>(currentJob_.object_);
        nextRoutePoint_ = toCover_->getRandomPoint();
    }
    if (moveTo(nextRoutePoint_, 0.5f, false, toCover_->radius_ / 4.f + 10.f))
    {
        nextRoutePoint_.x_ = FLT_MAX;
    }
}

void BotController::escape()
{
    if (nextRoutePoint_.x_ == FLT_MAX)
        nextRoutePoint_ = zones::freeZone()->getRandomPoint();
    if (moveTo(nextRoutePoint_, 0.4f, false, 100.f))
        nextRoutePoint_.x_ = FLT_MAX;
}

void BotController::heal()
{
    if (currentJob_.object_)
    {
        if (moveTo(static_cast<Ship *>(currentJob_.object_)->location(), 0.f,
                   true, 20.f + 30.f * ship()->fragStars_))
            slaveSpecial(100);
    }
}

void BotController::unfreeze()
{
    if (currentJob_.object_)
    {
        target_ = NULL;
        moveTo(static_cast<Ship *>(currentJob_.object_)->location(), 0.f);
        shootEnemy(static_cast<Ship *>(currentJob_.object_));
    }
}

void BotController::getPowerUp()
{
    if (currentJob_.object_)
    {
        moveTo(*static_cast<Vector2f *>(currentJob_.object_), 1.f);
        shootEnemies();
    }
}

void BotController::getControl()
{
    CannonControl * control(items::getCannonControl());
    if (control)
    {
        moveTo(control->location(), 1.f);
    }
}
