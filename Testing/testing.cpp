
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


int main() {

    char list[10] = "hello";
    
    cout << list << endl;

    if ( list[0] == 'h' ) { cout << "this worked!" << endl; }
    else { cout << "this did not work!" << endl; }
    
    return 0;

}