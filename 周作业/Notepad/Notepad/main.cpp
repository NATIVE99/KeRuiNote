#include <afxwin.h>
#include <commctrl.h>
#include <shellapi.h>
#include <tchar.h>
#include "resource1.h"

#define ID_EDITCHILD 0x123
HWND g_hwndEdit=NULL;
HWND g_dlg=NULL;
HWND g_hWndTreeCtrl=NULL;
HWND g_hWndEdit=NULL;
HWND g_hWndList=NULL;
WCHAR g_szFile[260] = { 0 };
HIMAGELIST g_hSmallImgLst = NULL;
HIMAGELIST g_hImgLst = NULL;

void InitView(HWND hwndDlg);
void OpenTxt(HWND hwnd);
void SaveTxt();
void SaveAs(HWND hwnd);
void DelChildItem(HTREEITEM hItem);
CString GetFullPath(HTREEITEM hItem);
void InsertFile(CString strPath, HTREEITEM hParent);
void FindInsertFile(CString strPath, HTREEITEM hParent);
void InitView(HWND hwndDlg);

void HandleMenuMessage(HWND hwnd,      // handle to window
    UINT uMsg,      // message identifier
    WPARAM wParam,  // first message parameter
    LPARAM lParam);

//Dialog callback
INT_PTR CALLBACK AboutDialogProc(
    HWND hwndDlg,  // handle to dialog box
    UINT uMsg,     // message
    WPARAM wParam, // first message parameter
    LPARAM lParam  // second message parameter
    )
{

    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
         InitView( hwndDlg);
    }
    break;
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDCANCEL)
        {
            DestroyWindow(hwndDlg);
            PostQuitMessage(0);
            return (INT_PTR)TRUE;
        }

    }
    break;
    case WM_NOTIFY:
    {//复杂控件消息通过wm_notify传递
        LPNMTREEVIEW pItem = (LPNMTREEVIEW)lParam;
        if (pItem->hdr.hwndFrom == g_hWndTreeCtrl)
        {
            if (pItem->hdr.code == TVN_ITEMEXPANDED || pItem->hdr.code == TVN_SELCHANGED)
            {
                //删除所有子节点
                //清空列表
                ListView_DeleteAllItems(g_hWndList);
                DelChildItem(pItem->itemNew.hItem);

                //通过节点句柄获取到该节点的全路径
                CString strFullPath = GetFullPath(pItem->itemNew.hItem);
                if (strFullPath != _T(""))
                {
                    SetWindowText(g_hWndEdit, strFullPath);
                    FindInsertFile(strFullPath, pItem->itemNew.hItem);
                }
                else
                {
                    SetWindowText(g_hWndEdit, _T("此电脑"));
                    //遍历驱动器
                    TCHAR* pDriver = NULL;
                    TCHAR szBuf[MAX_PATH] = { 0 };
                    GetLogicalDriveStrings(MAX_PATH, szBuf);
                    pDriver = szBuf;
                    int nItem = 0;
                    while (*pDriver)
                    {
                        LVITEM lvI = { 0 };
                        lvI.pszText = pDriver;
                        lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
                        lvI.iImage = 8;
                        lvI.iItem = nItem++;
                        ListView_InsertItem(g_hWndList, &lvI);

                        pDriver += _tcslen(pDriver) + 1;
                    }
                }
            }
        }
    }
    break;
    }
    return 0;
}

//Mian Window  callback function
LRESULT CALLBACK MainWndProc(
    HWND hwnd,      // handle to window
    UINT uMsg,      // message identifier
    WPARAM wParam,  // first message parameter
    LPARAM lParam   // second message parameter
    )
{

    switch (uMsg)
    {
    case WM_CREATE:
    {
         g_hwndEdit = CreateWindow(
            L"EDIT",     // predefined class 
            NULL,       // no window title 
            WS_CHILD | WS_VISIBLE | WS_VSCROLL |
            ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            0, 0, 0, 0, // set size in WM_SIZE message 
            hwnd,       // parent window 
            (HMENU)ID_EDITCHILD, // edit control ID 
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
            NULL);                // pointer not needed 

        return 0;
    }

    case WM_DESTROY:
    {
        // exit code
        PostQuitMessage(0);
        return 0;
    }
    case WM_SIZE:
    {
        //  Make the edit control the size of the window's 
        MoveWindow(g_hwndEdit, 0, 0, LOWORD(lParam),// width of client area  
            HIWORD(lParam), TRUE);
        return 0;
    }
    case WM_COMMAND:
    {
        int nType=HIWORD(wParam);
        if (nType == 0)
        {
            //menu message
            HandleMenuMessage(hwnd,uMsg,wParam,lParam);
          
        }
        else  if (nType==1)
        {
            //accelerator message
        }
        else
        {
            //contronl message


        }
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//int CALLBACK _tWinMain(
//    HINSTANCE hInstance,      // handle to current instance
//    HINSTANCE hPrevInstance,  // handle to previous instance
//    LPSTR lpCmdLine,          // command line
//    int nCmdShow              // show state
//    )

    int CALLBACK _tWinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPTSTR     lpCmdLine,
    _In_ int       nCmdShow
    )
{
    
    WNDCLASSEX wcx = {0};
    wcx.cbSize = sizeof(wcx);          // size of structure 
    wcx.style = CS_HREDRAW |
        CS_VREDRAW;                    // redraw if size changes 
    wcx.lpfnWndProc = MainWndProc;     // points to window procedure 
    wcx.cbClsExtra = 0;                // no extra class memory 
    wcx.cbWndExtra = 0;                // no extra window memory 
    wcx.hInstance = hInstance;         // handle to instance 
    wcx.hIcon = LoadIcon(NULL,
        IDI_APPLICATION);              // predefined app. icon 
    wcx.hCursor = LoadCursor(NULL,
        IDC_ARROW);                    // predefined arrow 
    wcx.hbrBackground = (HBRUSH)GetStockObject(
        WHITE_BRUSH);                  // white background brush 
    wcx.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);    // name of menu resource 
    wcx.lpszClassName = L"MainWindowClass";  // name of window class 


    RegisterClassEx(&wcx);

    //create the main window
 HWND  hwnd=CreateWindow(L"MainWindowClass", L"Notepad++", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);

 //show window
 ShowWindow(hwnd, nCmdShow);
 UpdateWindow(hwnd);

 // message loop
 MSG msg;
 while (GetMessage(&msg, NULL, 0, 0)>0)
 {
    // if (TranslateAccelerator(hwnd,))
     TranslateMessage(&msg);
     DispatchMessage(&msg);
 }
}

void HandleMenuMessage(HWND hwnd,      // handle to window
                        UINT uMsg,      // message identifier
                        WPARAM wParam,  // first message parameter
                        LPARAM lParam)
{

    switch (LOWORD(wParam))
    {
    case ID_OPEN:
    {
        SendMessage(g_hwndEdit, EM_SETSEL, 0, -1);
        SendMessage(g_hwndEdit, WM_CLEAR, 0, 0);
        OpenTxt(hwnd);
    }
    break;
    case ID_SAVE:
    {
        void SaveTxt();
    }
    break;
    case ID_SAVEAS:
    {
        SaveAs(hwnd);
    }
    break;
    case ID_ABOUT:
    {
        DialogBox((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutDialogProc);
    }
    break;
    case ID_CUT:
    {
        SendMessage(g_hwndEdit, WM_CUT, 0, 0);
    }
    break;
    case ID_COPY:
    {
        SendMessage(g_hwndEdit, WM_COPY, 0, 0);
    }
    break;
    case ID_PASTE:
    {
        SendMessage(g_hwndEdit, WM_PASTE, 0, 0);
    }
    break;
    case ID_CLEAR:
    {
        SendMessage(g_hwndEdit, WM_CLEAR, 0, 0);
    }
    break;
    case ID_SELALL:
    {
        SendMessage(g_hwndEdit, EM_SETSEL, 0, -1);
    }
    break;
    }
}

void OpenTxt(HWND hwnd)
{
    OPENFILENAME ofn = { 0 };       // common dialog box structure
    WCHAR szFile[260] = { 0 };       // buffer for file name
    HANDLE hf=NULL;              // file handle
    int bResult = 0;
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 

    if (GetOpenFileName(&ofn) == TRUE)
        hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
        0, (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);

    //init file path
    memset(g_szFile, 0, 260);
    memcpy(g_szFile, szFile, 260);

    char szBuf[256] = { 0 };
    DWORD nBytesToRead = 255;
    DWORD nBytesRead = 0;

    while (1)
    {
        memset(szBuf, 0, 256);
        bResult = ReadFile(hf, szBuf, nBytesToRead, &nBytesRead, NULL);

        SendMessage(g_hwndEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
        SendMessageA(g_hwndEdit, EM_REPLACESEL, 0, (LPARAM)(szBuf));
        // Check for end of file. 
        if (bResult &&  nBytesRead == 0)
        {
            // we're at the end of the file 
            CloseHandle(hf);
            break;
        }

    }
}

void SaveTxt()
{
    int nLength = GetWindowTextLength(g_hwndEdit);

    char* szbuf = (char*)malloc(nLength);
    GetWindowTextA(g_hwndEdit, szbuf, nLength);

    HANDLE hf = CreateFile(g_szFile, GENERIC_WRITE, 0, (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);

    DWORD NumberOfBytesWritten = 0;
    WriteFile(hf, szbuf, nLength, &NumberOfBytesWritten, NULL);
    CloseHandle(hf);
    free(szbuf);
}

void SaveAs(HWND hwnd)
{

    OPENFILENAME Ofn = { 0 };
    ZeroMemory(&Ofn, sizeof(OPENFILENAME));
    WCHAR szFile[260] = { 0 };

    Ofn.lStructSize = sizeof(OPENFILENAME);
    Ofn.hwndOwner = hwnd;
    Ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    Ofn.lpstrFile = szFile;
    Ofn.nMaxFile = sizeof(szFile);
    Ofn.lpstrFileTitle = NULL;
    Ofn.nMaxFileTitle = 0;
    Ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
    //Ofn.lpstrTitle = szTitle;

    GetSaveFileName(&Ofn);


    HANDLE hf = CreateFile(Ofn.lpstrFile, GENERIC_WRITE, 0, (LPSECURITY_ATTRIBUTES)NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);
    int nLength = GetWindowTextLength(g_hwndEdit);

    char* szbuf = (char*)malloc(nLength);
    GetWindowTextA(g_hwndEdit, szbuf, nLength);
    DWORD NumberOfBytesWritten = 0;
    WriteFile(hf, szbuf, nLength, &NumberOfBytesWritten, NULL);
    CloseHandle(hf);
    free(szbuf);

}

void InitView(HWND hwndDlg)
{
    //设置树控件属性
    g_dlg = hwndDlg;
    g_hWndTreeCtrl = GetDlgItem(hwndDlg, IDC_TREE1);
    g_hWndEdit = GetDlgItem(hwndDlg, IDC_EDIT1);
    g_hWndList = GetDlgItem(hwndDlg, IDC_LIST1);
    SetWindowLong(g_hWndTreeCtrl, GWL_STYLE, GetWindowLong(g_hWndTreeCtrl, GWL_STYLE) | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);

    //获得系统imagelist
    SHFILEINFOA sfi = { 0 };
    HIMAGELIST hSmallIconLst = (HIMAGELIST)SHGetFileInfoA(NULL,
        FILE_ATTRIBUTE_NORMAL,
        &sfi,
        sizeof(SHFILEINFOA),
        SHGFI_SMALLICON | SHGFI_SYSICONINDEX);

    HIMAGELIST hIconLst = (HIMAGELIST)SHGetFileInfoA(NULL,
        FILE_ATTRIBUTE_NORMAL,
        &sfi,
        sizeof(SHFILEINFOA),
        SHGFI_ICON | SHGFI_SYSICONINDEX);

    //发送消息设置imagelist
    SendMessage(g_hWndTreeCtrl, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hSmallIconLst);
    SendMessage(g_hWndList, LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM)hIconLst);

    //将此电脑根节点插入树
    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = NULL;
    tvInsert.hInsertAfter = NULL;
    tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvInsert.item.iImage = 15;
    tvInsert.item.iSelectedImage = 15;
    tvInsert.item.pszText = _T("此电脑");
    HTREEITEM hMyCom = (HTREEITEM)::SendDlgItemMessage(hwndDlg, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);

    SetWindowText(g_hWndEdit, _T("此电脑"));



    //遍历驱动器
    TCHAR* pDriver = NULL;
    TCHAR szBuf[MAX_PATH] = { 0 };
    GetLogicalDriveStrings(MAX_PATH, szBuf);
    pDriver = szBuf;
    int nItem = 0;
    while (*pDriver)
    {
        //combox
        SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, NULL, (LPARAM)pDriver);

        //将驱动器加入树
        TVINSERTSTRUCT tvInsert;
        tvInsert.hParent = hMyCom;
        tvInsert.hInsertAfter = NULL;
        tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvInsert.item.iImage = 8;
        tvInsert.item.iSelectedImage = 8;
        tvInsert.item.pszText = pDriver;
        HTREEITEM hParent = (HTREEITEM)::SendDlgItemMessage(hwndDlg, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);

        LVITEM lvI = { 0 };
        lvI.pszText = pDriver;
        lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
        lvI.iImage = 8;
        lvI.iItem = nItem++;
        ListView_InsertItem(g_hWndList, &lvI);


        CString strDriver = pDriver;
        strDriver += _T("*");
        InsertFile(strDriver, hParent);

        pDriver += _tcslen(pDriver) + 1;
    }
}
void DelChildItem(HTREEITEM hItem)
{

    if (hItem == TreeView_GetRoot(g_hWndTreeCtrl))
    {
        return;
    }
    HTREEITEM hChild = 0;
    while (hChild = TreeView_GetChild(g_hWndTreeCtrl, hItem))
    {
        TreeView_DeleteItem(g_hWndTreeCtrl, hChild);
    }
}

CString GetFullPath(HTREEITEM hItem)
{
    CString strTemp;
    CString strReturn = _T("");

    HTREEITEM hRoot = TreeView_GetRoot(g_hWndTreeCtrl);
    while (hItem != hRoot)
    {
        //获得节点的字符串
        TCHAR szName[MAX_PATH] = { 0 };
        TVITEM tvitem;
        tvitem.hItem = hItem;
        tvitem.cchTextMax = MAX_PATH;
        tvitem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvitem.pszText = szName;

        TreeView_GetItem(g_hWndTreeCtrl, &tvitem);
        strTemp = szName;
        if (strTemp.Right(1) != _T("\\"))
            strTemp += _T("\\");
        strReturn = strTemp + strReturn;

        //获得该节点父节点的句柄
        hItem = TreeView_GetParent(g_hWndTreeCtrl, hItem);
    }
    return strReturn;
}

void InsertFile(CString strPath, HTREEITEM hParent)
{
    //插入下一层
    WIN32_FIND_DATA wfd = { 0 };
    HANDLE hFind = FindFirstFile(strPath, &wfd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return;
    }
    BOOL bHasFile = FALSE;
    do
    {
        if (!_tcscmp(_T("."), wfd.cFileName) || !_tcscmp(_T(".."), wfd.cFileName))
        {
            continue;
        }
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
        {
            continue;
        }
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            bHasFile = TRUE;
    } while (FindNextFile(hFind, &wfd));

    if (bHasFile)
    {
        TVINSERTSTRUCT tvInsert;
        tvInsert.hParent = hParent;
        tvInsert.hInsertAfter = NULL;
        tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvInsert.item.iImage = 0;
        tvInsert.item.iSelectedImage = 0;
        tvInsert.item.pszText = wfd.cFileName;
        HTREEITEM hDir = (HTREEITEM)::SendDlgItemMessage(g_dlg, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
    }

    FindClose(hFind);
}

void FindInsertFile(CString strPath, HTREEITEM hParent)
{
    if (strPath.Right(1) != _T("\\"))
        strPath += _T("\\");
    CString strFullPath = strPath + _T("*");

    WIN32_FIND_DATA wfd = { 0 };
    HANDLE hFind = FindFirstFile(strFullPath, &wfd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return;
    }
    int nItem = 0;
    do
    {
        if (!_tcscmp(_T("."), wfd.cFileName) || !_tcscmp(_T(".."), wfd.cFileName))
        {
            continue;
        }
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
        {
            continue;
        }
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //将文件夹插入树
            TVINSERTSTRUCT tvInsert;
            tvInsert.hParent = hParent;
            tvInsert.hInsertAfter = NULL;
            tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvInsert.item.iImage = 4;
            tvInsert.item.iSelectedImage = 4;
            tvInsert.item.pszText = wfd.cFileName;
            HTREEITEM hDir = (HTREEITEM)::SendDlgItemMessage(g_dlg, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);

            //将文件插入list
            LVITEM lvI = { 0 };
            lvI.pszText = wfd.cFileName;
            lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
            lvI.iImage = 4;
            lvI.iItem = nItem++;
            ListView_InsertItem(g_hWndList, &lvI);

            CString strInsertPath = strPath + wfd.cFileName + _T("\\*");
            InsertFile(strInsertPath, hDir);
        }
        else
        {

            LVITEM lvI = { 0 };
            lvI.pszText = wfd.cFileName;
            lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
            lvI.iImage = 0;
            lvI.iItem = nItem++;
            ListView_InsertItem(g_hWndList, &lvI);
        }

    } while (FindNextFile(hFind, &wfd));
    FindClose(hFind);
}

