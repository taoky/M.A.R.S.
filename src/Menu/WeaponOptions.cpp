/* ShaderError.cpp

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

#include "Menu/WeaponOptions.hpp"

#include <SFML/System/String.hpp>

#include "Interface/Button.hpp"
#include "Interface/Checkbox.hpp"
#include "Interface/Label.hpp"
#include "Interface/LabeledBox.hpp"
#include "Interface/Line.hpp"
#include "Interface/UiWindow.hpp"
#include "Locales/locales.hpp"
#include "Media/text.hpp"
#include "Menu/menus.hpp"
#include "Specials/specials.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/settings.hpp"
#include "Weapons/weapons.hpp"

UiWindow * WeaponOptions::instance_(nullptr);
bool WeaponOptions::kOk_(false);
bool WeaponOptions::kAFK_(false);
bool WeaponOptions::kBurner_(false);
bool WeaponOptions::kFist_(false);
bool WeaponOptions::kFlubba(false);
bool WeaponOptions::kShotgun_(false);
bool WeaponOptions::kRocket_(false);
bool WeaponOptions::kRofle_(false);
bool WeaponOptions::kH2OMG_(false);
bool WeaponOptions::kShocker_(false);

bool WeaponOptions::kFreeze_(false);
bool WeaponOptions::kHeal_(false);
bool WeaponOptions::kBlast_(false);
bool WeaponOptions::kFireWall_(false);

auto WeaponOptions::get() -> UiWindow *
{
    if (instance_ == nullptr)
    {
        instance_ = new WeaponOptions(350, 300);
        instance_->addWidget(new Button(locales::getLocale(locales::Ok),
                                        nullptr, &kOk_, Vector2f(250, 270), 90,
                                        20));
        instance_->addWidget(new Label(
            locales::getLocale(locales::WeaponOptions), TEXT_ALIGN_LEFT,
            Vector2f(10, 10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));

        instance_->addWidget(new LabeledBox(
            locales::getLocale(locales::Weapons), Vector2f(10, 50), 100, 210));
        instance_->addWidget(
            new LabeledBox(locales::getLocale(locales::Specials),
                           Vector2f(160, 50), 100, 210));

        // weapons
        instance_->addWidget(new Checkbox(new sf::String("AFK-47"), nullptr,
                                          &kAFK_, Vector2f(20, 80), 100));
        instance_->addWidget(new Checkbox(new sf::String("WT-FLAMER"), nullptr,
                                          &kBurner_, Vector2f(20, 100), 100));
        instance_->addWidget(new Checkbox(new sf::String("FIST OF ALI"),
                                          nullptr, &kFist_, Vector2f(20, 120),
                                          100));
        instance_->addWidget(new Checkbox(new sf::String("FLUBBA"), nullptr,
                                          &kFlubba, Vector2f(20, 140), 100));
        instance_->addWidget(new Checkbox(new sf::String("SHOTGUN"), nullptr,
                                          &kShotgun_, Vector2f(20, 160), 100));
        instance_->addWidget(new Checkbox(new sf::String("ROCK'n'LOL"), nullptr,
                                          &kRocket_, Vector2f(20, 180), 100));
        instance_->addWidget(new Checkbox(new sf::String("ROFLE"), nullptr,
                                          &kRofle_, Vector2f(20, 200), 100));
        instance_->addWidget(new Checkbox(new sf::String("H2-OMG"), nullptr,
                                          &kH2OMG_, Vector2f(20, 220), 100));

        instance_->addWidget(new Checkbox(new sf::String("FREEZER"), nullptr,
                                          &kFreeze_, Vector2f(170, 80), 100));
        instance_->addWidget(new Checkbox(new sf::String("HEAL"), nullptr,
                                          &kHeal_, Vector2f(170, 100), 100));
        instance_->addWidget(new Checkbox(new sf::String("BLAST"), nullptr,
                                          &kBlast_, Vector2f(170, 120), 100));
        instance_->addWidget(new Checkbox(new sf::String("FIRE WALL"), nullptr,
                                          &kFireWall_, Vector2f(170, 140),
                                          100));
        instance_->addWidget(new Checkbox(new sf::String("SHOCKER"), nullptr,
                                          &kShocker_, Vector2f(170, 160), 100));
    }
    return instance_;
}

void WeaponOptions::onShow()
{
    kAFK_ = settings::C_EnabledWeaponsByUser & weapons::wAFK47;
    kBurner_ = settings::C_EnabledWeaponsByUser & weapons::wBurner;
    kFist_ = settings::C_EnabledWeaponsByUser & weapons::wFist;
    kFlubba = settings::C_EnabledWeaponsByUser & weapons::wFlubba;
    kShotgun_ = settings::C_EnabledWeaponsByUser & weapons::wShotgun;
    kRocket_ = settings::C_EnabledWeaponsByUser & weapons::wRocketLauncher;
    kRofle_ = settings::C_EnabledWeaponsByUser & weapons::wROFLE;
    kH2OMG_ = settings::C_EnabledWeaponsByUser & weapons::wH2OMG;

    kFreeze_ = settings::C_EnabledSpecialsByUser & specials::sFreeze;
    kHeal_ = settings::C_EnabledSpecialsByUser & specials::sHeal;
    kBlast_ = settings::C_EnabledSpecialsByUser & specials::sBlast;
    kFireWall_ = settings::C_EnabledSpecialsByUser & specials::sFireWall;
    kShocker_ = settings::C_EnabledSpecialsByUser & specials::sShocker;
}

void WeaponOptions::checkWidgets()
{
    if (kOk_)
    {
        kOk_ = false;

        settings::C_EnabledWeaponsByUser =
            (kAFK_ ? weapons::wAFK47 : 0) | (kBurner_ ? weapons::wBurner : 0) |
            (kFist_ ? weapons::wFist : 0) | (kFlubba ? weapons::wFlubba : 0) |
            (kShotgun_ ? weapons::wShotgun : 0) |
            (kRocket_ ? weapons::wRocketLauncher : 0) |
            (kRofle_ ? weapons::wROFLE : 0) | (kH2OMG_ ? weapons::wH2OMG : 0);

        settings::C_EnabledSpecialsByUser =
            (kFreeze_ ? specials::sFreeze : 0) |
            (kHeal_ ? specials::sHeal : 0) | (kBlast_ ? specials::sBlast : 0) |
            (kFireWall_ ? specials::sFireWall : 0) |
            (kShocker_ ? specials::sShocker : 0);

        if (settings::C_EnabledWeaponsByUser == 0)
            settings::C_EnabledWeaponsByUser = weapons::wNoWeapon;

        if (settings::C_EnabledSpecialsByUser == 0)
            settings::C_EnabledSpecialsByUser = specials::sNoSpecial;

        menus::hideWindow();
    }
}

void WeaponOptions::reset()
{
    if (instance_)
        delete instance_;
    instance_ = nullptr;
}
