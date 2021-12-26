#include <windows.h> 
#include <stdio.h> 
// Global variable 

HINSTANCE hinst;

// Function prototypes. 

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
InitApplication(HINSTANCE);
InitInstance(HINSTANCE, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

#define IDM_EDUNDO  0 
#define IDM_EDCUT   1 
#define IDM_EDCOPY  2
#define IDM_EDPASTE 3
#define IDM_EDDEL   4
#define IDM_ABOUT   5


// Application entry point. 

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (!InitApplication(hinstance))
		return FALSE;

	if (!InitInstance(hinstance, nCmdShow))
		return FALSE;

	BOOL fGotMessage;
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(lpCmdLine);
}

BOOL InitApplication(HINSTANCE hinstance)
{
	WNDCLASSEX wcx;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wcx.cbSize = sizeof(wcx);          // size of structure 
	wcx.style = CS_HREDRAW |
		CS_VREDRAW;                    // redraw if size changes 
	wcx.lpfnWndProc = MainWndProc;     // points to window procedure 
	wcx.cbClsExtra = 0;                // no extra class memory 
	wcx.cbWndExtra = 0;                // no extra window memory 
	wcx.hInstance = hinstance;         // handle to instance 
	wcx.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);              // predefined app. icon 
	wcx.hCursor = LoadCursor(NULL,
		IDC_ARROW);                    // predefined arrow 
	wcx.hbrBackground = GetStockObject(
		WHITE_BRUSH);                  // white background brush 
	wcx.lpszMenuName = "MainMenu";    // name of menu resource 
	wcx.lpszClassName = "MainWClass";  // name of window class 
	wcx.hIconSm = LoadImage(hinstance, // small class icon 
		MAKEINTRESOURCE(5),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	// Register the window class. 

	return RegisterClassEx(&wcx);
}

BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
{
	HWND hwnd;

	// Save the application-instance handle. 

	hinst = hinstance;

	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",        // name of window class 
		"中国传媒大学",            // title-bar string 
		WS_OVERLAPPEDWINDOW, // top-level window 
		0,       // default horizontal position 
		0,       // default vertical position 
		800,       // default width 
		500,       // default height 
		(HWND)NULL,         // no owner window 
		(HMENU)NULL,        // use class menu 
		hinstance,           // handle to application instance 
		(LPVOID)NULL);      // no window-creation data 

	if (!hwnd)
		return FALSE;

	// Show the window and send a WM_PAINT message to the window 
	// procedure. 

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return TRUE;

}

#define ID_EDITCHILD 100
HWND hwndEdit;

LRESULT CALLBACK MainWndProc(HWND hwnd,      // window handle 
	UINT message,   // type of message 
	WPARAM wParam,  // additional information 
	LPARAM lParam)  // additional information 
{
	HMENU hMenu;

	CHAR lpszLatin[] = "教学案例";

	switch (message)
	{
	case WM_CREATE:

		hMenu = CreateMenu();
		if (hMenu != NULL)
		{
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDUNDO, "撤销");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDCUT, "剪切");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDCOPY, "复制");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDPASTE, "粘贴");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDDEL, "删除");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_ABOUT, "关于");

			SetMenu(hwnd, hMenu);
		}
		hwndEdit = CreateWindowEx(
			0, "EDIT",   // predefined class 
			NULL,         // no window title 
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			0, 0, 200, 100,   // set size in WM_SIZE message 
			hwnd,         // parent window 
			(HMENU)ID_EDITCHILD,   // edit control ID 
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);        // pointer not needed 

						  // Add text to the window. 
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)lpszLatin);

		return 0;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDM_EDUNDO:
			// Send WM_UNDO only if there is something to be undone. 

			if (SendMessage(hwndEdit, EM_CANUNDO, 0, 0))
				SendMessage(hwndEdit, WM_UNDO, 0, 0);
			else
			{
				MessageBox(hwndEdit,
					"Nothing to undo.",
					"Undo notification",
					MB_OK);
			}
			break;

		case IDM_EDCUT:
			SendMessage(hwndEdit, WM_CUT, 0, 0);
			break;

		case IDM_EDCOPY:
			SendMessage(hwndEdit, WM_COPY, 0, 0);
			break;

		case IDM_EDPASTE:
			SendMessage(hwndEdit, WM_PASTE, 0, 0);
			break;

		case IDM_EDDEL:
			SendMessage(hwndEdit, WM_CLEAR, 0, 0);
			break;

		case IDM_ABOUT:
			MessageBoxA(hwnd, "教学案例1.0", "教学案例1.0", MB_OK);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;

	case WM_SETFOCUS:
		SetFocus(hwndEdit);
		return 0;

	case WM_SIZE:
		// Make the edit control the size of the window's client area. 

		MoveWindow(hwndEdit,
			0, 0,                  // starting x- and y-coordinates 
			LOWORD(lParam),        // width of client area 
			HIWORD(lParam),        // height of client area 
			TRUE);                 // repaint window 
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

DWORD ReadFileContent(LPSTR szFilePath)
{
    //文件大小
    HANDLE hFileRead;
    //保存文件大小
    LARGE_INTEGER liFileSize;
    //文件数据缓存
    BYTE lpFileDataBuffer[32];

    //打开已经存在的文件，读取内容
    hFileRead = CreateFileA(szFilePath,
        GENERIC_READ,                   
        FILE_SHARE_READ,               
        NULL,                           
        OPEN_EXISTING,                  
        FILE_ATTRIBUTE_NORMAL,           
        NULL);                           
    //打开文件是否成功
    if(hFileRead==INVALID_HANDLE_VALUE)
    {
        printf("打开文件失败: %d",GetLastError());
    }
    
    //循环读取并打印内容
    while(TRUE)
    {
        DWORD i;
        if(!ReadFile(hFileRead,
            lpFileDataBuffer,  //存储读取文件内容
            NULL))               
        {
            printf("读取文件错误： %d\n",GetLastError());
            break;
        }

        for(i=0;i<dwReadedSize;i++)
        {
            printf("0x%x ",lpFileDataBuffer[i]);
        }
        printf("\n");
        liTotalRead+=dwReadedSize;
        if(liTotalRead==liFileSize.QuadPart)
        {
            printf("读取文件结束\n");
            break;
        }
    }
    CloseHandle(hFileRead);
    return 0;
}

DWORD SaveDataToFile(
    LPSTR szFilePath,
    LPVOID lpData,
    DWORD dwDataSize)
{
    HANDLE hFileWrite;
    //打开已经存在的文件，读取内容
    hFileWrite=CreateFileA(szFilePath,//要打开的文件名
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    //判断是否成功打开
    if(hFileWrite==INVALID_HANDLE_VALUE)
    {
        printf("打开文件失败： %d\n",GetLastError());
    }
    //设置文件指针
    SetFilePointer(hFileWrite,0,0,FILE_END);
    //将数据写入文件
    if(!WriteFile(hFileWrite,lpData,dwDataSize,&dwWritedDateSize,NULL))
    {
        printf("写文件失败： %d\n",GetLastError());
    }
    else
    {
        printf("写文件成功。\n");
    }
    CloseHandle(hFileWrite);
    return 0;
}
