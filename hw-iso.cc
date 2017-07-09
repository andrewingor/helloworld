#ifndef Win64
#define Win64

#include <iostream>
#include <string>

using namespace std;

int main ()
{
    std::string input;
    
    for(;;) {
        cout << "Type 'quit', stupid: " << flush;
        cin >> input;
        if (input.find("quit") != std::string::npos)
        return 0;
    }
    return -1;
}

#endif