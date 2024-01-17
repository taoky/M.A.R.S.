/* Vector2f.hpp

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

#include "System/Vector2f.hpp"

#include <cmath>

#include "System/randomizer.hpp"

Vector2f::Vector2f() : x_(0.0), y_(0.0) {}

Vector2f::Vector2f(Vector2f const & point) = default;

Vector2f::Vector2f(float x, float y) : x_(x), y_(y) {}

auto Vector2f::normalize() const -> Vector2f
{
    float len = length();
    if (len != 0)
        return {x_ / len, y_ / len};
    else
        return {0, 0};
}

auto Vector2f::length() const -> float
{
    return (std::sqrt(x_ * x_ + y_ * y_));
}

auto Vector2f::lengthSquare() const -> float { return x_ * x_ + y_ * y_; }

auto Vector2f::randDir() -> Vector2f const
{
    float random = randomizer::random(0.0f, 2 * M_PI);
    return Vector2f(std::sin(random), std::cos(random));
}

auto Vector2f::randDirLen() -> Vector2f const
{
    return randDir() * randomizer::random(0.0f, 1.0f);
}

auto Vector2f::operator+=(Vector2f const & rhs) -> Vector2f &
{
    x_ += rhs.x_;
    y_ += rhs.y_;
    return *this;
}

auto Vector2f::operator-=(Vector2f const & rhs) -> Vector2f &
{
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    return *this;
}

auto Vector2f::operator*=(double rhs) -> Vector2f &
{
    x_ *= rhs;
    y_ *= rhs;
    return *this;
}

auto Vector2f::operator/=(double rhs) -> Vector2f &
{
    x_ /= rhs;
    y_ /= rhs;
    return *this;
}

auto operator+(Vector2f const & lhs, Vector2f const & rhs) -> Vector2f
{
    return {lhs.x_ + rhs.x_, lhs.y_ + rhs.y_};
}

auto operator-(Vector2f const & lhs, Vector2f const & rhs) -> Vector2f
{
    return {lhs.x_ - rhs.x_, lhs.y_ - rhs.y_};
}

auto operator*(Vector2f const & lhs, Vector2f const & rhs) -> float
{
    return (lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_);
}

auto operator*(Vector2f const & lhs, float rhs) -> Vector2f
{
    return {lhs.x_ * rhs, lhs.y_ * rhs};
}

auto operator*(float const & lhs, Vector2f rhs) -> Vector2f
{
    return rhs * lhs;
}

auto operator/(Vector2f const & lhs, float rhs) -> Vector2f
{
    return {lhs.x_ / rhs, lhs.y_ / rhs};
}

auto operator==(Vector2f const & lhs, Vector2f const & rhs) -> bool
{
    return (lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_);
}

auto operator!=(Vector2f const & lhs, Vector2f const & rhs) -> bool
{
    return (lhs.x_ != rhs.x_ && lhs.y_ != rhs.y_);
}

auto operator<(Vector2f const & lhs, Vector2f const & rhs) -> bool
{
    return lhs.lengthSquare() < rhs.lengthSquare();
}

auto operator>(Vector2f const & lhs, Vector2f const & rhs) -> bool
{
    return lhs.lengthSquare() > rhs.lengthSquare();
}

auto clockWise(Vector2f const & first, Vector2f const & second) -> bool
{
    return (first.x_ * second.y_ - first.y_ * second.x_) < 0;
}

auto operator<<(std::ostream & os, Vector2f const & rhs) -> std::ostream &
{
    return os << "[" << rhs.x_ << ", " << rhs.y_ << "]";
}
