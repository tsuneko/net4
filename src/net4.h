/**
 * Copyright (c) 2016 Team Kamikakushi
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
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
