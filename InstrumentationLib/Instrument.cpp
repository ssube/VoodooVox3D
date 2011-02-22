#include "Instrument.hpp"

Instrumenter::Instrumenter()
    : depth(0)
{
    ZeroMemory(timers, sizeof(DWORD) * 16);

    logfile = fopen("instrumentation.log", "rt");
}

Instrumenter::~Instrumenter()
{
    while ( depth > 0 )
    {
        this->StepOut();
    }

    fclose(logfile);
}

void Instrumenter::StepIn(const char * func, const char * file, int line)
{
    if ( depth < 15 )
    {
        fprintf(logfile, "Step In: %s (%s :: %u)\n", func, file, line);

        timers[depth] = GetTickCount();
        ++depth;
    } else {
        fprintf(logfile, "Step In failed (too deep) at %s (%s :: %u)\n", func, file, line);
    }
}

void Instrumenter::StepOut()
{
    if ( depth > 0 )
    {
        DWORD duration = GetTickCount() - timers[depth];
        fprintf(logfile, "Step Out: %u\n", duration);
        --depth;
    }
}
