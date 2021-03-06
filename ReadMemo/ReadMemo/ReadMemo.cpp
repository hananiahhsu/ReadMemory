// ReadMemo.cpp : 定义应用程序的入口点。
#include "stdafx.h"
#include "ReadMemo.h"
#include "CReadMemoDlg.h"
using namespace std;

#define MAX_LOADSTRING 100
//xuxu
#define IDM_OPT1     301
#define IDM_OPT2     302

HMENU hRoot;
void CreateMyMenu();//创建菜单
void CreateReadMemoDlg(HINSTANCE hInstance, HWND parent_wnd, DLGPROC lpDialogFunc);
void UsrReadBuffer(HWND m_hWnd, char* p_base_address);
void UsrGetProcessHandle(HWND m_hWnd);
//
LPWSTR sDialogBuf;
LPWSTR p_base_address;
// 全局变量:
HWND      hReadDlg=NULL,hReadProcessHandle=NULL;
HWND      hWriteDlg = NULL;
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
WCHAR szReadTitle[MAX_LOADSTRING];                  // READ
WCHAR szReadWindowClass[MAX_LOADSTRING];            // READ
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ReadDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About_Dialog2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
    LoadStringW(hInstance, IDC_READMEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_READMEMO));

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



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_READMEMO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_READMEMO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


void CreateMyMenu()
{
	hRoot = CreateMenu();
	if (!hRoot)
		return;
	HMENU pop1 = CreatePopupMenu();
	AppendMenu(hRoot,
		MF_POPUP,
		(UINT_PTR)pop1,
		L"操作");
	// 一种方法是使用AppendMenu函数
	AppendMenu(pop1,
		MF_STRING,
		IDM_OPT1,
		L"飞机");

	// 另一种方法是使用InsertMenuItem函数
	MENUITEMINFO mif;
	mif.cbSize = sizeof(MENUITEMINFO);
	mif.cch = 100;
	mif.dwItemData = NULL;
	mif.dwTypeData = (LPWSTR)L"机关枪";
	mif.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;
	mif.fState = MFS_ENABLED;
	mif.fType = MIIM_STRING;
	mif.wID = IDM_OPT2;

	InsertMenuItem(pop1, IDM_OPT2, FALSE, &mif);

}



//
//   函数: InitInstance(HINSTANCE, int)
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

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
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
			case IDM_READ_DATA:
				//MessageBox(hWnd, L"READ。", L"提示", MB_OK);
				CreateReadMemoDlg(hInst, hWnd, ReadDlgProc);//2018.10.20--hsu
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



/********************************************************
//--Crate the dialog of read memory from process
//
*********************************************************/
void CreateReadMemoDlg(HINSTANCE hInstance,HWND parent_wnd, DLGPROC lpDialogFunc)
{
	HWND hwnd;
	hwnd = CreateDialogW(hInstance, MAKEINTRESOURCE(IDD_DIALOG_READ_MEMO), NULL, lpDialogFunc);
	ShowWindow(hwnd, SW_SHOW);
	HWND edit_handle =  GetDlgItem(hwnd, IDC_EDIT_READ);
	//SendMessage(edit_handle, EM_SETEVENTMASK, 0, ENM_SELCHANGE |
	//	ENM_MOUSEEVENTS | ENM_CHANGE | ENM_KEYEVENTS | ENM_SCROLL | ENM_DROPFILES);
	//LoadStringW(hInstance, IDS_READDLG, szReadTitle, MAX_LOADSTRING);
	//LoadStringW(hInstance, IDD_DIALOG_READ_MEMO, szReadWindowClass, MAX_LOADSTRING);
	//HWND hWnd = CreateWindowW(szReadWindowClass, szReadTitle, WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, parent_wnd, nullptr, hInstance, nullptr);
	//ShowWindow(hWnd, SW_SHOW);
	return;
}


//process function for dlg
LRESULT CALLBACK   ReadDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	hReadDlg = hDlg;//Get the handle of read dlg

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


		case IDC_BUTTON_READ_DATA:

			UsrGetProcessHandle(NULL);
			UsrReadBuffer(hReadProcessHandle, (LPSTR)(LPCWSTR)0x0040505d);
			

			return (INT_PTR)TRUE;
        //You should add proc function to the next dlg... otherwise the dlg cannot be closed properly
		case IDC_BUTTON_NEXT_DLG:
		    HWND hNextDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_NEXT), hDlg, About_Dialog2);
			 //初始化窗口标题栏，并显示窗口
			 SetWindowText(hNextDialog, L"Next dlg... ...");
			 ShowWindow(hNextDialog, SW_SHOWNA);
			 UpdateWindow(hNextDialog);
			break;
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



// “关于”对话框2框的消息处理程序。
INT_PTR CALLBACK About_Dialog2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	int dwmId;
	int ret_num;

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		dwmId = LOWORD(wParam);
		switch (dwmId)
		{
		case IDOK:
		{
			ret_num = GetDlgItemText(hDlg, IDC_EDIT1, sDialogBuf, sizeof(sDialogBuf));
			if (strlen((char *)sDialogBuf) <= 0)
			{
				MessageBox(NULL, L"输入为空请重新输入", L"提示", NULL);
			}
			else
			{
				MessageBox(NULL, sDialogBuf, L"提示", NULL);
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		}
		break;
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
		default:
			break;
		}
	}
	return (INT_PTR)FALSE;
}



//Read buffer
void UsrReadBuffer(HWND m_hWnd,char* p_base_address)
{
	int nLen  = 10;

	char szBuff[1024];
	BYTE m_btCurrIndex;
	SIZE_T dwLen;
	ReadProcessMemory(m_hWnd, (void*)p_base_address, szBuff, nLen, &dwLen);
	//Show the result of read contents

	WCHAR wszClassName[256];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, szBuff, strlen(szBuff) + 1, wszClassName,
		sizeof(wszClassName) / sizeof(wszClassName[0]));

	SetDlgItemText(hReadDlg, IDC_EDIT_READ, (LPCWSTR)wszClassName);
	UpdateWindow(hReadDlg);
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
			hReadProcessHandle = (HWND)OpenProcess(PROCESS_ALL_ACCESS, 0, pProcInfo->th32ProcessID);
			break;
		}
	} while (Process32Next(hSnapshot, pProcInfo));

	delete pProcInfo;
}