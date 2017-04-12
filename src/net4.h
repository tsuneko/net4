/**
MIT License

Copyright (c) 2017 Tsuneko Shirley Kogami

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 **/

extern "C" {
#define LUA_COMPAT_ALL
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <windows.h>
#include <psapi.h>
#include <TlHelp32.h>  
#include <tchar.h>

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>

//.NET

// Debug Functions

bool loaded();
const char *version();
bool isProcessRunning(const char *processName);
bool isValidHandle(HANDLE handle);
bool isValidProcessID(DWORD ProcessID);

// WinAPI Functions

void WinAPI_MessageBox(const char *title, const char *message);

DWORD WinAPI_GetProcessID(const char *processName);
HANDLE WinAPI_OpenHandle(DWORD ProcessID);
bool WinAPI_CloseHandle(HANDLE handle);
void *GetModuleBaseAddress(const char *processName, DWORD ProcessID);
void *GetHWND(const char *windowName);

bool SuspendProcess(DWORD ProcessID);
bool ResumeProcess(DWORD ProcessID);

int ReadInt(HANDLE handle, DWORD address);
bool WriteInt(HANDLE handle, DWORD address, int value);
bool ReadBool(HANDLE handle, DWORD address);
bool WriteBool(HANDLE handle, DWORD address, bool value);
float ReadFloat(HANDLE handle, DWORD address);
bool WriteFloat(HANDLE handle, DWORD address, float value);

void *SignatureScan(HANDLE handle, const char *signature, const char *mask, int offset);
