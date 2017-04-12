cmake_minimum_required(VERSION 3.1)

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
	# Protip: run cmake like this: cmake -G "<generator>" -H. -Bbuild
	message(FATAL_ERROR "Prevented in-tree build.")
endif()

include_directories($ENV{DXSDK_DIR}/Include)
link_directories("$ENV{DXSDK_DIR}/Lib/x64")

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

SET(LIB_NAME net4)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set (CMAKE_CXX_STANDARD 11)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(net4_X64 TRUE)
	set(net4_TARGET_PLATFORM x64)
else()
	set(net4_X86 TRUE)
	set(net4_TARGET_PLATFORM x86)
endif()

option(net4_JIT "Use LuaJIT" TRUE)

message(STATUS "Target platform: ${net4_TARGET_PLATFORM}")

if(net4)
	set(net4_MSVC_DLLS ${net4_MSVC_DLLS})

	set(net4_INCLUDE_DIRS)

	if(net4_JIT)
		set(net4_LUA_LIBRARY ${net4_LUAJIT_LIB})
		# net4_EXTRA_DLLS are non-runtime DLLs which should be bundled with the
		# net4 binary in installers, etc. It's only needed for external
		# (non-CMake) targets, i.e. LuaJIT.
		set(net4_EXTRA_DLLS ${net4_LUAJIT_DLL})
		set(EXTRA_DEPENDECIES luajit)

		set(net4_INCLUDE_DIRS
			${net4_INCLUDE_DIRS}
			${net4_LUAJIT_INCLUDE}
		)
		set(LINK_LIBRARIES
			${LINK_LIBRARIES}
			${net4_LUA_LIBRARY}
		)
		set(net4_MOVE_DLLS
			${net4_MOVE_DLLS}
			${net4_LUAJIT_DLL}
		)
	else()
		set(net4_LUA_LIBRARY ${net4_LUA51})

		set(LINK_LIBRARIES
			${LINK_LIBRARIES}
			${net4_LUA_LIBRARY}
		)
		set(net4_MOVE_DLLS
			${net4_MOVE_DLLS}
			${net4_LUA_LIBRARY}
		)
	endif()
else()

	set(LINK_LIBRARIES
		${net4_LUA_LIBRARY}
	)

	if(net4_JIT)
		find_package(LuaJIT REQUIRED)
		set(net4_LUA_LIBRARY ${LUAJIT_LIBRARY})
		set(net4_LUA_INCLUDE_DIR ${LUAJIT_INCLUDE_DIR})
	else()
		find_package(Lua51 REQUIRED)
		set(net4_LUA_LIBRARY ${LUA_LIBRARY})
		set(net4_LUA_INCLUDE_DIR ${LUA_INCLUDE_DIR})
	endif()
	
	set(net4_INCLUDE_DIRS
		${net4_INCLUDE_DIRS}
		${net4_LUA_INCLUDE_DIR}
	)
	
	set(LINK_LIBRARIES
		${LINK_LIBRARIES}
		${net4_LUA_LIBRARY}
	)

endif()

set(net4_SRC
	src/net4.h
	src/net4.cpp
	src/wrap_net4.h
	src/wrap_net4.cpp
)


set(net4_LIB_SRC
	${net4_SRC}
)

include_directories(
	src
	${net4_INCLUDE_DIRS}
)
link_directories(${LOVE_LINK_DIRS})

set(LOVE_RC)

if(EXTRA_DEPENDECIES)
	add_dependencies(${LIB_NAME} ${EXTRA_DEPENDECIES})
endif()

if(MSVC)
	set_target_properties(${LIB_NAME} PROPERTIES RELEASE_OUTPUT_NAME "net4" PDB_NAME "libnet4" IMPORT_PREFIX "lib")
	set_target_properties(${LIB_NAME} PROPERTIES DEBUG_OUTPUT_NAME "net4" PDB_NAME "libnet4" IMPORT_PREFIX "lib")
endif()
