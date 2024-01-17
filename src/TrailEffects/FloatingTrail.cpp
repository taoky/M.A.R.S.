/* FloatingTrail.cpp

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

#include "TrailEffects/FloatingTrail.hpp"

#include <GL/gl.h>

#include "Media/texture.hpp"
#include "SpaceObjects/SpaceObject.hpp"
#include "System/timer.hpp"

FloatingTrail::FloatingTrail(SpaceObject * target, float timeStep,
                             float duration, float width, Color3f const & color)
    : Trail(target), points_(duration / timeStep), frontIndex_(0), length_(0),
      timer_(0.f), timeStep_(timeStep), width_(width), color_(color)
{
    points_[frontIndex_++] = target_->location();
    ++length_;
}

void FloatingTrail::update()
{
    timer_ -= timer::frameTime();
    if (timer_ < 0.f)
    {
        timer_ = timeStep_;
        if (target_)
        {
            points_[frontIndex_++] = target_->location();
            frontIndex_ %= points_.size();
            if (length_ < points_.size())
                ++length_;
        }
        else
        {
            --length_;
        }
    }
}

void FloatingTrail::draw() const
{
    if (length_ > 1)
    {
        const int posX = 1;
        const int posY = 1;

        Vector2f toNext;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUAD_STRIP);

        for (int i = length_; i > 0; --i)
        {
            if (!target_ && i == 1)
                color_.gl4f(0.f);
            else
                color_.gl4f(static_cast<float>(length_ - i) / length_);

            int index((frontIndex_ - i + points_.size()) % points_.size());

            if (i > 1)
            {
                int nextIndex((frontIndex_ - i + 1 + points_.size()) %
                              points_.size());
                toNext =
                    (points_[nextIndex] - points_[index]).normalize() * width_;
            }

            glTexCoord2f((posX + 0.5) * 0.125f, posY * 0.125f);
            glVertex2f(points_[index].x_ + toNext.y_,
                       points_[index].y_ - toNext.x_);
            glTexCoord2f((posX + 0.5) * 0.125f, (posY + 1) * 0.125f);
            glVertex2f(points_[index].x_ - toNext.y_,
                       points_[index].y_ + toNext.x_);
        }

        if (target_)
        {
            color_.gl4f(0);
            glTexCoord2f((posX + frontIndex_ % 2) * 0.125f, posY * 0.125f);
            glVertex2f(target_->location().x_ + toNext.y_,
                       target_->location().y_ - toNext.x_);
            glTexCoord2f((posX + frontIndex_ % 2) * 0.125f,
                         (posY + 1) * 0.125f);
            glVertex2f(target_->location().x_ - toNext.y_,
                       target_->location().y_ + toNext.x_);
        }

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

auto FloatingTrail::isDead() const -> bool { return length_ <= 0; }
