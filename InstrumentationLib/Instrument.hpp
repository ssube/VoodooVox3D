#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef IMPORT_INSTRUMENT
#   define INSTRUMENT_API __declspec(dllexport)
#else
#   define INSTRUMENT_API __declspec(dllimport)
#endif

class INSTRUMENT_API Instrumenter
{
public:
    Instrumenter();
    ~Instrumenter();

    void StepIn(const char * func, const char * file, int line);
    void StepOut();

private:
    FILE * logfile;
    char depth;
    DWORD timers[16];
};

extern INSTRUMENT_API Instrumenter * GlobalInstrumenter;

#ifdef _DEBUG
#   define STEP_IN //__try { GlobalInstrumenter->StepIn(__FUNCTION__, __FILE__, __LINE__)
#   define STEP_OUT //} __finally { GlobalInstrumenter->StepOut(); }
#else
#   define STEP_IN
#   define STEP_OUT
#endif
