/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "libRanger.h"

using namespace Rangers;

extern void FillARGBItoRGBA(unsigned char *bufdes, unsigned char *bufsrc, unsigned char *pal, int width, int height);

/*!
 * \param stream input file
 * \return HAI animation structure
 */
HAIAnimation Rangers::loadHAI(std::istream& stream)
{
    HAIHeader header;
    stream.read((char *)&header, sizeof(HAIHeader));

    int size = header.width * header.height;

    unsigned char *buffer = new unsigned char[header.width * header.height + header.palSize];
    unsigned char *rgba = new unsigned char[size * 4 * header.count];

    unsigned char *outbuf = rgba;

    for (int i = 0; i < header.count; i++)
    {
        stream.read((char *)buffer, header.width * header.height + header.palSize);
        FillARGBItoRGBA(outbuf, buffer, buffer + size, header.width, header.height);
        outbuf += size * 4;
    }

    delete buffer;

    HAIAnimation result;
    result.frameCount = header.count;
    result.frames = rgba;
    result.width = header.width;
    result.height = header.height;

    return result;
}
