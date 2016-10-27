#include <iostream>

#include <tclap/CmdLine.h>

using namespace std;

int main() {
    TCLAP::CmdLine cmd("Command description message", ' ', "0.9");
    (void) cmd;

    cout << "Hello CPP-Pass" << endl;

    return 0;
}