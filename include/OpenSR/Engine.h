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

#ifndef OPENSR_ENGINE_H
#define OPENSR_ENGINE_H

#include <QApplication>

class QQuickView;
class QQmlEngine;
class QJSEngine;

namespace OpenSR
{
class SoundManager;
class Engine: public QApplication
{
    Q_OBJECT
public:
    Engine(int argc, char **argv);
    virtual ~Engine();

    int run();

    SoundManager *sound() const;

public Q_SLOTS:
    void addRCCArchive(const QString& path);

private:
    QQuickView *m_qmlView;
    QQmlEngine *m_qmlEngine;
    QJSEngine *m_scriptEngine;
    SoundManager *m_sound;
};
}

#endif
