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

#include "gi.h"
#include <libRanger.h>
#include <IL/il.h>

namespace Rangers
{
void BGRAToRGBA(char *data, int width, int height)
{
    for(int i = 0; i < width * height; i++)
    {
    	char buf =  data[i * 4];
        data[i * 4] =  data[i * 4 + 2];
        data[i * 4 + 2] = buf;
    }
}

int gi2png(const std::string &inFile, const std::string &outFile)
{
	std::ifstream giStream(inFile.c_str(), std::ios_base::in | std::ios_base::binary);

	GIFrame g = loadGIFile(giStream);

	ilInit();
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilTexImage(g.width, g.height, 1, 4, IL_BGRA, IL_UNSIGNED_BYTE, g.data);
	ilSaveImage(outFile.c_str());
	ilDeleteImage(image);
	delete g.data;
	return 0;
}

int gi2dds(const std::string &inFile, const std::string &outFile, DDSCompression compression)
{
	std::ifstream giStream(inFile.c_str(), std::ios_base::in | std::ios_base::binary);

    GIFrame g = loadGIFile(giStream);
    BGRAToRGBA((char*)g.data, g.width, g.height);
    int ret = saveDataToDDS(g.width, g.height, g.data, outFile, compression);
    delete g.data;
    return ret;
}
}
