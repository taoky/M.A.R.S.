/* NewGameMenu.cpp

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

#include "Menu/NewGameMenu.hpp"

#include <SFML/System/String.hpp>

#include "Games/games.hpp"
#include "Interface/Button.hpp"
#include "Interface/Checkbox.hpp"
#include "Interface/Label.hpp"
#include "Interface/LabeledBox.hpp"
#include "Interface/RadioButton.hpp"
#include "Interface/RadioGroup.hpp"
#include "Interface/Slider.hpp"
#include "Interface/Tab.hpp"
#include "Interface/TabList.hpp"
#include "Interface/UiWindow.hpp"
#include "Locales/locales.hpp"
#include "Media/music.hpp"
#include "Media/text.hpp"
#include "Menu/InfoCK.hpp"
#include "Menu/InfoDM.hpp"
#include "Menu/InfoGIP.hpp"
#include "Menu/InfoSB.hpp"
#include "Menu/InfoTDM.hpp"
#include "Menu/WeaponOptions.hpp"
#include "Menu/menus.hpp"
#include "System/Color3f.hpp"
#include "System/Vector2f.hpp"
#include "System/settings.hpp"

UiWindow * NewGameMenu::instance_(nullptr);
bool NewGameMenu::kStart_(false);
bool NewGameMenu::kInfo_(false);
bool NewGameMenu::kCancel_(false);
bool NewGameMenu::playerI_(false);
bool NewGameMenu::playerII_(false);
bool NewGameMenu::tSB_(false);
bool NewGameMenu::tDM_(false);
bool NewGameMenu::tTDM_(false);
bool NewGameMenu::tCK_(false);
bool NewGameMenu::tGI_(false);
bool NewGameMenu::tRLY_(false);
bool NewGameMenu::kWeaponOptions_(false);
Tab * NewGameMenu::tabSpaceBall_(nullptr);
Tab * NewGameMenu::tabDeathMatch_(nullptr);
Tab * NewGameMenu::tabTeamDeathMatch_(nullptr);
Tab * NewGameMenu::tabCannonKeep_(nullptr);
Tab * NewGameMenu::tabGraveItation_(nullptr);
Tab * NewGameMenu::tabRally_(nullptr);

auto NewGameMenu::get() -> UiWindow *
{
    if (instance_ == nullptr)
    {
        instance_ = new NewGameMenu(580, 335);
        instance_->addWidget(new Button(locales::getLocale(locales::Start),
                                        nullptr, &kStart_, Vector2f(475, 300),
                                        90, 20));

        instance_->addWidget(new Label(
            locales::getLocale(locales::StartLocalGame), TEXT_ALIGN_LEFT,
            Vector2f(10, 10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));

        auto * tabList = new TabList(Vector2f(10, 55), 560, 220);
        tabSpaceBall_ =
            new Tab(locales::getStaticString("SpaceBall"), 90, &tSB_);
        tabDeathMatch_ =
            new Tab(locales::getStaticString("DeathMatch"), 100, &tDM_);
        tabTeamDeathMatch_ =
            new Tab(locales::getStaticString("Team-DeathMatch"), 120, &tTDM_);
        tabCannonKeep_ =
            new Tab(locales::getStaticString("CannonKeep"), 100, &tCK_);
        tabGraveItation_ =
            new Tab(locales::getStaticString("Grave-Itation Pit"), 125, &tGI_);
        tabRally_ = new Tab(locales::getStaticString("Rally"), 80, &tRLY_);

        tabSpaceBall_->addWidget(new LabeledBox(
            locales::getLocale(locales::LeftTeam), Vector2f(10, 30), 250, 80));
        tabSpaceBall_->addWidget(
            new LabeledBox(locales::getLocale(locales::RightTeam),
                           Vector2f(300, 30), 250, 80));
        auto * player1Group = new RadioGroup();
        player1Group->addRadioButton(new RadioButton(
            &settings::C_playerIName, locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIteamL, Vector2f(20, 60), 80, true));
        player1Group->addRadioButton(new RadioButton(
            &settings::C_playerIName, locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIteamR, Vector2f(310, 60), 80, true));
        tabSpaceBall_->addWidget(player1Group);
        auto * player2Group = new RadioGroup();
        player2Group->addRadioButton(new RadioButton(
            &settings::C_playerIIName,
            locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIIteamL, Vector2f(120, 60), 80, true));
        player2Group->addRadioButton(new RadioButton(
            &settings::C_playerIIName,
            locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIIteamR, Vector2f(410, 60), 80, true));
        tabSpaceBall_->addWidget(player2Group);
        tabSpaceBall_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsLeft, 0,
            10, 1, Vector2f(20, 80), 230, 120, true));
        tabSpaceBall_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsRight, 0,
            10, 1, Vector2f(310, 80), 230, 120, true));
        tabSpaceBall_->addWidget(
            new LabeledBox(locales::getLocale(locales::GameOptions),
                           Vector2f(10, 120), 540, 100));
        tabSpaceBall_->addWidget(
            new Slider(locales::getLocale(locales::Pointlimit),
                       locales::getLocale(locales::ttPointLimitTeam),
                       &settings::C_pointLimitSB, 1, 20, 1, Vector2f(20, 150),
                       520, 270, true));
        tabSpaceBall_->addWidget(new Slider(
            locales::getLocale(locales::iDumb),
            locales::getLocale(locales::ttBotStrength), &settings::C_iDumb, 0,
            100, 5, Vector2f(20, 170), 520, 270, true));
        tabSpaceBall_->addWidget(
            new Slider(locales::getLocale(locales::PowerUpRate),
                       locales::getLocale(locales::ttPowerUpRate),
                       &settings::C_powerUpRate, 0, 100, 5, Vector2f(20, 190),
                       520, 270, true));
        tabSpaceBall_->addWidget(new Button(locales::getLocale(locales::Info),
                                            nullptr, &kInfo_, Vector2f(10, 235),
                                            90, 20));
        tabSpaceBall_->addWidget(
            new Button(locales::getLocale(locales::WeaponOptions), nullptr,
                       &kWeaponOptions_, Vector2f(110, 235), 120, 20));

        tabDeathMatch_->addWidget(
            new LabeledBox(locales::getLocale(locales::PlayerOptions),
                           Vector2f(10, 30), 540, 80));
        tabDeathMatch_->addWidget(new Checkbox(
            &settings::C_playerIName, locales::getLocale(locales::ttPlayersWho),
            &playerI_, Vector2f(20, 60), 100));
        tabDeathMatch_->addWidget(
            new Checkbox(&settings::C_playerIIName,
                         locales::getLocale(locales::ttPlayersWho), &playerII_,
                         Vector2f(310, 60), 100));
        tabDeathMatch_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsDeath, 0,
            20, 1, Vector2f(20, 80), 520, 270, true));
        tabDeathMatch_->addWidget(
            new LabeledBox(locales::getLocale(locales::GameOptions),
                           Vector2f(10, 120), 540, 100));
        tabDeathMatch_->addWidget(
            new Slider(locales::getLocale(locales::Fraglimit),
                       locales::getLocale(locales::ttPointLimitTeam),
                       &settings::C_pointLimitDM, 1, 100, 1, Vector2f(20, 150),
                       520, 270, true));
        tabDeathMatch_->addWidget(new Slider(
            locales::getLocale(locales::iDumb),
            locales::getLocale(locales::ttBotStrength), &settings::C_iDumb, 0,
            100, 5, Vector2f(20, 170), 520, 270, true));
        tabDeathMatch_->addWidget(
            new Slider(locales::getLocale(locales::PowerUpRate),
                       locales::getLocale(locales::ttPowerUpRate),
                       &settings::C_powerUpRate, 0, 100, 5, Vector2f(20, 190),
                       520, 270, true));
        tabDeathMatch_->addWidget(new Button(locales::getLocale(locales::Info),
                                             nullptr, &kInfo_,
                                             Vector2f(10, 235), 90, 20));
        tabDeathMatch_->addWidget(
            new Button(locales::getLocale(locales::WeaponOptions), nullptr,
                       &kWeaponOptions_, Vector2f(110, 235), 120, 20));

        tabTeamDeathMatch_->addWidget(new LabeledBox(
            locales::getLocale(locales::LeftTeam), Vector2f(10, 30), 250, 80));
        tabTeamDeathMatch_->addWidget(
            new LabeledBox(locales::getLocale(locales::RightTeam),
                           Vector2f(300, 30), 250, 80));
        auto * player1Group2 = new RadioGroup();
        player1Group2->addRadioButton(new RadioButton(
            &settings::C_playerIName, locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIteamL, Vector2f(20, 60), 80, true));
        player1Group2->addRadioButton(new RadioButton(
            &settings::C_playerIName, locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIteamR, Vector2f(310, 60), 80, true));
        tabTeamDeathMatch_->addWidget(player1Group2);
        auto * player2Group2 = new RadioGroup();
        player2Group2->addRadioButton(new RadioButton(
            &settings::C_playerIIName,
            locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIIteamL, Vector2f(120, 60), 80, true));
        player2Group2->addRadioButton(new RadioButton(
            &settings::C_playerIIName,
            locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIIteamR, Vector2f(410, 60), 80, true));
        tabTeamDeathMatch_->addWidget(player2Group2);
        tabTeamDeathMatch_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsLeft, 0,
            10, 1, Vector2f(20, 80), 230, 120, true));
        tabTeamDeathMatch_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsRight, 0,
            10, 1, Vector2f(310, 80), 230, 120, true));
        tabTeamDeathMatch_->addWidget(
            new LabeledBox(locales::getLocale(locales::GameOptions),
                           Vector2f(10, 120), 540, 100));
        tabTeamDeathMatch_->addWidget(
            new Slider(locales::getLocale(locales::Fraglimit),
                       locales::getLocale(locales::ttPointLimitTeam),
                       &settings::C_pointLimitTDM, 1, 100, 1, Vector2f(20, 150),
                       520, 270, true));
        tabTeamDeathMatch_->addWidget(new Slider(
            locales::getLocale(locales::iDumb),
            locales::getLocale(locales::ttBotStrength), &settings::C_iDumb, 0,
            100, 5, Vector2f(20, 170), 520, 270, true));
        tabTeamDeathMatch_->addWidget(
            new Slider(locales::getLocale(locales::PowerUpRate),
                       locales::getLocale(locales::ttPowerUpRate),
                       &settings::C_powerUpRate, 0, 100, 5, Vector2f(20, 190),
                       520, 270, true));
        tabTeamDeathMatch_->addWidget(
            new Button(locales::getLocale(locales::Info), nullptr, &kInfo_,
                       Vector2f(10, 235), 90, 20));
        tabTeamDeathMatch_->addWidget(
            new Button(locales::getLocale(locales::WeaponOptions), nullptr,
                       &kWeaponOptions_, Vector2f(110, 235), 120, 20));

        tabCannonKeep_->addWidget(new LabeledBox(
            locales::getLocale(locales::LeftTeam), Vector2f(10, 30), 250, 80));
        tabCannonKeep_->addWidget(
            new LabeledBox(locales::getLocale(locales::RightTeam),
                           Vector2f(300, 30), 250, 80));
        auto * player1Group3 = new RadioGroup();
        player1Group3->addRadioButton(new RadioButton(
            &settings::C_playerIName, locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIteamL, Vector2f(20, 60), 80, true));
        player1Group3->addRadioButton(new RadioButton(
            &settings::C_playerIName, locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIteamR, Vector2f(310, 60), 80, true));
        tabCannonKeep_->addWidget(player1Group3);
        auto * player2Group3 = new RadioGroup();
        player2Group3->addRadioButton(new RadioButton(
            &settings::C_playerIIName,
            locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIIteamL, Vector2f(120, 60), 80, true));
        player2Group3->addRadioButton(new RadioButton(
            &settings::C_playerIIName,
            locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIIteamR, Vector2f(410, 60), 80, true));
        tabCannonKeep_->addWidget(player2Group3);
        tabCannonKeep_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsLeft, 0,
            10, 1, Vector2f(20, 80), 230, 120, true));
        tabCannonKeep_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsRight, 0,
            10, 1, Vector2f(310, 80), 230, 120, true));
        tabCannonKeep_->addWidget(
            new LabeledBox(locales::getLocale(locales::GameOptions),
                           Vector2f(10, 120), 540, 100));
        tabCannonKeep_->addWidget(
            new Slider(locales::getLocale(locales::Pointlimit),
                       locales::getLocale(locales::ttPointLimitTeam),
                       &settings::C_pointLimitCK, 1, 50, 1, Vector2f(20, 150),
                       520, 270, true));
        tabCannonKeep_->addWidget(new Slider(
            locales::getLocale(locales::iDumb),
            locales::getLocale(locales::ttBotStrength), &settings::C_iDumb, 0,
            100, 5, Vector2f(20, 170), 520, 270, true));
        tabCannonKeep_->addWidget(
            new Slider(locales::getLocale(locales::PowerUpRate),
                       locales::getLocale(locales::ttPowerUpRate),
                       &settings::C_powerUpRate, 0, 100, 5, Vector2f(20, 190),
                       520, 270, true));
        tabCannonKeep_->addWidget(new Button(locales::getLocale(locales::Info),
                                             nullptr, &kInfo_,
                                             Vector2f(10, 235), 90, 20));
        tabCannonKeep_->addWidget(
            new Button(locales::getLocale(locales::WeaponOptions), nullptr,
                       &kWeaponOptions_, Vector2f(110, 235), 120, 20));

        tabGraveItation_->addWidget(new LabeledBox(
            locales::getLocale(locales::LeftTeam), Vector2f(10, 30), 250, 80));
        tabGraveItation_->addWidget(
            new LabeledBox(locales::getLocale(locales::RightTeam),
                           Vector2f(300, 30), 250, 80));
        auto * player1Group4 = new RadioGroup();
        player1Group4->addRadioButton(new RadioButton(
            &settings::C_playerIName, locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIteamL, Vector2f(20, 60), 80, true));
        player1Group4->addRadioButton(new RadioButton(
            &settings::C_playerIName, locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIteamR, Vector2f(310, 60), 80, true));
        tabGraveItation_->addWidget(player1Group4);
        auto * player2Group4 = new RadioGroup();
        player2Group4->addRadioButton(new RadioButton(
            &settings::C_playerIIName,
            locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIIteamL, Vector2f(120, 60), 80, true));
        player2Group4->addRadioButton(new RadioButton(
            &settings::C_playerIIName,
            locales::getLocale(locales::ttTeamPlayer),
            &settings::C_playerIIteamR, Vector2f(410, 60), 80, true));
        tabGraveItation_->addWidget(player2Group4);
        tabGraveItation_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsLeft, 0,
            10, 1, Vector2f(20, 80), 230, 120, true));
        tabGraveItation_->addWidget(new Slider(
            locales::getLocale(locales::Bots),
            locales::getLocale(locales::ttBotCount), &settings::C_botsRight, 0,
            10, 1, Vector2f(310, 80), 230, 120, true));
        tabGraveItation_->addWidget(
            new LabeledBox(locales::getLocale(locales::GameOptions),
                           Vector2f(10, 120), 540, 80));
        tabGraveItation_->addWidget(
            new Slider(locales::getLocale(locales::Fraglimit),
                       locales::getLocale(locales::ttPointLimitTeam),
                       &settings::C_pointLimitTDM, 1, 100, 1, Vector2f(20, 150),
                       520, 270, true));
        tabGraveItation_->addWidget(new Slider(
            locales::getLocale(locales::iDumb),
            locales::getLocale(locales::ttBotStrength), &settings::C_iDumb, 0,
            100, 5, Vector2f(20, 170), 520, 270, true));
        tabGraveItation_->addWidget(
            new Button(locales::getLocale(locales::Info), nullptr, &kInfo_,
                       Vector2f(10, 235), 90, 20));

        tabRally_->addWidget(
            new LabeledBox(locales::getLocale(locales::PlayerOptions),
                           Vector2f(10, 30), 540, 80));
        tabRally_->addWidget(new Checkbox(
            &settings::C_playerIName, locales::getLocale(locales::ttPlayersWho),
            &playerI_, Vector2f(20, 60), 100));
        tabRally_->addWidget(
            new Checkbox(&settings::C_playerIIName,
                         locales::getLocale(locales::ttPlayersWho), &playerII_,
                         Vector2f(310, 60), 100));
        tabRally_->addWidget(new Slider(locales::getLocale(locales::Bots),
                                        locales::getLocale(locales::ttBotCount),
                                        &settings::C_botsDeath, 0, 20, 1,
                                        Vector2f(20, 80), 520, 270, true));
        tabRally_->addWidget(
            new LabeledBox(locales::getLocale(locales::GameOptions),
                           Vector2f(10, 120), 540, 100));
        tabRally_->addWidget(
            new Slider(locales::getLocale(locales::Fraglimit),
                       locales::getLocale(locales::ttPointLimitTeam),
                       &settings::C_pointLimitDM, 1, 100, 1, Vector2f(20, 150),
                       520, 270, true));
        tabRally_->addWidget(new Slider(
            locales::getLocale(locales::iDumb),
            locales::getLocale(locales::ttBotStrength), &settings::C_iDumb, 0,
            100, 5, Vector2f(20, 170), 520, 270, true));
        tabRally_->addWidget(
            new Slider(locales::getLocale(locales::PowerUpRate),
                       locales::getLocale(locales::ttPowerUpRate),
                       &settings::C_powerUpRate, 0, 100, 5, Vector2f(20, 190),
                       520, 270, true));
        tabRally_->addWidget(new Button(locales::getLocale(locales::Info),
                                        nullptr, &kInfo_, Vector2f(10, 235), 90,
                                        20));
        tabRally_->addWidget(
            new Button(locales::getLocale(locales::WeaponOptions), nullptr,
                       &kWeaponOptions_, Vector2f(110, 235), 120, 20));

        tabList->addTab(tabSpaceBall_);
        tabList->addTab(tabDeathMatch_);
        tabList->addTab(tabTeamDeathMatch_);
        tabList->addTab(tabCannonKeep_);
        tabList->addTab(tabGraveItation_);
        // tabList->addTab(tabRally_);

        instance_->addWidget(tabList);

        instance_->addWidget(new Button(locales::getLocale(locales::Cancel),
                                        nullptr, &kCancel_, Vector2f(375, 300),
                                        90, 20));
    }
    return instance_;
}

void NewGameMenu::checkWidgets()
{
    if (kStart_)
    {
        kStart_ = false;
        music::fadeOut();
        if (tabSpaceBall_->isActive())
        {
            menus::hideWindow();
            menus::hideWindow();
            settings::save();
            games::start(games::gSpaceBall);
        }
        else if (tabTeamDeathMatch_->isActive())
        {
            menus::hideWindow();
            menus::hideWindow();
            settings::save();
            games::start(games::gTeamDeathMatch);
        }
        else if (tabDeathMatch_->isActive())
        {
            menus::hideWindow();
            menus::hideWindow();
            settings::C_playerIteamR = false;
            settings::C_playerIteamL = false;
            settings::C_playerIIteamR = false;
            settings::C_playerIIteamL = false;
            if (playerI_)
                settings::C_playerIteamL = true;
            if (playerII_)
                settings::C_playerIIteamL = true;
            settings::save();
            games::start(games::gDeathMatch);
        }
        else if (tabCannonKeep_->isActive())
        {
            menus::hideWindow();
            menus::hideWindow();
            settings::save();
            games::start(games::gCannonKeep);
        }
        else if (tabGraveItation_->isActive())
        {
            menus::hideWindow();
            menus::hideWindow();
            settings::save();
            settings::C_powerUpRate = 0;
            games::start(games::gGraveItation);
        }
        else if (tabRally_->isActive())
        {
            menus::hideWindow();
            menus::hideWindow();
            settings::C_playerIteamR = false;
            settings::C_playerIteamL = false;
            settings::C_playerIIteamR = false;
            settings::C_playerIIteamL = false;
            if (playerI_)
                settings::C_playerIteamL = true;
            if (playerII_)
                settings::C_playerIIteamL = true;
            settings::save();
            games::start(games::gRally);
        }
    }
    else if (kInfo_)
    {
        kInfo_ = false;
        if (tabSpaceBall_->isActive())
        {
            menus::showWindow(InfoSB::get());
        }
        else if (tabDeathMatch_->isActive())
        {
            menus::showWindow(InfoDM::get());
        }
        else if (tabTeamDeathMatch_->isActive())
        {
            menus::showWindow(InfoTDM::get());
        }
        else if (tabCannonKeep_->isActive())
        {
            menus::showWindow(InfoCK::get());
        }
        else if (tabGraveItation_->isActive())
        {
            menus::showWindow(InfoGIP::get());
        }
        else if (tabRally_->isActive())
        {
            menus::showWindow(InfoGIP::get());
        }
    }
    else if (tSB_)
    {
        tSB_ = false;
        if (settings::C_showInfoSB)
            menus::showWindow(InfoSB::get());
    }
    else if (tDM_)
    {
        tDM_ = false;
        if (settings::C_showInfoDM)
            menus::showWindow(InfoDM::get());
    }
    else if (tTDM_)
    {
        tTDM_ = false;
        if (settings::C_showInfoTDM)
            menus::showWindow(InfoTDM::get());
    }
    else if (tCK_)
    {
        tCK_ = false;
        if (settings::C_showInfoCK)
            menus::showWindow(InfoCK::get());
    }
    else if (tGI_)
    {
        tGI_ = false;
        if (settings::C_showInfoCK)
            menus::showWindow(InfoGIP::get());
    }
    else if (tRLY_)
    {
        tRLY_ = false;
        if (settings::C_showInfoCK)
            menus::showWindow(InfoGIP::get());
    }
    else if (kWeaponOptions_)
    {
        kWeaponOptions_ = false;
        menus::showWindow(WeaponOptions::get());
    }
    else if (kCancel_)
    {
        kCancel_ = false;
        menus::hideWindow();
    }
}

void NewGameMenu::onShow()
{
    if (settings::C_playerIteamL | settings::C_playerIteamR)
        playerI_ = true;
    if (settings::C_playerIIteamL | settings::C_playerIIteamR)
        playerII_ = true;
}

void NewGameMenu::reset()
{
    if (instance_)
        delete instance_;
    instance_ = nullptr;
}
