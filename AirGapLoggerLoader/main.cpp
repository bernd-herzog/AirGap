#include "main.h"
#include "logger.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#define NOTIFYICONDATA_ICONUID 0
#define NOTIFYICONDATA_CALLBACKMESSAGE (WM_USER + 1)
#define POPUPMENU_EXIT 0


int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	// kill old
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return 0;
	cProcesses = cbNeeded / sizeof(DWORD);
	unsigned int i;
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	for (i = 0; i < cProcesses; i++){
		if (aProcesses[i] != 0){
			//PrintProcessNameAndID( aProcesses[i] );

			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
				PROCESS_VM_READ | SYNCHRONIZE | PROCESS_TERMINATE,
				FALSE, aProcesses[i]);
			if (NULL != hProcess)
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
					&cbNeeded))
				{
					GetModuleBaseName(hProcess, hMod, szProcessName,
						sizeof(szProcessName) / sizeof(TCHAR));
					if (aProcesses[i] != GetCurrentProcessId() && wcsncmp(szProcessName, TEXT("AirGapLoggerLoader.exe"), lstrlenW(TEXT("AirGapLoggerLoader.exe"))) == 0)
					{
						TerminateProcess(hProcess, 0);
						return 0;
					}
				}
			}

		}
	}

	//window
	HWND hWnd;
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = (HINSTANCE)hInstance;
	wcex.lpszClassName = _T("WorkerW");

	hWnd = CreateWindow((LPCWSTR)RegisterClassEx(&wcex), 0, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, (HINSTANCE)hInstance, NULL);

	//tray
	NOTIFYICONDATA nid;
	nid.uID = NOTIFYICONDATA_ICONUID;
	nid.hWnd = hWnd;
	nid.cbSize = sizeof (nid);
	nid.uCallbackMessage = NOTIFYICONDATA_CALLBACKMESSAGE;
	wsprintf(nid.szTip, _T("MdLH"));

	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	nid.hIcon = LoadIcon((HINSTANCE)hInstance, MAKEINTRESOURCE(IDI_MDLH));
	Shell_NotifyIcon(NIM_ADD, &nid);

	//hook
	installHook();

	// msg queue
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//unhook
	removeHook();

	//untray
	nid.uID = 0;
	nid.hWnd = hWnd;
	nid.cbSize = sizeof (nid);
	Shell_NotifyIcon(NIM_DELETE, &nid);

	//unwindow
	DestroyWindow(hWnd);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HMENU menu;
	BOOL item;
	POINT cur;

	if (message == NOTIFYICONDATA_CALLBACKMESSAGE && wParam == NOTIFYICONDATA_ICONUID){
		switch (lParam)
		{
		case WM_RBUTTONUP:
			menu = CreatePopupMenu();
			AppendMenu(menu, MF_STRING, POPUPMENU_EXIT, _T("Exit"));
			GetCursorPos(&cur);
			item = TrackPopupMenu(menu, TPM_RETURNCMD, cur.x, cur.y, 0, hWnd, NULL);

			switch (item)
			{
			case POPUPMENU_EXIT:
				PostQuitMessage(0);
				break;
			}
			break;

		case WM_RBUTTONDBLCLK:
		case WM_MOUSEFIRST:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		default:
			//MessageBox(hWnd, _T("default"), _T("default"), MB_OK);
			break;
		}
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
