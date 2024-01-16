/* Ball.hpp

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

#ifndef BALL_HPP_INCLUDED
#define BALL_HPP_INCLUDED

#include "Players/Player.hpp"
#include "SpaceObjects/MobileSpaceObject.hpp"

class Ball : public MobileSpaceObject
{
  public:
    Ball(Vector2f const & location);

    void update();
    void draw() const;

    void onCollision(SpaceObject * with, Vector2f const & location,
                     Vector2f const & direction, Vector2f const & velocity);

    void onShockWave(Player * source, float intensity);

    bool atStart() const { return sticky_; }
    bool isVisible() const { return visible_; }
    float heatAmount() const { return heatTimer_ * 5.f; }

    Player * lastShooter() const { return lastShooter_; }
    void resetShooter() { lastShooter_ = NULL; }

    friend class BotController;
    friend class Freezer;
    friend class Shocker;
    template <typename Object> friend class Ice;

  private:
    void explode();
    void respawn();
    float rotation_;
    float rotateSpeed_;
    float frozen_;
    bool sticky_, visible_;

    Vector2f respawnLocation_;
    float respawnRotation_;

    float heatTimer_, smokeTimer_, respawnTimer_;

    Player * lastShooter_;
};

#endif // BALL_HPP_INCLUDED
