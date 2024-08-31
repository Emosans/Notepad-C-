#include <windows.h>
#include <iostream>
#include<commdlg.h>
#include<fstream>
#include <string>
using namespace std;

//function definitions
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void addMenus(HWND);

//global variables
HMENU hMenu;
HMENU hFileSubMenu;
HWND hEdit;
HWND hwnd;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg) {
        case WM_CREATE: {
            hEdit = CreateWindow("EDIT", "", 
                                 WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
                                 0, 0, 880, 880, 
                                 hwnd, NULL, NULL, NULL);
            break;
        }

         case WM_COMMAND: {
            switch (wParam) {
                case 4:
                    SetWindowText(hEdit, "");
                    break;
                case 5: {
                    // Implement File Open Logic
                    break;
                }
                case 6: {
                    // Implement File Save Logic
                    OPENFILENAME ofn;       // Common dialog box structure
                    char szFile[260];        // Buffer for file name

                    // Initialize OPENFILENAME
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = szFile;
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                    // Display the Save As dialog box
                    if (GetSaveFileName(&ofn) == TRUE) {
                        // Get the text from the EDIT control
                        int len = GetWindowTextLength(hEdit);
                        char* buffer = new char[len + 1];
                        GetWindowText(hEdit, buffer, len + 1);

                        // Write the text to the selected file
                        ofstream outFile(ofn.lpstrFile);
                        outFile << buffer;
                        outFile.close();

                        delete[] buffer;
                    }
                    PostQuitMessage(0);
                    break;
                }
                case 7:
                    PostQuitMessage(0);
                    break;
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProcW(hwnd,uMsg,wParam,lParam);
}

void addMenus(HWND hwnd){
    //main menu
    hMenu = CreateMenu();

    //submenu for file
    hFileSubMenu = CreateMenu();
    AppendMenu(hFileSubMenu, MF_STRING, 4, "New");
    AppendMenu(hFileSubMenu, MF_STRING, 5, "Open");
    AppendMenu(hFileSubMenu, MF_STRING, 6, "Save");
    AppendMenu(hFileSubMenu, MF_STRING, 7, "Exit");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileSubMenu, "File");

    AppendMenu(hMenu, MF_STRING, 2, "Edit");
    AppendMenu(hMenu, MF_STRING, 3, "Help");
    //set menu to window
    SetMenu(hwnd,hMenu);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSW windowClass = {0};

    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL,IDC_HAND);
    windowClass.lpszClassName = L"WindowClass";
    windowClass.lpfnWndProc = WindowProc;

    if(!RegisterClassW(&windowClass)){
        return -1;
    }
    hwnd =CreateWindowW(L"WindowClass",L"My Window",WS_OVERLAPPEDWINDOW|WS_VISIBLE,100,100,900,900,NULL,NULL,NULL,NULL);
    addMenus(hwnd);

    MSG msg = {0};

    while(GetMessageW(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}