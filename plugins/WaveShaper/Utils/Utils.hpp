#ifndef SPOONIE_UTILS_DEFINED_H
#define SPOONIE_UTILS_DEFINED_H

namespace spoonie
{
static int slog(const char *format, ...)
{
    va_list arg;
    int done;

    va_start(arg, format);
    done = vfprintf(stderr, format, arg);
    va_end(arg);

    return done;
}
}

#endif