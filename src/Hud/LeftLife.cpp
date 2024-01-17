/* LeftLife.cpp

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

#include "Hud/LeftLife.hpp"

#include <GL/gl.h>
#include <SFML/System/String.hpp>
#include <sstream>
#include <string>

#include "Media/text.hpp"
#include "Media/texture.hpp"
#include "Players/Player.hpp"
#include "Players/players.hpp"
#include "SpaceObjects/Ship.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/settings.hpp"
#include "System/window.hpp"
#include "Teams/Team.hpp"
#include "Teams/teams.hpp"

void LeftLife::draw() const
{
    text::drawFooText();

    Player const * player(nullptr);

    if ((settings::C_playerIteamL && settings::C_playerIIteamL) |
        (settings::C_playerIteamR && settings::C_playerIIteamR))
        player = players::getPlayerII();
    else if (settings::C_playerIteamL && settings::C_playerIIteamR)
        player = players::getPlayerI();
    else if (settings::C_playerIIteamL && settings::C_playerIteamR)
        player = players::getPlayerII();
    else if (settings::C_playerIteamL)
        player = players::getPlayerI();
    else if (settings::C_playerIIteamL)
        player = players::getPlayerII();

    if (player)
    {
        Color3f color = player->color();
        color.v(1.f);
        color.s(0.3f);
        ;
        Vector2f const & port = window::getViewPort();

        std::stringstream sstr;
        int first(teams::getFirstPoints());
        if (player->team()->points() == first)
        {
            int second(teams::getSecondPoints());
            sstr << player->name().toAnsiString() << " ("
                 << player->team()->points() << "/ +" << first - second << ")";
        }
        else
            sstr << player->name().toAnsiString() << " ("
                 << player->team()->points() << "/ -"
                 << first - player->team()->points() << ")";

        text::drawScreenText(sf::String(sstr.str()), Vector2f(11, port.y_ - 74),
                             18.f, TEXT_ALIGN_LEFT, Color3f(0.f, 0.f, 0.f));
        text::drawScreenText(sf::String(sstr.str()), Vector2f(10, port.y_ - 75),
                             18.f, TEXT_ALIGN_LEFT, color);

        float life = player->ship()->getLife();
        float fuel = player->ship()->getFuel();

        // player life bar
        glBegin(GL_QUADS);
        Color3f lifeColor(1.f, 0.f, 0.f);
        lifeColor.h(lifeColor.h() + life);
        lifeColor.gl4f(0.9f);
        glVertex2i(15, port.y_ - 32);
        glVertex2i(15, port.y_ - 47);
        glVertex2i(15 + life * 1.64f, port.y_ - 47);
        glVertex2i(15 + life * 1.64f, port.y_ - 32);
        glEnd();

        // draw caps
        if (life > 0)
        {
            if (life == 100.f)
            {
                glBegin(GL_QUADS);
                glVertex2i(179, port.y_ - 47);
                glVertex2i(184, port.y_ - 43);
                glVertex2i(184, port.y_ - 37);
                glVertex2i(179, port.y_ - 32);
                glEnd();
            }

            glBegin(GL_QUADS);
            glVertex2i(15, port.y_ - 47);
            glVertex2i(10, port.y_ - 43);
            glVertex2i(10, port.y_ - 37);
            glVertex2i(15, port.y_ - 32);
            glEnd();
        }

        // player fuel bar
        glBegin(GL_QUADS);
        glColor4f(1.f, 1.f, 0.f, 0.9f);
        glVertex2i(15, port.y_ - 8);
        glVertex2i(15, port.y_ - 23);
        glVertex2i(15 + fuel * 1.64f, port.y_ - 23);
        glVertex2i(15 + fuel * 1.64f, port.y_ - 8);
        glEnd();

        // draw caps
        if (fuel > 0)
        {
            if (fuel == 100.f)
            {
                glBegin(GL_QUADS);
                glVertex2i(179, port.y_ - 23);
                glVertex2i(184, port.y_ - 18);
                glVertex2i(184, port.y_ - 13);
                glVertex2i(179, port.y_ - 8);
                glEnd();
            }

            glBegin(GL_QUADS);
            glVertex2i(15, port.y_ - 23);
            glVertex2i(10, port.y_ - 18);
            glVertex2i(10, port.y_ - 13);
            glVertex2i(15, port.y_ - 8);
            glEnd();
        }

        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::EnergyBars));
        glBegin(GL_QUADS);
        glColor3f(1.0, 1.0, 1.0);
        glTexCoord2f(0, 0);
        glVertex2f(0, port.y_ - 64);
        glTexCoord2f(0, 1);
        glVertex2f(0, port.y_);
        glTexCoord2f(0.75f, 1);
        glVertex2f(192, port.y_);
        glTexCoord2f(0.75f, 0);
        glVertex2f(192, port.y_ - 64);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        /*
        // draw glow
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        // fuel
        glBegin(GL_QUADS);
            glColor4f(1.f,1.f,0.f,0.5f);
            glVertex2i(16, port.y_-10);
            glVertex2i(16, port.y_-27);
            glVertex2i(16 + fuel*1.92f, port.y_-27);
            glVertex2i(16 + fuel*1.92f, port.y_-10);

            glColor4f(1.f,1.f,0.f,0.5f);
            glVertex2i(16, port.y_-27);
            glVertex2i(16 + fuel*1.92f, port.y_-27);
            glColor4f(1.f,1.f,0.f,0.0f);
            glVertex2i(16 + fuel*1.92f, port.y_-37);
            glVertex2i(16, port.y_-37);

            glColor4f(1.f,1.f,0.f,0.5f);
            glVertex2i(16, port.y_-10);
            glVertex2i(16 + fuel*1.92f, port.y_-10);
            glColor4f(1.f,1.f,0.f,0.0f);
            glVertex2i(16 + fuel*1.92f, port.y_);
            glVertex2i(16, port.y_);
        glEnd();
        */
    }
}
