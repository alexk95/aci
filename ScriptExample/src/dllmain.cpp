#include <Windows.h>
#include "ScriptSharedTypes.h"
#include <aci/InterpreterObject.h>

#include "MyCommand.h"

extern "C" {
	SCRIPT_API_EXPORT aci::InterpreterObject ** generateObjects(int& _count) {
		_count = 1;
		aci::InterpreterObject ** ret = new aci::InterpreterObject *[_count];
		ret[0] = new MyCommand;
		return ret;
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
