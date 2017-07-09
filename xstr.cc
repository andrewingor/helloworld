#ifndef Win64
#define Win64

#include <iostream>
#include <string>

#include "xstr.hpp"

using namespace std;

int main ()
{
    hide::line Args;
    getline (cin, Args);
    Args.encode();
    cout << Args << endl;
    Args .decode();
    cout << Args << endl;
}

#endif