#include <cassert>

#ifndef SPOONIE_STACK_H_INCLUDED
#define SPOONIE_STACK_H_INCLUDED

namespace spoonie
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
    assert(count < size);

    items[count++] = item;
}

template <class T>
T Stack<T>::pop()
{
    assert(count > 0);

    return items[--count];
}

template <class T>
T Stack<T>::peek()
{
    assert(count > 0);

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

#endif