// TodoList.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TodoList.h"
#include <windowsx.h>
#include <CommCtrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnDestroy(HWND hwnd);
void OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);
void TotalToDo(HWND hwnd, UINT uMsg);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TODOLIST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TODOLIST));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TODOLIST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TODOLIST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, L"Todo-List", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 405, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_CONTEXTMENU, OnContextMenu);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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

HWND txtTask;
HWND lsbTasks;
HWND lblTotal;
HWND btnUpdate;
HWND btnDeleteAll;
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	HWND lbl = CreateWindowEx(NULL, L"BUTTON", L"To Do:",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 30, 370, 100, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lbl, hFont, TRUE);

	txtTask = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL,
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		22.85, 58, 345, 20, hwnd,
		(HMENU)IDC_TXTTASK, lpCreateStruct->hInstance, NULL);
	SetWindowFont(txtTask, hFont, TRUE);

	HWND btnAdd = CreateWindowEx(NULL, L"BUTTON", L"ADD",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 90, 75, 30, hwnd,
		(HMENU)IDC_BTNADD, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnAdd, hFont, TRUE);
	MoveWindow(btnAdd, 52.85, 90, 75, 30, TRUE);

	btnUpdate = CreateWindowEx(NULL, L"BUTTON", L"UPDATE",
		WS_CHILD | BS_PUSHBUTTON,
		250, 90, 75, 30, hwnd,
		(HMENU)IDC_BTN_UPDATE, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnUpdate, hFont, TRUE);
	MoveWindow(btnUpdate, 157.85, 90, 75, 30, TRUE);

	btnDeleteAll = CreateWindowEx(NULL, L"BUTTON", L"DELETE ALL",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		250, 90, 75, 30, hwnd,
		(HMENU)IDC_BTN_DELETEALL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnDeleteAll, hFont, TRUE);
	MoveWindow(btnDeleteAll, 262.85, 90, 75, 30, TRUE);

	lbl = CreateWindowEx(NULL, L"BUTTON", L"To-do List:",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 130, 370, 400, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lbl, hFont, TRUE);

	lblTotal = CreateWindowEx(NULL, L"STATIC", L"Total: 0",
		WS_CHILD | WS_VISIBLE,
		315, 10, 80, 20, hwnd,
		(HMENU)IDC_TOTAL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblTotal, hFont, TRUE);

	lsbTasks = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | LBS_MULTIPLESEL | LBS_NOTIFY,
		22.75, 150, 345, 380, hwnd,
		(HMENU)IDC_LSB_TASKS, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lsbTasks, hFont, TRUE);


	return TRUE;
}

// Khai báo chuỗi bộ đệm chuyên chứa các chuỗi tạm
const int BUFFER_SIZE = 255;
WCHAR buffer[BUFFER_SIZE];
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (IDC_BTNADD == id) {
		ShowWindow(btnUpdate, SW_HIDE);
		GetWindowText(txtTask, buffer, BUFFER_SIZE);
		int length = GetWindowTextLength(txtTask);
		if (length != 0) { // check trường hợp người nhập chưa nhập mà add To-do trống
			int flag = 0;
			for (int i = 0; i < length; i++) {
				// check trường hợp người nhập cấn (hoặc lí do gì đó) chỉ nhập khoảng cách không có nội dung 
				if ((int(buffer[i]) > 32 && int(buffer[i]) < 123)) { // Chỉ xét trường không nhập hoặc chỉ nhập khoảng cách,
																	 // Vì người dùng có thể sử dụng ngôn ngữ riêng bản thân qui định với chỉ toàn kí tự, toàn số mà người dùng hiểu
																	 // Không có loại nội dung nào mà chỉ toàn dấu cách hoặc không nhập mà là nội dung có nghĩa :)
					ListBox_AddString(lsbTasks, buffer);
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				//MessageBox(0, L"We must have content!!!", L"WARNING!!!", 0);
				// Không để message box để người dùng đỡ ức chế khi lỡ tay bấm nút add với không nội dung
			}
		}
		Edit_SetText(txtTask, L""); // Xóa trống Editbox để sẵn sàng cho việc nhập kế	
	}
	else if (IDC_DELETE == id) {
		ShowWindow(btnUpdate, SW_HIDE);
		int count = ListBox_GetSelCount(lsbTasks);  // Bao nhiêu item được chọn? 
		int* indices = new int[count];				// Cấp phát mảng tương ứng chứa chỉ mục
		ListBox_GetSelItems(lsbTasks, count, indices); // Lấy chỉ mục các item được chọn
		for (auto i = count - 1; i >= 0; i--) { // Lần lượt duyệt qua từng chỉ mục			
			ListBox_DeleteString(lsbTasks, indices[i]); // Xóa bỏ item khỏi ListBox
		}
		delete[] indices; // Thu hồi vùng nhớ đã cấp nhát
		Edit_SetText(txtTask, L""); // trường hợp khi đang update mà muốn delete thì sẽ làm trống lại
	}
	else if (IDC_LSB_TASKS == id) {
		switch (codeNotify) {
		case LBN_SELCHANGE:
			int selectedIndex = ListBox_GetCurSel(lsbTasks);  // Lấy chỉ mục của item được click
			ListBox_GetText(lsbTasks, selectedIndex, buffer); // Lấy text
			Edit_SetText(txtTask, buffer);
			ShowWindow(btnUpdate, SW_SHOW);
			break;
		}
	}
	else if (IDC_BTN_UPDATE == id) {
		int selectedIndex = ListBox_GetCurSel(lsbTasks);  // Lấy chỉ mục của item được click
		Edit_GetText(txtTask, buffer, BUFFER_SIZE);

		// ListBox mặc định không cho thay đổi text		
		ListBox_DeleteString(lsbTasks, selectedIndex); // Nên phải xóa đi dòng bị thay đổi
		ListBox_InsertString(lsbTasks, selectedIndex, buffer); // Và chèn ngược lại		
		ShowWindow(btnUpdate, SW_HIDE); // Cập nhật xong rồi thì ẩn nút Cập nhật nào
		Edit_SetText(txtTask, L""); // Xóa trống luôn Edit box nào
	}
	else if (IDC_BTN_DELETEALL == id) {
		ShowWindow(btnUpdate, SW_HIDE);
		if (ListBox_GetCount(lsbTasks) != 0) { // check có to do để xóa không???
			int check = MessageBox(NULL, L"Are you sure?", L"Confirm", MB_ICONEXCLAMATION | MB_YESNO); // check người dùng có bấm nhầm xóa hết không
			if (check == IDYES) {
				ListBox_ResetContent(lsbTasks);
			}
		}
		Edit_SetText(txtTask, L""); // trường hợp khi đang update mà muốn delete all thì sẽ làm trống lại
	}
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		//DestroyWindow(hwnd); // Chỉ đóng app không thoát app được
		PostMessage(hwnd, WM_QUIT, 0, 0);// Thoát hoàn toàn app
		break;
	}
	TotalToDo(hwnd, id); // Số lượng To-do hiện tại
}

void OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos) {
	if (hwndContext == lsbTasks) { // Nếu bấm chuột phải vào ListBox
		//MessageBox(0, 0, 0, 0);
		// Tạo menu popup
		HMENU popup = CreatePopupMenu();
		InsertMenu(popup, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDC_DELETE, L"Delete");
		// Hiển thị menu vừa tạo ra
		TrackPopupMenu(popup, TPM_TOPALIGN | TPM_LEFTALIGN, xPos, yPos, 0, hwnd, NULL);
	}
}

void TotalToDo(HWND hwnd, UINT uMsg) {
	int count = ListBox_GetCount(lsbTasks);
	WCHAR wcCount[10];
	wsprintf(wcCount, L"Total: %d", count);
	SetWindowText(lblTotal, wcCount);
}

void OnDestroy(HWND hwnd) {
	PostQuitMessage(0);
}
