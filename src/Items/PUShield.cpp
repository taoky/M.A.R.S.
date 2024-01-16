/* PUShield.cpp

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

#include "Items/PUShield.hpp"

#include "Particles/particles.hpp"
#include "Players/Player.hpp"
#include "SpaceObjects/Ship.hpp"

void PUShield::draw() const
{
    if (!collected_)
    {
        PowerUp::draw();
    }
    else
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        for (std::list<Ship *>::const_iterator it = ships_.begin();
             it != ships_.end(); ++it)
        {

            glPushMatrix();
            glLoadIdentity();
            glTranslatef((*it)->location().x_, (*it)->location().y_, 0.f);

            // shield bubble
            glColor4f(1.0f, 0.5f, 0.8f, 0.9f);
            glBegin(GL_QUADS);
            const int posX = 5;
            const int posY = 0;
            glTexCoord2f(posX * 0.15625f, posY * 0.15625f);
            glVertex2f(-35, -35);
            glTexCoord2f(posX * 0.15625f, (posY + 1) * 0.15625f);
            glVertex2f(-35, +35);
            glTexCoord2f((posX + 1) * 0.15625f, (posY + 1) * 0.15625f);
            glVertex2f(+35, +35);
            glTexCoord2f((posX + 1) * 0.15625f, posY * 0.15625f);
            glVertex2f(+35, -35);
            glEnd();

            glPopMatrix();
        }
    }
}

void PUShield::refreshLifeTime()
{
    lifeTime_ = 0.f;
    totalLifeTime_ = 10.f;
    // direction is abused for texture coords
    particles::spawnMultiple(5, particles::pPowerUpCollect, location_,
                             Vector2f(4, 0));
}
