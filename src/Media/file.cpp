/* file.cpp

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

#include "Media/file.hpp"

#include <SFML/Config.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Utf.hpp>
#include <algorithm>
#include <fribidi/fribidi-bidi-types.h>
#include <fribidi/fribidi-char-sets.h>
#include <fribidi/fribidi-types.h>
#include <fribidi/fribidi.h>
#include <fstream>
#include <iterator>
#include <vector>

namespace file
{
auto load(std::string fileName, std::vector<sf::String> & strings) -> bool
{
    // Open the file (contains UTF-8 encoded text)
    std::ifstream fileStream(fileName.c_str());
    if (!fileStream.is_open())
    {
        return false;
    }
    else
    {
        std::string line;
        // int lineCount(0);
        // Play with all the lines in the file
        while (std::getline(fileStream, line, '\n'))
        {
            // ++ lineCount;

            // ignore empty lines
            if (line.empty())
                continue;

            // ignore comment lines
            if (line.size() >= 2 && (line[0] == 47 && line[1] == 47))
                continue;

            // remove '\r' at end of lines, when file has a CR LF EOL
            // (windows...)
            if (*(line.end() - 1) == '\r')
                line.erase(line.end() - 1);

            // Convert it to utf-32
            int inSize = line.size();
            std::vector<FriBidiChar> logical(inSize);
            const char * tmp(line.c_str());
            int outSize = fribidi_charset_to_unicode(FRIBIDI_CHAR_SET_UTF8, tmp,
                                                     inSize, logical.data());

            std::vector<FriBidiChar> visual(outSize);
            FriBidiParType base = FRIBIDI_PAR_LTR;
            fribidi_log2vis(logical.data(), outSize, &base, visual.data(),
                            nullptr, nullptr, nullptr);

            std::vector<char> outstring(outSize * 4, 0);
            fribidi_unicode_to_charset(FRIBIDI_CHAR_SET_UTF8, visual.data(),
                                       outSize, outstring.data());

            line = std::string(outstring.data());

            std::basic_string<sf::Uint32> utf32line;
            sf::Utf8::toUtf32(line.begin(), line.end(),
                              back_inserter(utf32line));

            strings.emplace_back(utf32line);
        }
        fileStream.close();
    }
    return true;
}
} // namespace file
