/* decoObjects.cpp

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

#include "DecoObjects/decoObjects.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <atomic>
#include <list>
#include <memory>
#include <vector>

#include "DecoObjects/Bolt.hpp"
#include "DecoObjects/Cannon.hpp"
#include "DecoObjects/DecoObject.hpp"
#include "DecoObjects/Evil.hpp"
#include "DecoObjects/Ice.hpp"
#include "DecoObjects/PlanetSign.hpp"
#include "DecoObjects/ShipHighlight.hpp"
#include "DecoObjects/ShipName.hpp"
#include "DecoObjects/SunHeat.hpp"
#include "Particles/AmmoRocket.hpp"
#include "SpaceObjects/Ball.hpp"
#include "SpaceObjects/Ship.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"

extern std::atomic_bool exiting;

namespace decoObjects
{

namespace
{
Cannon * cannon_(nullptr);
std::vector<std::unique_ptr<DecoObject>> decos_;
std::vector<std::unique_ptr<DecoObject>> heats_;
std::vector<std::unique_ptr<DecoObject>> names_;
std::list<std::unique_ptr<DecoObject>> ices_;
std::list<std::unique_ptr<DecoObject>> bolts_;
} // namespace

void update()
{
    if (cannon_)
        cannon_->update();
}

void draw()
{
    if (cannon_)
        cannon_->draw();
    for (auto & deco : decos_)
        deco->draw();

    for (auto it = ices_.begin(); it != ices_.end(); ++it)
    {
        if ((*it) != nullptr)
            (*it)->draw();
        else
        {
            it = ices_.erase(it);
        }
    }

    for (auto it = bolts_.begin(); it != bolts_.end(); ++it)
    {
        if ((*it) != nullptr)
            (*it)->draw();
        else
        {
            it = bolts_.erase(it);
        }
    }
}

void drawHeat()
{
    for (auto & heat : heats_)
        heat->draw();
}

void drawNames()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (auto & name : names_)
        name->draw();
}

void drawArrow(Vector2f const & from, Vector2f const & to,
               Color3f const & color, float width)
{

    Vector2f direction((to - from).normalize() * width * 0.5f);
    Vector2f normal(direction.y_, -direction.x_);

    glBlendFunc(GL_ONE, GL_ONE);
    glLineWidth(width);

    glBegin(GL_TRIANGLES);
    (color * 0.5f).gl3f();
    glVertex2f(from.x_, from.y_);

    color.gl3f();
    glVertex2f(to.x_ - normal.x_, to.y_ - normal.y_);
    glVertex2f(to.x_ + normal.x_, to.y_ + normal.y_);

    glVertex2f(to.x_ + direction.x_ * 3.f, to.y_ + direction.y_ * 3.f);
    glVertex2f(to.x_ + normal.x_ * 2.f, to.y_ + normal.y_ * 2.f);
    glVertex2f(to.x_ - normal.x_ * 2.f, to.y_ - normal.y_ * 2.f);
    glEnd();
}

void addCannon()
{
    cannon_ = new Cannon();
    decos_.push_back(std::make_unique<Evil>());
}

void addPlanetSign(Planet * planet)
{
    decos_.push_back(std::make_unique<PlanetSign>(planet));
}

void addSunHeat(Sun * sun) { heats_.push_back(std::make_unique<SunHeat>(sun)); }

void addIce(std::shared_ptr<Ship> ship)
{
    ices_.push_back(std::make_unique<Ice<Ship>>(ship));
}

void addIce(std::shared_ptr<Ball> ball)
{
    ices_.push_back(std::make_unique<Ice<Ball>>(ball));
}

void addIce(std::shared_ptr<AmmoRocket> rocket)
{
    ices_.push_back(std::make_unique<Ice<AmmoRocket>>(rocket));
}

void removeIce(DecoObject const * toBeRemoved)
{
    for (auto & ice : ices_)
        if (ice.get() == toBeRemoved)
        {
            ice.reset();
            break;
        }
}

void addBolt(SpaceObject * from, SpaceObject * to, float intensity)
{
    bolts_.push_back(std::make_unique<Bolt>(from, to, intensity));
}

void removeBolt(DecoObject const * toBeRemoved)
{
    for (auto & bolt : bolts_)
        if (bolt.get() == toBeRemoved)
        {
            bolt.reset();
            break;
        }
}

void addName(Ship * ship)
{
    names_.push_back(std::make_unique<ShipName>(ship));
}

void addHighlight(Ship * ship)
{
    decos_.push_back(std::make_unique<ShipHighlight>(ship));
}

void clear()
{
    if (cannon_)
    {
        delete cannon_;
        cannon_ = nullptr;
    }
    if (!exiting)
    {
        decos_.clear();
        heats_.clear();
        names_.clear();
        ices_.clear();
    }
}
} // namespace decoObjects
