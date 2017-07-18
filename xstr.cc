#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include "xstr.hpp"

using namespace std;

int main (int argc, char** argv) {
try {
static hide::line Args;

    if (argc > 1) {
        string dir(argv[1]);
        string name(argv[2]);
        ifstream file(name.c_str(), ios::in);
        if( !file.is_open()) throw runtime_error("file error");
        while (getline (file, Args)) {
            if (dir == "en") Args   .
            encode();
            else Args   .
            decode();
            cout << Args << endl;
        }
        throw runtime_error("=eof<<<<<<<<<<<<<<<<<<<<<<<<<");
    }

    getline (cin, Args);
    Args                    .
        encode();
    cout << Args << endl;
    Args                    .
        decode();
    cout << Args << endl;

} catch (exception& e) {
    cerr << e.what() << endl;
}}