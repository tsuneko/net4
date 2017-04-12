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

#include "net4.h"

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

bool loaded()
{
	return true;
}

const char *version()
{
	return "0.0.8";
}

bool isProcessRunning( const char* processName )
{
	HANDLE SnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	if( SnapShot == INVALID_HANDLE_VALUE )
		return false;

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof( PROCESSENTRY32 );

	if( !Process32First( SnapShot, &procEntry ) )
		return false;

	do
	{
		if( strcmp( procEntry.szExeFile, processName ) == 0 )
			return true;
	}
	while( Process32Next( SnapShot, &procEntry ) );

	return false;
}

bool isValidHandle(HANDLE handle)
{
	if (handle != NULL && handle != INVALID_HANDLE_VALUE)
	{
		return true;
	}
	return false;
}

bool isValidProcessID(DWORD ProcessID)
{
	if (ProcessID != 0)
	{
		return true;
	}
	return false;
}

void WinAPI_MessageBox(const char *title, const char *message)
{
	MessageBox(0,message,title,0);
}

DWORD WinAPI_GetProcessID(const char *processName)
{
	PROCESSENTRY32 entryPoint32;
	HANDLE snapshotHandle;
	entryPoint32.dwSize = sizeof(PROCESSENTRY32);
	snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(snapshotHandle, &entryPoint32))
	{
		do
		{
			if(strcmp(entryPoint32.szExeFile, processName) == 0)
			{
				break;
			}
		}
		while(Process32Next(snapshotHandle, &entryPoint32));
	}
	if (isValidHandle(snapshotHandle))
	{
		CloseHandle(snapshotHandle);
		return entryPoint32.th32ProcessID;
	}
	return 0;
}

void *GetModuleBaseAddress(const char *processName, DWORD ProcessID) 
{ 
   DWORD_PTR dwModuleBaseAddress = 0; 
   HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);  
   if (hSnapshot != INVALID_HANDLE_VALUE) 
   { 
      MODULEENTRY32 ModuleEntry32; 
      ModuleEntry32.dwSize = sizeof(MODULEENTRY32); 
      if (Module32First(hSnapshot, &ModuleEntry32)) 
      { 
         do 
         { 
            if (_tcsicmp(ModuleEntry32.szModule, processName) == 0) 
            { 
               dwModuleBaseAddress = (DWORD_PTR)ModuleEntry32.modBaseAddr; 
               break; 
            } 
         } 
         while (Module32Next(hSnapshot, &ModuleEntry32)); 
      } 
      CloseHandle(hSnapshot); 
   } 
   return (void *)dwModuleBaseAddress;
}

void *GetHWND(const char *windowName)
{
	return (void *)FindWindow(NULL, windowName);
}

HANDLE WinAPI_OpenHandle(DWORD ProcessID)
{
	HANDLE handle;
	if (isValidProcessID(ProcessID))
	{
		handle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, false, ProcessID);
		if (isValidHandle(handle))
		{
			return handle;
		}
	}
	return NULL;
}

bool WinAPI_CloseHandle(HANDLE handle)
{
	if (isValidHandle(handle))
	{
		CloseHandle(handle);
		return true;
	}
	return false;
}

bool SuspendProcess(DWORD ProcessID)
{
	if (isValidProcessID(ProcessID))
	{	
		if (DebugActiveProcess(ProcessID) != 0)
		{
			DebugSetProcessKillOnExit(false);
			return true;
		}
	}
	return false;
}

bool ResumeProcess(DWORD ProcessID)
{
	if (ProcessID != 0)
	{
		bool debugPresent = false;
		HANDLE handle = WinAPI_OpenHandle(ProcessID);
		if (isValidHandle(handle))
		{
			if (CheckRemoteDebuggerPresent(handle, (PBOOL)&debugPresent) != 0 && debugPresent)
			{
				DebugActiveProcessStop(ProcessID);
				return true;
			}
		}
	}
	return false;
}

// Memory Functions

int ReadInt(HANDLE handle, DWORD address)
{
	int value;
	ReadProcessMemory(handle, (void *)(UINT_PTR)address, &value, sizeof(value), NULL);
	if (value == NULL)
	{
		return 0;
	}
	return value;
}

bool WriteInt(HANDLE handle, DWORD address, int value)
{
	SIZE_T bytesWritten;
	WriteProcessMemory(handle, (void *)(UINT_PTR)address, &value, sizeof(value), &bytesWritten);
	if (bytesWritten == sizeof(value))
	{
		return true;
	}
	return false;
}

bool ReadBool(HANDLE handle, DWORD address)
{
	bool value;
	ReadProcessMemory(handle, (void *)(UINT_PTR)address, &value, sizeof(value), NULL);
	if (value == NULL)
	{
		return false;
	}
	return value;
}

bool WriteBool(HANDLE handle, DWORD address, bool value)
{
	SIZE_T bytesWritten;
	WriteProcessMemory(handle, (void *)(UINT_PTR)address, &value, sizeof(value), &bytesWritten);
	if (bytesWritten == sizeof(value))
	{
		return true;
	}
	return false;
}

float ReadFloat(HANDLE handle, DWORD address)
{
	float value;
	ReadProcessMemory(handle, (void *)(UINT_PTR)address, &value, sizeof(value), NULL);
	if (value == NULL)
	{
		return 0;
	}
	return value;
}

bool WriteFloat(HANDLE handle, DWORD address, float value)
{
	SIZE_T bytesWritten;
	WriteProcessMemory(handle, (void *)(UINT_PTR)address, &value, sizeof(value), &bytesWritten);
	if (bytesWritten == sizeof(value))
	{
		return true;
	}
	return false;
}

void *SignatureScan(HANDLE handle, const char *signature, const char *mask, int offset)
{
	int maskLength = (int)strlen(mask);
	
	std::vector<unsigned char> dump;
	
	std::string sigstring(signature);
	std::istringstream sigstream(sigstring);
	std::vector<unsigned char> sig;
	unsigned int c;
	while (sigstream >> std::hex >> c)
	{
		sig.push_back(c);
	}

	bool canFind = true;
	
	MEMORY_BASIC_INFORMATION info;
	
	for (long currentAddress = 0; VirtualQueryEx(handle, (void *)(UINT_PTR)currentAddress, &info, sizeof(info)) == sizeof(info); currentAddress += (long)info.RegionSize) // iterate through all regions
	{
		if (info.State == MEM_COMMIT && (info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE))
		{
			SIZE_T bytesRead;
			dump.resize(info.RegionSize);
			ReadProcessMemory(handle, (void *)(UINT_PTR)currentAddress, &dump[0], info.RegionSize, &bytesRead);
			dump.resize(bytesRead);
			
			if (bytesRead > 0) // memory is dumped
			{
				long currentOffset = 0;
				while (currentOffset < bytesRead-maskLength) // iterate through dumped region
				{
					canFind = true;
					for (int x = 0; x < maskLength-1; x++) // iterate through signature / mask at particular offset in dumped region
					{
						if (mask[x] == 'x' && dump[currentOffset+x] != sig[x])
						{
							canFind = false;
							break;
						}
					}
					if (canFind) // found signature
					{
						return (void *)(UINT_PTR)(currentAddress + currentOffset + offset - 1);
					}
					currentOffset++;
				}
			}
		}
	}
	return NULL;
}
