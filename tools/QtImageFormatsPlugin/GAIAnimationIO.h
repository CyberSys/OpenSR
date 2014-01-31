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

#ifndef GAIANIMATIONIO_H
#define GAIANIMATIONIO_H

#include <QImageIOHandler>
#include <QImage>
#include <QVector>

namespace Rangers
{
struct GAIHeader;

class GAIAnimationIO : public QImageIOHandler
{
public:
    GAIAnimationIO();
    ~GAIAnimationIO();

    virtual bool canRead() const;
    virtual bool read(QImage *image);

    virtual bool supportsOption(ImageOption option) const;
    virtual QVariant option(ImageOption option) const;

    int imageCount() const;
    int loopCount() const;
    int nextImageDelay() const;
    int currentImageNumber() const;

private:
    void peekHeader() const;
    void readTimes();

    mutable GAIHeader *m_header;
    QVector<int> m_times;
    int m_currentFrame;
    QImage m_prev;
};
}

#endif // GAIANIMATIONIO_H
