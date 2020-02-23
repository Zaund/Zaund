#pragma once
#ifndef memoryAPI
#define memoryAPI

void readMem(int addr);
void writeMem();
void refreshWindow();
void playerText(char* txtMsg, int playerNumber);
void centerText(char* txtMsg);
void anywhereText(char* txtMsg);

#endif