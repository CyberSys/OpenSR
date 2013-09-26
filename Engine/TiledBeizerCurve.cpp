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

#include "OpenSR/TiledBeizerCurve.h"
#include "OpenSR/private/TiledBeizerCurve_p.h"
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Texture.h>
#include <OpenSR/Log.h>

namespace
{
const float ANGLE_TRESHOLD = 0.99f;
}

namespace Rangers
{
TiledBeizerCurve::TiledBeizerCurve(): TiledPolyline(*(new TiledBeizerCurvePrivate()))
{
}

TiledBeizerCurve::TiledBeizerCurve(boost::shared_ptr<Texture> texture): TiledPolyline(*(new TiledBeizerCurvePrivate()))
{
    RANGERS_D(TiledBeizerCurve);
    d->texture = texture;
}

TiledBeizerCurve::TiledBeizerCurve(const std::wstring& texture): TiledPolyline(*(new TiledBeizerCurvePrivate()))
{
    RANGERS_D(TiledBeizerCurve);
    d->texture = ResourceManager::instance().loadTexture(texture);
}

TiledBeizerCurve::~TiledBeizerCurve()
{
}

TiledBeizerCurve::TiledBeizerCurve(TiledBeizerCurvePrivate &p): TiledPolyline(p)
{
}

TiledBeizerCurvePrivate::TiledBeizerCurvePrivate()
{
}

void TiledBeizerCurve::setControlPoints(Vector p0, Vector p1, Vector p2, Vector p3)
{
    RANGERS_D(TiledBeizerCurve);
    d->p0 = p0;
    d->p1 = p1;
    d->p2 = p2;
    d->p3 = p3;
    calcCurve();
}

Vector TiledBeizerCurvePrivate::calcBezierPoint(float t)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Vector p = uuu * p0;
    p += 3 * uu * t * p1;
    p += 3 * u * tt * p2;
    p += ttt * p3;

    return p;
}

int TiledBeizerCurvePrivate::findPoints(float t0, float t1, int index)
{
    float tMid = (t0 + t1) / 2;
    Vector p0 = calcBezierPoint(t0);
    Vector p1 = calcBezierPoint(t1);

    if (texture && ((p0 - p1).length() < texture->width()))
    {
        return 0;
    }

    Vector pMid = calcBezierPoint(tMid);
    Vector leftDirection = (p0 - pMid).norm();
    Vector rightDirection = (p1 - pMid).norm();

    if (fabs(leftDirection * rightDirection) < ANGLE_TRESHOLD)
    {
        int pointsAdded = 0;

        pointsAdded += findPoints(t0, tMid, index);

        points.insert(points.begin() + index + pointsAdded, pMid);
        pointsAdded++;

        pointsAdded += findPoints(tMid, t1, index + pointsAdded);
        return pointsAdded;
    }
    return 0;
}

void TiledBeizerCurve::calcCurve()
{
    RANGERS_D(TiledBeizerCurve);
    Vector p0 = d->calcBezierPoint(0.0f);
    Vector p1 = d->calcBezierPoint(1.0f);
    d->points.push_back(p0);
    d->points.push_back(p1);

    d->findPoints(0.0f, 1.0f, 1);

    Log::debug() << "Beizer points count: " << d->points.size();

    markToUpdate();
}
}
