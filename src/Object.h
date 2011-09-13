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

#ifndef RANGERS_OBJECT_H
#define RANGERS_OBJECT_H

#include "Types.h"
#include <boost/thread/recursive_mutex.hpp>
#include <list>

namespace Rangers
{
class Object
{
public:
    Object(Object *parent = 0);
    Object(const Vector& pos, float rot, int layer = 0, Object *parent = 0);
    Object(const Object& other);

    ~Object();

    virtual void draw();

    //Function to process all needed OpenGL functions
    virtual void processMain();
    //Function to process all logic operations
    virtual void processLogic(int dt);

    void setPosition(float x, float y);
    void setPosition(const Vector& pos);
    void setRotation(float angle);
    void setColor(float r, float g, float b, float a = 1.0f);

    void addChild(Object *object);
    void removeChild(Object *object);
    void setParent(Object *parent);

    const Vector& position() const;
    float rotation() const;
    int layer() const;
    Object* parent() const;

    void setLayer(int layer);

    Object& operator=(const Object& other);

private:
    int objectLayer;

protected:
    boost::recursive_mutex objectMutex;
    float red, green, blue, alpha;
    bool marked;
    Object *objectParent;
    std::list<Object*> objectChilds;
    Vector objPosition;
    float objRotation;

    bool prepareDraw();
    void endDraw();
    void lock();
    void unlock();
    void markToUpdate();
};
}

#endif
