#ifndef SPOONIE_IDLE_THREAD_HPP_INCLUDED
#define SPOONIE_IDLE_THREAD_HPP_INCLUDED

#include "extra/Thread.hpp"

START_NAMESPACE_DISTRHO

class UIExporter;

class IdleThread : public Thread
{
  public:
    IdleThread(UIExporter *ui);

    void run();

  private:
    UIExporter *ui;
};

END_NAMESPACE_DISTRHO

#endif