/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_WORLD_RACE_H
#define OPENSR_WORLD_RACE_H

#include "WorldObject.h"

#include <QColor>
#include <QUrl>

namespace OpenSR
{
namespace World
{
class RaceStyle
{
    Q_GADGET

    Q_PROPERTY(QString icon MEMBER icon)
    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(QString sound MEMBER sound)

public:
    QString icon;
    QColor color;
    QString sound;
};

QDataStream& operator<<(QDataStream & stream, const RaceStyle & string);
QDataStream& operator>>(QDataStream & stream, RaceStyle & string);

class OPENSR_WORLD_API Race: public WorldObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(OpenSR::World::RaceStyle style READ style WRITE setStyle NOTIFY styleChanged)

public:
    Q_INVOKABLE Race(WorldObject *parent = 0, quint32 id = 0);
    virtual ~Race();

    RaceStyle style() const;
    void setStyle(const RaceStyle& style);

    virtual quint32 typeId() const;
    virtual QString namePrefix() const;

    virtual bool save(QDataStream &stream) const;
    virtual bool load(QDataStream &stream, const QMap<quint32, WorldObject*>& objects);

Q_SIGNALS:
    void styleChanged();

private:
    RaceStyle m_style;
};
}
}

Q_DECLARE_METATYPE(OpenSR::World::RaceStyle)

#endif // OPENSR_WORLD_RACE_H
