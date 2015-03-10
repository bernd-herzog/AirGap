#pragma once

#include <windows.h>
#include <psapi.h>

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#ifdef _DEBUG
#pragma comment(lib, "../Debug/AirGapLogger.lib")
#else
#pragma comment(lib, "../Release/AirGapLogger.lib")
#endif
#define CORE_API __declspec(dllimport)
#endif

CORE_API	void installHook();
CORE_API	void removeHook();
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);
