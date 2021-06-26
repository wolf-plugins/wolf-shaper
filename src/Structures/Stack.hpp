#include <cassert>

#ifndef WOLF_STACK_H_INCLUDED
#define WOLF_STACK_H_INCLUDED

START_NAMESPACE_DISTRHO

namespace wolf
{

template <class T>
class Stack
{
  public:
    Stack(int size);
    ~Stack();

    void push(T item);
    T pop();
    T peek();
    int getCount();
    int getSize();

  private:
    T *items;
    int count;
    int size;
};

template <class T>
Stack<T>::Stack(int size)
{
    this->items = new T[size];
    this->size = size;
    this->count = 0;
};

template <class T>
Stack<T>::~Stack()
{
    delete[] items;
};

template <class T>
void Stack<T>::push(T item)
{
    DISTRHO_SAFE_ASSERT(count < size);

    items[count++] = item;
}

template <class T>
T Stack<T>::pop()
{
    DISTRHO_SAFE_ASSERT(count > 0);

    return items[--count];
}

template <class T>
T Stack<T>::peek()
{
    DISTRHO_SAFE_ASSERT(count > 0);

    return items[count - 1];
}

template <class T>
int Stack<T>::getCount()
{
    return count;
}

template <class T>
int Stack<T>::getSize()
{
    return size;
}
}

END_NAMESPACE_DISTRHO

#endif