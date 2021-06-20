#include "Stack.hpp"

#ifndef WOLF_OBJECT_POOL_H_INCLUDED
#define WOLF_OBJECT_POOL_H_INCLUDED

START_NAMESPACE_DISTRHO

namespace wolf
{
template <class T>
class ObjectPool
{
  public:
    template <typename... Args>
    ObjectPool(int numberOfObjects, Args &&... args);
    ~ObjectPool();

    T *getObject();
    void freeObject(T *object);

    int numberObjectsLeft();

  private:
    wolf::Stack<T *> objects;
};

template <class T>
template <typename... Args>
ObjectPool<T>::ObjectPool(int numberOfObjects, Args &&... args) : objects(numberOfObjects)
{
    for (int i = 0; i < objects.getSize(); ++i)
    {
        this->objects.push(new T(args...));
    }
}

template <class T>
ObjectPool<T>::~ObjectPool()
{    
    while (objects.getCount() > 0)
    {
        delete this->objects.pop();
    }
}

template <class T>
T *ObjectPool<T>::getObject()
{
    return this->objects.pop();
}

template <class T>
void ObjectPool<T>::freeObject(T *object)
{
    object->reset();

    this->objects.push(object);
}

template <class T>
int ObjectPool<T>::numberObjectsLeft()
{
    return this->objects.getCount();
}
}

END_NAMESPACE_DISTRHO

#endif