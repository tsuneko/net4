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
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#include "wrap_net4.h"
#include "net4.h"

// local functions

std::string toHexStr(int hexInt)
{
	// int to hex
	
	std::stringstream stream;
	stream << std::hex << hexInt;
	std::string hex(stream.str());
	
	// hex to upper
	
	for(int i=0;hex[i]!=0;i++)
	{
		if(hex[i]<=122 && hex[i]>=97)
		{
			hex[i]-=32;
		}
	}
	
	// return std::string
	return hex;
}

// Type Definitions

static void lua_pushpointer(lua_State *L, void *ptr)
{
	lua_settop(L, 0);
	
	lua_newtable(L);
	
	lua_pushlightuserdata(L, ptr);
	lua_setfield(L, -2, "ptr");
	
	lua_pushnumber(L, (int)(UINT_PTR)ptr);
	lua_setfield(L, -2, "int");
	
	lua_pushstring(L, toHexStr((int)(UINT_PTR)ptr).c_str());
	lua_setfield(L, -2, "hex");
}

// Love Functions

static int w_Loaded(lua_State *L)
{
	lua_pushboolean(L, loaded());
	return 1;
}

static int w_Version(lua_State *L)
{
	lua_pushstring(L, version());
	return 1;
}

static int w_isProcessRunning(lua_State *L)
{
	const char *processName = luaL_checkstring(L, 1);
	lua_pushboolean(L, isProcessRunning(processName));
	return 1;
}

static int w_isValidHandle(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	lua_pushboolean(L, isValidHandle(handle));
	return 1;
}

static int w_isValidProcessID(lua_State *L)
{
	DWORD ProcessID = (DWORD)luaL_checknumber(L, 1);
	lua_pushboolean(L, isValidProcessID(ProcessID));
	return 1;
}

static int w_MessageBox(lua_State *L)
{
	const char *title = luaL_checkstring(L, 1);
	const char *message = luaL_checkstring(L, 2);
	WinAPI_MessageBox(title, message);
	return 1;
}

static int w_GetProcessID(lua_State *L)
{
	const char *processName = luaL_checkstring(L, 1);
	lua_pushnumber(L, WinAPI_GetProcessID(processName));
	return 1;
}

static int w_OpenHandle(lua_State *L)
{
	DWORD ProcessID = (DWORD)luaL_checknumber(L, 1);
	lua_pushpointer(L, WinAPI_OpenHandle(ProcessID));
	return 1;
}

static int w_CloseHandle(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	lua_pushboolean(L, WinAPI_CloseHandle(handle));
	return 1;
}

static int w_GetModuleBaseAddress(lua_State *L)
{
	const char *processName = luaL_checkstring(L, 1);
	DWORD ProcessID = (DWORD)luaL_checknumber(L, 2);
	lua_pushpointer(L, GetModuleBaseAddress(processName, ProcessID));
	return 1;
}

static int w_GetHWND(lua_State *L)
{
	const char *windowName = luaL_checkstring(L, 1);
	lua_pushpointer(L, GetHWND(windowName));
	return 1;
}

static int w_ResumeProcess(lua_State *L)
{
	DWORD ProcessID = (DWORD)luaL_checknumber(L, 1);
	lua_pushboolean(L, ResumeProcess(ProcessID));
	return 1;
}

static int w_SuspendProcess(lua_State *L)
{
	DWORD ProcessID = (DWORD)luaL_checknumber(L, 1);
	lua_pushboolean(L, SuspendProcess(ProcessID));
	return 1;
}

static int w_ReadInt(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	DWORD address = (DWORD)luaL_checknumber(L, 2);
	int readIntVar = ReadInt(handle, address);
	
	lua_settop(L, 0);
	lua_newtable(L);
	
	lua_pushnumber(L, readIntVar);
	lua_setfield(L, -2, "int");
	
	lua_pushstring(L, toHexStr(readIntVar).c_str());
	lua_setfield(L, -2, "hex");
	return 1;
}

static int w_WriteInt(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	DWORD address = (DWORD)luaL_checknumber(L, 2);
	int value = (int)luaL_checknumber(L,3);
	lua_pushboolean(L, WriteInt(handle, address, value));
	return 1;
}

static int w_WriteFloatArray4(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	DWORD address = (DWORD)luaL_checknumber(L, 2);
	float r = (float)luaL_checknumber(L, 3);
	float g = (float)luaL_checknumber(L, 4);
	float b = (float)luaL_checknumber(L, 5);
	float a = (float)luaL_checknumber(L, 6);
	float array[] = {r,g,b,a};
	WriteProcessMemory(handle, (void *)(UINT_PTR)address, &array[0], sizeof(array), NULL);
	return 0;
}

static int w_ReadBool(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	DWORD address = (DWORD)luaL_checknumber(L, 2);
	bool readBoolVar = ReadBool(handle, address);
	lua_pushboolean(L, readBoolVar);
	return 1;
}

static int w_WriteBool(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	DWORD address = (DWORD)luaL_checknumber(L, 2);
	bool value = lua_toboolean(L, 3)!=0;
	lua_pushboolean(L, WriteBool(handle, address, value));
	return 1;
}

static int w_ReadFloat(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	DWORD address = (DWORD)luaL_checknumber(L, 2);
	lua_pushnumber(L, ReadFloat(handle,address));
	return 1;
}

static int w_WriteFloat(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	DWORD address = (DWORD)luaL_checknumber(L, 2);
	float value = (float)luaL_checknumber(L, 3);
	lua_pushboolean(L, WriteFloat(handle, address, value));
	return 1;
}

static int w_SignatureScan(lua_State *L)
{
	HANDLE handle = (HANDLE)lua_touserdata(L, 1);
	const char *signature = luaL_checkstring(L, 2);
	const char *mask = luaL_checkstring(L, 3);
	int offset = (int)luaL_checknumber(L, 4);
	lua_pushpointer(L, SignatureScan(handle, signature, mask, offset));
	return 1;
}

static const struct luaL_Reg functions[] =
{
	{ "loaded", w_Loaded },
	{ "version", w_Version },
	
	{ "MessageBox", w_MessageBox },
	
	{ "isProcessRunning", w_isProcessRunning },
	{ "isValidHandle", w_isValidHandle },	
	{ "isValidProcessID", w_isValidProcessID },
	
	{ "ProcessID", w_GetProcessID },
	{ "OpenHandle", w_OpenHandle },
	{ "CloseHandle", w_CloseHandle },
	{ "GetModuleBaseAddress", w_GetModuleBaseAddress },	
	{ "GetHWND", w_GetHWND },
	
	{ "ResumeProcess", w_ResumeProcess },
	{ "SuspendProcess", w_SuspendProcess },
	
	{ "ReadInt", w_ReadInt },
	{ "WriteInt", w_WriteInt },
	{ "ReadBool", w_ReadBool },
	{ "WriteBool", w_WriteBool },
	{ "ReadFloat", w_ReadFloat },
	{ "WriteFloat", w_WriteFloat },
	
	{ "WriteFloatArray4", w_WriteFloatArray4 },
	
	{ "SignatureScan", w_SignatureScan },
	
	{ NULL , NULL }
};

extern "C" {
	void luax_register(lua_State *L, const char *name, const luaL_Reg *l);
}
  
extern "C" LOVE_NET4_EXPORT int luaopen_net4(lua_State *L)
{
	luaL_openlib(L, "net4", functions, 1);
	return 1;
}
