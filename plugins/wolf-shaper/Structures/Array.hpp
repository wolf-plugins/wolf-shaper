#pragma once

#include <stdexcept>
#include <cassert>

START_NAMESPACE_DISTRHO

namespace wolf
{

template <class T>
class Array
{
  public:
    explicit Array(const int size);
    ~Array();

    void add(const T item);

    T get(const int index);
    T getLast();

    void remove(const int index);
    void removeFirstMatchingValue(T value);
    void clear();

    int count();
    bool full();
    bool empty();

  private:
    int fCount;
    T *fItems;
    const int fCapacity;
};

template <class T>
Array<T>::Array(const int capacity) : fCount(0),
                                      fItems(new T[capacity]),
                                      fCapacity(capacity)
{
}

template <class T>
Array<T>::~Array()
{
    delete[] fItems;
}

template <class T>
void Array<T>::add(const T value)
{
    DISTRHO_SAFE_ASSERT(!full());

    fItems[fCount++] = value;
}

template <class T>
T Array<T>::get(const int index)
{
    DISTRHO_SAFE_ASSERT(index >= 0 && index < fCount);

    return fItems[index];
}

template <class T>
int Array<T>::count()
{
    return fCount;
}

template <class T>
bool Array<T>::full()
{
    return fCount == fCapacity;
}

template <class T>
bool Array<T>::empty()
{
    return fCount == 0;
}

template <class T>
void Array<T>::remove(const int index)
{
    DISTRHO_SAFE_ASSERT(index >= 0 && index < fCount);

    --fCount;

    for (int i = index; i < fCount; ++i)
    {
        fItems[i] = fItems[i + 1];
    }
}

template <class T>
void Array<T>::clear()
{
    fCount = 0;
}

template <class T>
T Array<T>::getLast()
{
    DISTRHO_SAFE_ASSERT(!empty());

    return fItems[fCount - 1];
}

template <class T>
void Array<T>::removeFirstMatchingValue(T value)
{
    if (empty())
        return;

    for (int i = 0; i < count(); ++i)
    {
        if (fItems[i] == value)
        {
            remove(i);
            return;
        }
    }
}

} // namespace wolf

END_NAMESPACE_DISTRHO
