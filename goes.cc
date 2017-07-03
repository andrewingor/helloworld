#ifndef Win64
#define Win64

#include <iostream>
#include <string>
#include <stdexcept>

#include <windows.h>

using namespace std;

int main (int argc, char** argv) {
try {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    string args("cmd.exe /c cmd.exe");

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    ZeroMemory( &pi, sizeof(pi) );

    if( !CreateProcess ( NULL,   // No module name (use command line)
        (LPSTR)args.c_str(),        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,              // Hidden
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
    cerr << e.what() << endl;
    return -1;
}}

#endif