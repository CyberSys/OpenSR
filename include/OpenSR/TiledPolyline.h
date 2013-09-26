/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_TILED_POLYLINE_H
#define RANGERS_TILED_POLYLINE_H

#include "OpenSR/Object.h"

namespace Rangers
{
class Texture;

class TiledPolylinePrivate;
class RANGERS_ENGINE_API TiledPolyline: public Object
{
    RANGERS_DECLARE_PRIVATE(TiledPolyline)
public:
    TiledPolyline();
    TiledPolyline(boost::shared_ptr<Texture> texture);
    TiledPolyline(const std::wstring& texture);

    virtual ~TiledPolyline();

    virtual void draw() const;
    virtual void processMain();

    boost::shared_ptr<Texture> texture() const;
    std::vector<Vector> polyline() const;

    void setPolyline(const std::vector<Vector> points);
    void setTexture(boost::shared_ptr<Texture> texture);
    void setTexture(const std::wstring& texture);

protected:
    TiledPolyline(TiledPolylinePrivate &p);

    RANGERS_DISABLE_COPY(TiledPolyline)
};
}

#endif
