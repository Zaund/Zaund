// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "memoryAPI.h"
#include "unitalert.h"

using namespace std;

void JmpPatch(void* pDest, void* pSrc, int nNops) {

    DWORD originalPstatus;

    VirtualProtect(pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &originalPstatus);

    *(char*)pSrc = (char)0xE9;
    *(DWORD*)((DWORD)pSrc + 1) = (DWORD)pDest - (DWORD)pSrc - 5;

    for (int i = 0; i < nNops; ++i) { *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90; }

    VirtualProtect(pSrc, 5 + nNops, originalPstatus, &originalPstatus);
}

DWORD WINAPI Listening(HMODULE hModule)
{
    bool zaundLoaded = TRUE;

    while (TRUE)
    { 
        if (GetAsyncKeyState(VK_OEM_3) & 1) {
            pickState();
        }
        //if (GetAsyncKeyState(VK_RETURN) & 1) {
            //centerText(marine);
        //}
        Sleep(1); //Prevents 100% CPU usage
    }
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //MessageBoxA(0, "DLL has been attached !", "DLL Bot", MB_ICONEXCLAMATION | MB_OK);
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Listening, hModule, 0, nullptr));
        JmpPatch(&unitAlert, (PBYTE)0x40EF5C, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
