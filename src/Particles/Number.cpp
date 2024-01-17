/* Number.cpp

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

#include "Particles/Number.hpp"

#include <SFML/System/String.hpp>
#include <sstream>
#include <string>

#include "Media/text.hpp"
#include "SpaceObjects/spaceObjects.hpp"
#include "System/Vector2f.hpp"
#include "System/timer.hpp"

class Player;

std::list<std::shared_ptr<Number>> Number::activeParticles_;

Number::Number(Vector2f const & location, Vector2f const & direction,
               Vector2f const & velocity, Color3f const & color,
               Player * damageSource)
    : Particle<Number>(spaceObjects::oNumber, location, 4, 0, 1.f), alpha_(1.f)
{

    velocity_ = velocity;

    value_ = direction.x_;

    radius_ = direction.y_;
    if (radius_ > 50.f)
        radius_ = 50.f;

    if (value_ > 0)
        color_ = Color3f(0.4f, 1.0f, 0.4f);
    else
        color_ = Color3f(1.0f, 0.4f, 0.4f);
}

void Number::update()
{
    float time = timer::frameTime();

    alpha_ = 1.f - 1.f / totalLifeTime_ * lifeTime_;

    location_ = location_ + velocity_ * time;
    velocity_ = velocity_ + velocity_ * (-5.f) * time;

    lifeTime_ += time;
}

void Number::draw() const
{
    std::stringstream sstr;
    sstr << value_;
    text::drawMobileSpaceText(sf::String(sstr.str()), location_, radius_,
                              TEXT_ALIGN_CENTER, color_, alpha_);
}
