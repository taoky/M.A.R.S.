/* generateName.cpp

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

#include "System/generateName.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <bits/chrono.h>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <string>

#include "Locales/locales.hpp"
#include "Media/file.hpp"
#include "System/settings.hpp"

namespace generateName
{

namespace
{
std::vector<std::list<std::pair<sf::String, int>>> botNames_;
std::list<sf::String> gameNames_;
std::vector<sf::String> shipNames_;
bool initialized_(false);

void loadBotNames()
{
    std::vector<sf::String> lines;
    if (file::load(settings::C_dataPath + "botnames.txt", lines))
    {
        std::list<std::pair<sf::String, int>> newList;
        for (auto & line : lines)
        {
            if (line.toAnsiString()[0] == '[')
            {
                if (newList.size() > 0)
                {
                    botNames_.push_back(newList);
                    newList.clear();
                }
            }
            else
            {
                std::stringstream strengthStream(
                    std::string(line.toAnsiString(), line.getSize() - 3));
                int strength;
                strengthStream >> strength;
                line.erase(line.getSize() - 3, 3);
                while (line[line.getSize() - 1] == ' ' ||
                       line[line.getSize() - 1] == '\t')
                    line.erase(line.getSize() - 1);
                newList.emplace_back(line, strength);
            }
        }
    }
    if (botNames_.size() == 0)
    {
        std::cout << "Failed to open botnames.txt! Reverting to some boring "
                     "default names...\n";
        std::list<std::pair<sf::String, int>> defaultnames;
        defaultnames.emplace_back("Ernst", 90);
        defaultnames.emplace_back("Holger", 50);
        defaultnames.emplace_back("Jimmy", 78);
        defaultnames.emplace_back("Arnold", 100);
        defaultnames.emplace_back("Ute", 92);
        defaultnames.emplace_back("John", 94);
        defaultnames.emplace_back("Matt", 20);
        defaultnames.emplace_back("Gudrun", 67);
        defaultnames.emplace_back("Ringo", 82);
        defaultnames.emplace_back("Elvis", 98);
        defaultnames.emplace_back("Bertold", 100);
        defaultnames.emplace_back("Karl", 87);
        defaultnames.emplace_back("Joe", 41);
        defaultnames.emplace_back("Tom", 95);
        defaultnames.emplace_back("Hilde", 84);
        defaultnames.emplace_back("Herbert", 85);
        defaultnames.emplace_back("Lars", 99);
        defaultnames.emplace_back("Jeremy", 36);
        defaultnames.emplace_back("Thomas", 57);
        defaultnames.emplace_back("Jenny", 99);
        defaultnames.emplace_back("James", 66);
        defaultnames.emplace_back("Erwin", 92);
        defaultnames.emplace_back("Thompson", 90);
        defaultnames.emplace_back("August", 30);
        defaultnames.emplace_back("Thorben", 77);
        defaultnames.emplace_back("Tony", 80);
        botNames_.push_back(defaultnames);
    }
}

void loadShipNames()
{
    if (!file::load(settings::C_dataPath + "shipnames.txt", shipNames_))
        std::cout << "No Botnames found! Using boring numbers instead...\n";
}

void init_()
{
    // init random engine
    auto rng = std::default_random_engine{};
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());

    // load list of bot names from file
    loadBotNames();

    // load list of ship names from file
    loadShipNames();

    // init game names
    gameNames_.emplace_back("Retarded Shooter");
    gameNames_.emplace_back("Random Shooter");
    gameNames_.emplace_back("Ridiculous Shooter");
    gameNames_.emplace_back("Rapid Shooter");
    gameNames_.emplace_back("Rough Shooter");
    gameNames_.emplace_back("Rigged Shooter");
    gameNames_.emplace_back("Rude Shooter");
    gameNames_.emplace_back("Retro-Shooter");

    // shuffle both lists
    std::vector<std::pair<sf::String, int>> temp;
    for (auto & botName : botNames_)
    {
        for (auto it = botName.begin(); it != botName.end(); ++it)
            temp.push_back(*it);
        std::shuffle(temp.begin(), temp.end(), rng);
        botName.clear();
        for (auto & it : temp)
            botName.push_back(it);
        temp.clear();
    }

    std::vector<sf::String> temp2;
    for (auto & gameName : gameNames_)
        temp2.push_back(gameName);
    std::shuffle(temp2.begin(), temp2.end(), rng);
    gameNames_.clear();
    for (auto & it : temp2)
        gameNames_.push_back(it);

    initialized_ = true;
}
} // namespace

auto bot(int randomNumber) -> std::pair<sf::String, int> const &
{
    if (!initialized_)
        init_();
    int group = randomNumber % botNames_.size();
    botNames_[group].push_front(botNames_[group].back());
    botNames_[group].pop_back();
    return *botNames_[group].begin();
}

auto game() -> sf::String const &
{
    if (!initialized_)
        init_();
    gameNames_.push_front(gameNames_.back());
    gameNames_.pop_back();
    return *gameNames_.begin();
}

auto shipNames() -> std::vector<sf::String> const & { return shipNames_; }

auto key(Key const & key) -> sf::String const
{
    sf::String result("Unknown Key");
    switch (key.type_)
    {
    case Key::kKeyBoard:
    {
        sf::Keyboard::Key keyCode(key.code_.keyBoard_);
        // "normal" character
        if (static_cast<int>(keyCode) >= 0 && static_cast<int>(keyCode) <= 25)
            result = static_cast<char>(keyCode + 65);

        // numbers
        if (static_cast<int>(keyCode) >= 26 && static_cast<int>(keyCode) <= 35)
            result = static_cast<char>(keyCode + 22);

        else if (keyCode == sf::Keyboard::Add)
            result = *locales::getLocale(locales::Add);
        else if (keyCode == sf::Keyboard::BackSlash)
            result = *locales::getLocale(locales::BackSlash);
        else if (keyCode == sf::Keyboard::Comma)
            result = *locales::getLocale(locales::Comma);
        else if (keyCode == sf::Keyboard::Dash)
            result = *locales::getLocale(locales::Dash);
        else if (keyCode == sf::Keyboard::Delete)
            result = *locales::getLocale(locales::Delete);
        else if (keyCode == sf::Keyboard::Divide)
            result = *locales::getLocale(locales::Divide);
        else if (keyCode == sf::Keyboard::Down)
            result = *locales::getLocale(locales::Down);
        else if (keyCode == sf::Keyboard::End)
            result = *locales::getLocale(locales::End);
        else if (keyCode == sf::Keyboard::Equal)
            result = *locales::getLocale(locales::Equal);
        else if (keyCode == sf::Keyboard::F1)
            result = "F1";
        else if (keyCode == sf::Keyboard::F2)
            result = "F2";
        else if (keyCode == sf::Keyboard::F3)
            result = "F3";
        else if (keyCode == sf::Keyboard::F4)
            result = "F4";
        else if (keyCode == sf::Keyboard::F5)
            result = "F5";
        else if (keyCode == sf::Keyboard::F6)
            result = "F6";
        else if (keyCode == sf::Keyboard::F7)
            result = "F7";
        else if (keyCode == sf::Keyboard::F8)
            result = "F8";
        else if (keyCode == sf::Keyboard::F9)
            result = "F9";
        else if (keyCode == sf::Keyboard::F10)
            result = "F10";
        else if (keyCode == sf::Keyboard::F11)
            result = "F11";
        else if (keyCode == sf::Keyboard::F12)
            result = "F12";
        else if (keyCode == sf::Keyboard::Home)
            result = *locales::getLocale(locales::Home);
        else if (keyCode == sf::Keyboard::Insert)
            result = *locales::getLocale(locales::Insert);
        else if (keyCode == sf::Keyboard::LAlt)
            result = *locales::getLocale(locales::LeftAlt);
        else if (keyCode == sf::Keyboard::LBracket)
            result = *locales::getLocale(locales::LeftBracket);
        else if (keyCode == sf::Keyboard::LControl)
            result = *locales::getLocale(locales::LeftControl);
        else if (keyCode == sf::Keyboard::Left)
            result = *locales::getLocale(locales::Left);
        else if (keyCode == sf::Keyboard::LShift)
            result = *locales::getLocale(locales::LeftShift);
        else if (keyCode == sf::Keyboard::LSystem)
            result = *locales::getLocale(locales::LeftSuper);
        else if (keyCode == sf::Keyboard::Menu)
            result = *locales::getLocale(locales::Menu);
        else if (keyCode == sf::Keyboard::Multiply)
            result = *locales::getLocale(locales::Multiply);
        else if (keyCode == sf::Keyboard::Numpad0)
            result = "Num 0";
        else if (keyCode == sf::Keyboard::Numpad1)
            result = "Num 1";
        else if (keyCode == sf::Keyboard::Numpad2)
            result = "Num 2";
        else if (keyCode == sf::Keyboard::Numpad3)
            result = "Num 3";
        else if (keyCode == sf::Keyboard::Numpad4)
            result = "Num 4";
        else if (keyCode == sf::Keyboard::Numpad5)
            result = "Num 5";
        else if (keyCode == sf::Keyboard::Numpad6)
            result = "Num 6";
        else if (keyCode == sf::Keyboard::Numpad7)
            result = "Num 7";
        else if (keyCode == sf::Keyboard::Numpad8)
            result = "Num 8";
        else if (keyCode == sf::Keyboard::Numpad9)
            result = "Num 9";
        else if (keyCode == sf::Keyboard::PageDown)
            result = *locales::getLocale(locales::PageDown);
        else if (keyCode == sf::Keyboard::PageUp)
            result = *locales::getLocale(locales::PageUp);
        else if (keyCode == sf::Keyboard::Pause)
            result = *locales::getLocale(locales::Pause);
        else if (keyCode == sf::Keyboard::Period)
            result = *locales::getLocale(locales::Period);
        else if (keyCode == sf::Keyboard::Quote)
            result = *locales::getLocale(locales::Quote);
        else if (keyCode == sf::Keyboard::RAlt)
            result = *locales::getLocale(locales::RightAlt);
        else if (keyCode == sf::Keyboard::RBracket)
            result = *locales::getLocale(locales::RightBracket);
        else if (keyCode == sf::Keyboard::RControl)
            result = *locales::getLocale(locales::RightControl);
        else if (keyCode == sf::Keyboard::Return)
            result = *locales::getLocale(locales::Return);
        else if (keyCode == sf::Keyboard::Right)
            result = *locales::getLocale(locales::Right);
        else if (keyCode == sf::Keyboard::RShift)
            result = *locales::getLocale(locales::RightShift);
        else if (keyCode == sf::Keyboard::RSystem)
            result = *locales::getLocale(locales::RightSuper);
        else if (keyCode == sf::Keyboard::SemiColon)
            result = *locales::getLocale(locales::SemiColon);
        else if (keyCode == sf::Keyboard::Slash)
            result = *locales::getLocale(locales::Slash);
        else if (keyCode == sf::Keyboard::Space)
            result = *locales::getLocale(locales::Space);
        else if (keyCode == sf::Keyboard::Subtract)
            result = *locales::getLocale(locales::Subtract);
        else if (keyCode == sf::Keyboard::Tilde)
            result = *locales::getLocale(locales::Tilde);
        else if (keyCode == sf::Keyboard::Tab)
            result = *locales::getLocale(locales::Tab);
        else if (keyCode == sf::Keyboard::Up)
            result = *locales::getLocale(locales::Up);

        break;
    }

    case Key::kJoyButton:
    {
        unsigned int keyCode(key.code_.joyButton_);
        std::stringstream tmp;
        tmp << key.joyID_;
        result = *locales::getLocale(locales::JoyStick) + tmp.str() + " - ";
        if (keyCode == 0)
            result += *locales::getLocale(locales::JoyButtonA);
        else if (keyCode == 1)
            result += *locales::getLocale(locales::JoyButtonB);
        else if (keyCode == 2)
            result += *locales::getLocale(locales::JoyButtonX);
        else if (keyCode == 3)
            result += *locales::getLocale(locales::JoyButtonY);
        else if (keyCode == 4)
            result += *locales::getLocale(locales::JoyButtonLB);
        else if (keyCode == 5)
            result += *locales::getLocale(locales::JoyButtonRB);
        else if (keyCode == 6)
            result += *locales::getLocale(locales::JoyButtonStart);
        else if (keyCode == 10)
            result += *locales::getLocale(locales::JoyButtonBack);
        else
        {
            std::stringstream tmptmp;
            tmptmp << keyCode;
            result +=
                *locales::getLocale(locales::JoyButton) + " " + tmptmp.str();
        }
        break;
    }

    case Key::kJoyAxis:
    {
        Key::AxisType keyCode(key.code_.joyAxis_);
        std::stringstream tmp;
        tmp << key.joyID_;
        result = *locales::getLocale(locales::JoyStick) + tmp.str() + " - ";
        if (keyCode == Key::aLT)
            result += *locales::getLocale(locales::JoyAxisLT);
        else if (keyCode == Key::aRT)
            result += *locales::getLocale(locales::JoyAxisRT);
        else if (keyCode == Key::aARup)
            result += *locales::getLocale(locales::JoyA2up);
        else if (keyCode == Key::aARdown)
            result += *locales::getLocale(locales::JoyA2down);
        else if (keyCode == Key::aARleft)
            result += *locales::getLocale(locales::JoyA2left);
        else if (keyCode == Key::aARright)
            result += *locales::getLocale(locales::JoyA2right);
        else if (keyCode == Key::aALup)
            result += *locales::getLocale(locales::JoyA1up);
        else if (keyCode == Key::aALdown)
            result += *locales::getLocale(locales::JoyA1down);
        else if (keyCode == Key::aALleft)
            result += *locales::getLocale(locales::JoyA1left);
        else if (keyCode == Key::aALright)
            result += *locales::getLocale(locales::JoyA1right);
        else if (keyCode == Key::aPOVup)
            result += *locales::getLocale(locales::JoyPOVup);
        else if (keyCode == Key::aPOVdown)
            result += *locales::getLocale(locales::JoyPOVdown);
        else if (keyCode == Key::aPOVleft)
            result += *locales::getLocale(locales::JoyPOVleft);
        else if (keyCode == Key::aPOVright)
            result += *locales::getLocale(locales::JoyPOVright);
        break;
    }
    }
    return result;
}
} // namespace generateName
