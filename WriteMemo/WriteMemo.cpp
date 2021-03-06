// WriteMemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "WriteMemo.h"
#include <windows.h>
#include <tlhelp32.h>

#define MAX_LOADSTRING 100

BOOL CALLBACK EnumChildWindowProc(HWND hWnd, LPARAM lParam);//枚举记事本中的子窗体
void UsrGetProcessHandle(HWND m_hWnd);
char mess[999999];


// 全局变量:
LPWSTR p_base_address;
HWND      hWriteDlg,hReadDlg;
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WriteDlgProc(HWND, UINT, WPARAM, LPARAM);
void UsrWriteBuffer(HWND m_hWnd, char* p_base_address, char * input_value);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WRITEMEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WRITEMEMO));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WRITEMEMO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WRITEMEMO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void CreateWriteMemoDlg(HINSTANCE hInstance, HWND parent_wnd, DLGPROC lpDialogFunc)
{
	HWND hwnd;
	hwnd = CreateDialogW(hInstance, MAKEINTRESOURCE(IDD_DIALOG_Write_Memo), NULL, lpDialogFunc);
	ShowWindow(hwnd, SW_SHOW);

	return;
}

//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//  目标: 处理主窗口的消息。
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

			case ID_WRITE_DATA:
				CreateWriteMemoDlg(hInst, hWnd, WriteDlgProc);

				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


//process function for dlg
LRESULT CALLBACK   WriteDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	hWriteDlg = hDlg;//Get the handle of read dlg
	char temp_data[10] = "abc";
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);//ID
		int EventId = HIWORD(wParam);//Event
		switch (wmId)
		{
		case IDOK:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;


		case IDC_BUTTON_DATA:
			UsrGetProcessHandle(NULL);
			//Write the data
			UsrWriteBuffer(hReadDlg, (LPSTR)(LPCWSTR)p_base_address,temp_data);
			
			return (INT_PTR)TRUE;
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hDlg, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
	return 0;
}



//Read buffer
void UsrWriteBuffer(HWND m_hWnd, char* p_base_address,char * input_value)
{
	SIZE_T dwLen;
	WriteProcessMemory(m_hWnd, (void*)0x0040505d, input_value, sizeof(input_value), &dwLen);
	//Show the result of read contents
	//SetDlgItemText(hWriteDlg, IDC_EDIT_DATA, (LPCWSTR)szBuff);
}





void UsrGetProcessHandle(HWND m_hWnd)
{
	//char sz[20];
	//sprintf_s(sz, "%d", m_hWnd);

	HANDLE   hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32   *pProcInfo = new   PROCESSENTRY32;
	pProcInfo->dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, pProcInfo);

	do
	{
		if (wcscmp(L"ReadMemo.exe", pProcInfo->szExeFile) == 0)
		{
			hReadDlg = (HWND)OpenProcess(PROCESS_ALL_ACCESS, 0, pProcInfo->th32ProcessID);
			break;
		}
	} while (Process32Next(hSnapshot, pProcInfo));

	delete pProcInfo;
}

////////////////////////////////////////////////////////////////////////


#if 0



int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND nphWnd = ::FindWindow(L"notepad", NULL);
	if (nphWnd)
	{
		char temp[1024];
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获得进程列表的快照，第一个參数能够有其它选项，具体请參考MSDN
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			::MessageBox(NULL, L"CreateToolhelp32Snapshot error", L"error", MB_OK);
			return 0;
		}
		HANDLE hProcess;
		BOOL bMore = ::Process32First(hProcessSnap, &pe32);//获得第一个进程的信息
		while (bMore)
		{
			::wsprintf((LPWSTR)temp, L"%s", pe32.szExeFile);
			if (!::strcmp(temp, "button.exe"))
			{
				hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, (DWORD)pe32.th32ProcessID);
				if (hProcess == NULL)
				{
					::wsprintf((LPWSTR)temp, L"%s", "打开进程失败!");
					::strcat(mess, temp);
				}
				else
				{
					::wsprintf((LPWSTR)temp, L"%s", "打开进程成功!");
					::strcat(mess, temp);
					//改写内存中内容
					int tmp = 97;//ascii:a
					SIZE_T dwNumberOfBytesRead;
					if (!::WriteProcessMemory(hProcess, (LPVOID)0x0040505d, &tmp, 1, &dwNumberOfBytesRead))
					{
						::wsprintf((LPWSTR)temp, L"%s", "写入失败");
						::strcat(mess, temp);
					}
					else
					{
						::wsprintf((LPWSTR)temp, L"%s", "写入成功");
						::strcat(mess, temp);
					}
				}
				break;
			}
			bMore = ::Process32Next(hProcessSnap, &pe32);//获得其它进程信息
		}
		::EnumChildWindows(nphWnd, EnumChildWindowProc, 0);//获得记事本的edit窗体，打印进程信息
		return 0;
	}
	else
	{
		::MessageBox(NULL, L"please open notepad", L"error", MB_OK);
		return 0;
	}
}
BOOL CALLBACK EnumChildWindowProc(HWND hWnd, LPARAM lParam)
{
	char temp1[256];
	if (hWnd)
	{
		::GetClassName(hWnd,(LPWSTR)temp1, 255);
		if (!::strcmp(temp1, "Edit"))//得到edit子窗体句柄
		{
			::SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)mess);
			return 0;
		}
	}
	return true;
}

#endif // 1
