/* DMTeam.hpp

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

#ifndef DMTEAM_HPP_INCLUDED
#define DMTEAM_HPP_INCLUDED

#include <vector>

#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "Teams/Team.hpp"

class DMTeam : public Team
{
  public:
    DMTeam(Color3f const & color = Color3f::random()) : Team(color) {}

  private:
    void createJobs();

    void checkEnemies();
    void checkPowerUps();

    std::vector<Vector2f> powerUpLocations_;
};

#endif // DMTEAM_HPP_INCLUDED
