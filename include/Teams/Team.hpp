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

#ifndef TEAM_HPP_INCLUDED
#define TEAM_HPP_INCLUDED

#include <map>
#include <vector>

#include "Controllers/controllers.hpp"
#include "System/Color3f.hpp"
#include "Teams/Job.hpp"
#include "Zones/zones.hpp"

class Player;
class Home;
class BotController;
class KeyController;
class SpaceObject;
class TacticalZone;
class Zone;

class Team
{
  public:
    Team(Color3f const & color);
    virtual ~Team(){};

    void update();

    void addMember(Player * player);
    void setHome(Home * home);

    void addJob(Job const & job);

    std::vector<Player *> const & members() const { return members_; }
    Home const * home() const { return home_; }
    Color3f const & color() const { return color_; }
    int points() const { return points_; }
    int victories() const { return victories_; }
    void addStars() const;
    void addPoint() const;
    void subtractPoint() const;
    void addVictory() const;
    void resetPoints() const;

    friend short zones::isInside(Team * checker,
                                 SpaceObject const & toBeChecked);
    friend std::map<float, TacticalZone *> const
    zones::toProtect(Team * checker);

  protected:
    virtual void createJobs() = 0;
    std::vector<BotController *> botControllers_;
    std::vector<KeyController *> keyControllers_;

  private:
    void evaluate();

    float evaluationTimer_;
    std::vector<Player *> members_;
    std::multimap<Job, std::multimap<short, BotController *>> jobMap_;

    Home * home_;
    Color3f color_;
    Zone * homeZone_;
    Zone * teamZone_;
    mutable int victories_;
    mutable int points_;
};

#endif // TEAM_HPP_INCLUDED
