/* TabList.hpp

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

#ifndef TABLIST_HPP_INCLUDED
#define TABLIST_HPP_INCLUDED

#include "Interface/Tab.hpp"
#include "Interface/UiElement.hpp"

#include <vector>

class TabList : public UiElement
{
  public:
    TabList(Vector2f const & topLeft, int width, int height);
    ~TabList();

    void mouseMoved(Vector2f const & position);
    void mouseWheelMoved(Vector2f const & position, int delta);
    void mouseLeft(bool down);
    void keyEvent(bool down, Key const & key);
    void textEntered(sf::Uint32 keyCode);

    bool tabNext();
    bool tabPrevious();

    void setFocus(UiElement * toBeFocused, bool isPrevious);
    void clearFocus();

    void draw() const;

    void addTab(Tab * toBeAdded);
    void activate(Tab * toBeActivated);

  private:
    std::vector<Tab *> tabs_;
    Tab * focusedTab_;
    int lastTabEnd_;
};

#endif
