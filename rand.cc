#include <iostream>
#include <string>
#include <random>

using namespace std;

int main ()
{
    string input;

    random_device   r;
    default_random_engine re(r());
    uniform_int_distribution<int> uniform_dist(0,50);

    while(true) {
        int i = uniform_dist(re); 
        cout << i << endl;
        getline (cin, input);
    }
    return 0;
}