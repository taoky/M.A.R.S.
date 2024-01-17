/* font.cpp

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

#include "Media/font.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Locales/Locale.hpp"
#include "Locales/locales.hpp"
#include "System/settings.hpp"

namespace font
{
namespace
{
std::map<int, std::unique_ptr<sf::Font>> fonts_;
}

auto getFont(int languageID) -> sf::Font *
{
    auto it = fonts_.find(languageID);
    if (it == fonts_.end())
    {
        // load it from file and...
        auto * font = new sf::Font();
        font->loadFromFile(settings::C_dataPath + "fonts/" +
                           locales::getLocales()[languageID].font_);
        fonts_.insert(std::make_pair(languageID, font));
        // ... return it afterwards
        return font;
    }
    else
    {
        return it->second.get();
    }
}
} // namespace font
