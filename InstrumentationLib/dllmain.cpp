#include "Instrument.hpp"

Instrumenter * GlobalInstrumenter;

int __stdcall DllMain(_In_ void * _HDllHandle, _In_ unsigned _Reason, _In_opt_ void * _Reserved)
{
#ifdef _DEBUG
    switch ( _Reason )
    {
    case DLL_PROCESS_ATTACH:
        GlobalInstrumenter = new Instrumenter();
        break;
    case DLL_PROCESS_DETACH:
        delete GlobalInstrumenter;
    }
#endif

    return TRUE;
}
