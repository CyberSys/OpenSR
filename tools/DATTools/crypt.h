/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_DAT_CRYPT_H
#define RANGERS_DAT_CRYPT_H

#include <stdint.h>

namespace Rangers
{
namespace DAT
{
int32_t genKey();
void decrypt(uint8_t *data, uint32_t size, int32_t key, bool isCache = false);
void encrypt(uint8_t *data, uint32_t size, int32_t key, bool isCache = false);
}
}

#endif