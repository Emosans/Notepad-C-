#include <windows.h>
#include <iostream>
#include<commdlg.h>
#include<fstream>
#include <string>
#include<wingdi.h>
using namespace std;

//function definitions
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void addMenus(HWND);

//global variables
HMENU hMenu;
HMENU hFileSubMenu;
HWND hEdit;
HWND hwnd;
HFONT hFont;
HBRUSH hBrush;
RECT rect;

void addMenus(HWND hwnd){
    //main menu
    hMenu = CreateMenu();

    //submenu for file
    hFileSubMenu = CreateMenu(); //creates a submenu for the file menu
    AppendMenu(hFileSubMenu, MF_STRING, 4, "New");
    AppendMenu(hFileSubMenu, MF_STRING, 5, "Open");
    AppendMenu(hFileSubMenu, MF_STRING, 6, "Save");
    AppendMenu(hFileSubMenu, MF_STRING, 7, "Exit");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileSubMenu, "File");

    AppendMenu(hMenu, MF_STRING, 2, "Edit");

    hBrush = CreateSolidBrush(RGB(0,0,0));
    HDC hdc = (HDC)hBrush; //handle device context
    SetTextColor(hdc,RGB(255,255,255)); //set text color to black
    SetBkColor(hdc,RGB(0,0,0)); //set background color to light blue 
    //set menu to window
    SetMenu(hwnd,hMenu); //sets the menu to the window
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg) {
        case WM_CREATE: {
            hEdit = CreateWindow("EDIT", "", 
                                 WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
                                 0, 0, 885, 885, 
                                 hwnd, NULL, NULL, NULL);

            hFont = CreateFont(
                40,0,0,0,
                FW_MEDIUM,FALSE,FALSE,FALSE,
                FALSE,OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                FF_SCRIPT,
                "Segoe UI"
            );


            //create a new font {size,width,escapement,orientation,weight,italic,underline,strikeout,charset,outprecision,clipprecision,quality,pitchandfamily,fontname}
            SendMessage(hEdit,WM_SETFONT,(WPARAM)hFont,TRUE);

            hBrush = CreateSolidBrush(RGB(0,0,0)); //crete brush with light blue color
            break;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam; //handle device context
            SetTextColor(hdc,RGB(255,255,255)); //set text color to black
            SetBkColor(hdc,RGB(0,0,0)); //set background color to light blue
            return (INT_PTR)hBrush; //return the brush to paint the background
        }

        case WM_SIZE: {
            // Resize the EDIT control when the parent window is resized
            GetClientRect(hwnd, &rect); // get parent window size
            SetWindowPos(hEdit, NULL, 0, 0, rect.right, rect.bottom, SWP_NOZORDER); //resize edit window
            break;
        }


         case WM_COMMAND: {
            switch (wParam) {
                case 4:
                    SetWindowText(hEdit, "");
                    break;
                case 5: {
                    // Implement File Open Logic
                    OPENFILENAME ofn; //creates a dialog box
                    char szOpenFile[260]; //buffer for file name

                    ZeroMemory(&ofn, sizeof(ofn)); //initializes the ofn structure to zero
                    ofn.lStructSize = sizeof(ofn); //sets the size of the ofn structure
                    ofn.hwndOwner = hwnd; //sets the owner of the dialog box to the window handle
                    ofn.lpstrFile = new char[260]; //allocates memory for the file name
                    ofn.lpstrFile[0] = '\0'; //sets the first character of the file name to null
                    ofn.nMaxFile = 260; //sets the maximum length of the file name to 260
                    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0"; //sets the filter for the file type
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; //sets the flags for the file type
                    
                    if(GetOpenFileName(&ofn) == TRUE){ //displays the dialog box
                        ifstream inFile(ofn.lpstrFile); //opens the file
                        string buffer;
                        buffer.assign((istreambuf_iterator<char>(inFile)), //reads the file
                                      (istreambuf_iterator<char>()));
                        SetWindowText(hEdit, buffer.c_str()); //sets the text in the edit control to the text in the file
                        inFile.close();
                    }
                    break;
                }
                case 6: {
                    // Implement File Save Logic
                    OPENFILENAME ofn;       // Common dialog box structure
                    char szFile[260];        // Buffer for file name

                    // Initialize OPENFILENAME
                    ZeroMemory(&ofn, sizeof(ofn)); //initializes the ofn structure to zero  
                    ofn.lStructSize = sizeof(ofn); //sets the size of the ofn structure
                    ofn.hwndOwner = hwnd; //sets the owner of the dialog box to the window handle
                    ofn.lpstrFile = szFile; //sets the file name to the buffer
                    ofn.lpstrFile[0] = '\0'; //sets the first character of the file name to null
                    ofn.nMaxFile = sizeof(szFile); //sets the maximum length of the file name to the size of the buffer
                    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0"; //sets the filter for the file type
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; //sets the flags for the file type

                    // Display the Save As dialog box
                    if (GetSaveFileName(&ofn) == TRUE) {
                        // Get the text from the EDIT control
                        int len = GetWindowTextLength(hEdit); //gets the length of the text in the edit control
                        char* buffer = new char[len + 1]; //allocates memory for the text
                        GetWindowText(hEdit, buffer, len + 1); //gets the text from the edit control

                        // Write the text to the selected file
                        ofstream outFile(ofn.lpstrFile); //opens the file
                        outFile << buffer; //writes the text to the file
                        outFile.close(); //closes the file

                        delete[] buffer; //deallocates the memory for the text

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
            PostQuitMessage(0); //posts a quit message to the message queue
            break;
    }
    return DefWindowProcW(hwnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSW windowClass = {0};

    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL,IDC_HAND);
    windowClass.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    windowClass.lpszClassName = L"WindowClass";
    windowClass.lpfnWndProc = WindowProc; //sets the window procedure to the WindowProc function

    if(!RegisterClassW(&windowClass)){ //registers the window class
        return -1;
    }
    hwnd =CreateWindowW(L"WindowClass",L"My Window",WS_OVERLAPPEDWINDOW|WS_VISIBLE,100,100,900,900,NULL,NULL,NULL,NULL);
    addMenus(hwnd);

    MSG msg = {0};

    while(GetMessageW(&msg,NULL,0,0)){ //gets the message from the message queue
        TranslateMessage(&msg); //translates the message
        DispatchMessageW(&msg); //dispatches the message to the window procedure
    }
    return 0;
}