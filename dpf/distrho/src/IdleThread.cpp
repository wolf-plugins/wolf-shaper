#include "DistrhoUIInternal.hpp"
#include "IdleThread.hpp"

START_NAMESPACE_DISTRHO

IdleThread::IdleThread(UIExporter *ui) : Thread("IdleThread"),
                                         ui(ui)
{
}

void IdleThread::run()
{
    do
    {
        ui->glApp.idle();

        if (ui->glWindow.isReady())
            ui->fUI->uiIdle();

        usleep(12 * 1000);
    } while (!shouldThreadExit());
}

END_NAMESPACE_DISTRHO