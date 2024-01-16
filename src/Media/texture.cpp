/* texture.cpp

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

#include "Media/texture.hpp"

#include "System/randomizer.hpp"
#include "System/settings.hpp"

#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <memory>

namespace texture
{

namespace
{
std::vector<std::unique_ptr<GLuint>> textures_(COUNT);

void loadTexture_(TextureType type, std::string fileName)
{
    textures_[type] = std::make_unique<GLuint>();
    sf::Image img;
    img.loadFromFile(fileName);

    // convert sf::Image to GLuint
    const sf::Uint8 * ptr = img.getPixelsPtr();
    glGenTextures(1, textures_[type].get());
    glBindTexture(GL_TEXTURE_2D, *textures_[type]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, ptr);
    if (type == Interface || type == Stars1_large || type == Stars2_large ||
        type == Stars1_medium || type == Stars2_medium)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace

GLuint const & getTexture(TextureType type)
{
    // check if texture is already loaded
    if (textures_[type] != nullptr)
    {
        return *textures_[type];
    }
    else
    {
        // load it from file and...
        switch (type)
        {
        case Home:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/spaceObjects/planetPlayer.png");
            break;
        case Planet1:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet1.png");
            break;
        case Planet2:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet2.png");
            break;
        case Planet3:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet3.png");
            break;
        case Planet4:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet4.png");
            break;
        case Planet5:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet5.png");
            break;
        case Planet6:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet6.png");
            break;
        case Planet7:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet7.png");
            break;
        case Planet8:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet8.png");
            break;
        case Planet9:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/planet9.png");
            break;
        case Planet10:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/spaceObjects/planet10.png");
            break;
        case Planet11:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/spaceObjects/planet11.png");
            break;
        case Planet12:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/spaceObjects/planet12.png");
            break;
        case Planet13:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/spaceObjects/planet13.png");
            break;
        case Planet14:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/spaceObjects/planet14.png");
            break;
        case Planet15:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/spaceObjects/planet15.png");
            break;
        case Sun1:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/sun1.png");
            break;
        case SunHeat:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/sunHeat.png");
            break;
        case Ice:
            loadTexture_(type, settings::C_dataPath + "tex/ice.png");
            break;
        case Bolt:
            loadTexture_(type, settings::C_dataPath + "tex/bolt.png");
            break;
        case Cracks:
            loadTexture_(type, settings::C_dataPath + "tex/cracks.png");
            break;
        case Hole1:
            loadTexture_(type,
                         settings::C_dataPath + "tex/spaceObjects/hole1.png");
            break;
        case Ball:
            loadTexture_(type, settings::C_dataPath + "tex/ball.png");
            break;
        case Particles:
            loadTexture_(type, settings::C_dataPath + "tex/particles.png");
            break;
        case PowerUps:
            loadTexture_(type, settings::C_dataPath + "tex/powerUps.png");
            break;
        case Ships:
            loadTexture_(type, settings::C_dataPath + "tex/ships.png");
            break;
        case Weapons:
            loadTexture_(type, settings::C_dataPath + "tex/weapons.png");
            break;
        case MotherShip:
            loadTexture_(type, settings::C_dataPath + "tex/motherShip.png");
            break;
        case MotherShipPlayerLayer:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/motherShipPlayerLayer.png");
            break;
        case Sign1:
            loadTexture_(type, settings::C_dataPath + "tex/signs/sign1.png");
            break;
        case Sign2:
            loadTexture_(type, settings::C_dataPath + "tex/signs/sign2.png");
            break;
        case Logo1:
            loadTexture_(type, settings::C_dataPath + "tex/logos/logo1.png");
            break;
        case Cannon:
            loadTexture_(type, settings::C_dataPath + "tex/cannon.png");
            break;
        case CannonSocket:
            loadTexture_(type, settings::C_dataPath + "tex/cannonSocket.png");
            break;
        case Stars1_large:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/background/starfield_1_large.jpg");
            break;
        case Stars1_medium:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/background/starfield_1_medium.jpg");
            break;
        case Stars2_large:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/background/starfield_2_large.jpg");
            break;
        case Stars2_medium:
            loadTexture_(type, settings::C_dataPath +
                                   "tex/background/starfield_2_medium.jpg");
            break;
        case Star:
            loadTexture_(type,
                         settings::C_dataPath + "tex/background/star.png");
            break;
        case EnergyBars:
            loadTexture_(type,
                         settings::C_dataPath + "tex/interface/energyBars.png");
            break;
        case Widgets:
            loadTexture_(type,
                         settings::C_dataPath + "tex/interface/widgets.png");
            break;
        case Interface:
            loadTexture_(type,
                         settings::C_dataPath + "tex/interface/interface.png");
            break;
        case CountDown0:
            loadTexture_(type, settings::C_dataPath + "tex/countdown/0.png");
            break;
        case CountDown1:
            loadTexture_(type, settings::C_dataPath + "tex/countdown/1.png");
            break;
        case CountDown2:
            loadTexture_(type, settings::C_dataPath + "tex/countdown/2.png");
            break;
        case CountDown3:
            loadTexture_(type, settings::C_dataPath + "tex/countdown/3.png");
            break;
        case CountDown4:
            loadTexture_(type, settings::C_dataPath + "tex/countdown/4.png");
            break;
        case ShockWave:
            loadTexture_(type, settings::C_dataPath + "tex/shockwave.png");
            break;
        case COUNT:
            std::cout << "COUNT is not a valid Texturetype..." << std::endl;
        }
        // ... return it afterwards
        return *textures_[type];
    }
}

TextureType const randomPlanet()
{
    int randomInt = randomizer::random(1, 15);
    switch (randomInt)
    {
    case 1:
        return Planet1;
    case 2:
        return Planet2;
    case 3:
        return Planet3;
    case 4:
        return Planet4;
    case 5:
        return Planet5;
    case 6:
        return Planet6;
    case 7:
        return Planet7;
    case 8:
        return Planet8;
    case 9:
        return Planet9;
    case 10:
        return Planet10;
    case 11:
        return Planet11;
    case 12:
        return Planet12;
    case 13:
        return Planet13;
    case 14:
        return Planet14;
    default:
        return Planet15;
    }
}

Color3f const getMudColor(TextureType type)
{
    switch (type)
    {
    case Planet1:
        return Color3f(1.0f, 1.0f, 0.3f);
    case Planet2:
        return Color3f(1.0f, 0.3f, 0.9f);
    case Planet3:
        return Color3f(1.0f, 0.6f, 1.0f);
    case Planet4:
        return Color3f(0.6f, 0.3f, 1.0f);
    case Planet5:
        return Color3f(1.0f, 0.5f, 0.8f);
    case Planet6:
        return Color3f(0.0f, 0.8f, 1.0f);
    case Planet7:
        return Color3f(0.3f, 0.3f, 0.3f);
    case Planet8:
        return Color3f(0.2f, 1.0f, 0.2f);
    case Planet9:
        return Color3f(1.0f, 0.0f, 0.0f);
    case Planet10:
        return Color3f(1.0f, 1.0f, 0.5f);
    case Planet11:
        return Color3f(0.7f, 0.7f, 0.2f);
    case Planet12:
        return Color3f(1.0f, 0.6f, 0.8f);
    case Planet13:
        return Color3f(0.5f, 1.0f, 1.0f);
    case Planet14:
        return Color3f(1.0f, 1.0f, 0.0f);
    case Planet15:
        return Color3f(1.0f, 0.7f, 0.0f);
    default:
        return Color3f(1.f, 1.f, 1.f);
    }
}
} // namespace texture
