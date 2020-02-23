#include "pch.h"
#include "windows.h"
#include "memoryAPI.h"
#include <iostream>

using namespace std;

DWORD processID;
HANDLE    broodHandle;
HWND broodWindow;

char broodMsg[32];

// REMOVE RPM and WPM Needed??
void readMem(int addr)
{
    broodWindow = FindWindowA(NULL, "Brood War");
    if (GetWindowThreadProcessId(broodWindow, &processID))

    {
        if (broodHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID))

        {
            ReadProcessMemory(broodHandle, (LPVOID)addr, broodMsg, 32, 0);
            CloseHandle(broodHandle);
        }
        else
        {
            MessageBoxA(0, "Unable to open process", NULL, MB_ICONEXCLAMATION | MB_OK);
        }
    }
    else
    {
        MessageBoxA(0, "Unable to obtain process ID", NULL, MB_ICONEXCLAMATION | MB_OK);
    }

}
void writeMem()
{

    broodWindow = FindWindowA(NULL, "Brood War");
    INT miner = 5000;

    if (GetWindowThreadProcessId(broodWindow, &processID))

    {
        if (broodHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID))

        {
            WriteProcessMemory(broodHandle, (LPVOID)0x508718, &miner, sizeof(miner), 0);
            CloseHandle(broodHandle);
        }
        else
        {
            MessageBoxA(0, "Unable to open process", NULL, MB_ICONEXCLAMATION | MB_OK);
        }
    }
    else
    {
        MessageBoxA(0, "Unable to obtain process ID", NULL, MB_ICONEXCLAMATION | MB_OK);
    }

}

void refreshWindow() {
    static int refreshFunction = 0x4DB240; // 1.09b Refresh function
    __asm {
        PUSHAD
        MOV ESI, DWORD PTR DS : [0x4FC734] // Your player number
        MOV EDX, 480 // Top Y
        MOV EBP, 0 // Left X
        PUSH 0 // Bottom Y
        PUSH 640 // Right X
        CALL DWORD PTR[refreshFunction]
        POPAD
    }
}

void playerText(char* txtMsg, int playerNumber) {
    static int playerTextfunc = 0x46DD20; // 1.09b Player Text Function
    __asm
    {
        PUSH txtMsg
        POP EDI
        PUSH playerNumber
        POP EDX
        MOV ECX, EDI
        CALL DWORD PTR[playerTextfunc]
    }
}

void centerText(char* txtMsg) {
    static int centerTextfunction = 0x46DCA0; // 1.09b Center Text Function
    refreshWindow(); //??
    __asm
    {
        PUSH txtMsg
        POP EAX
        MOV ECX, EAX
        MOV ESI, DWORD PTR DS : [0x4FC734] // Your player number
        MOV EDX, ESI
        CALL DWORD PTR[centerTextfunction]
    }
}

void anywhereText(char* txtMsg) {
    static int anywhereTextfunction = 0x4DCFD0; // 1.09b Anywhere Text Function
    static int jumpBack = 0x4DD40A;
}
