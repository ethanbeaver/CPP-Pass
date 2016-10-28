#include <iostream>
#include <tclap/CmdLine.h>
#include "disk_storage.h"

using namespace std;

int main(int argc, char** argv) {
    try {
        TCLAP::CmdLine cmd("CPP Password Manager", ' ', "0.9");
        TCLAP::ValueArg<std::string> KeyArg("k", "key", "Location of keyfile", false, "/dev/rand", "string");
        TCLAP::ValueArg<std::string> PassArg("p", "pass", "Location of password file", false, "/dev/rand", "string");
        TCLAP::ValueArg<std::string> GenArg("g", "gen", "Generate key file at specified location", false, "~/CPPPasskey.bin", "string")
        cmd.add(KeyArg);
        cmd.add(PassArg);
        cmd.add(GenArg);

        // Parse the argv array.
        cmd.parse(argc, (const char * const * ) argv);

        // Get the value parsed by each arg.
        if(GenArg.isSet()) {
            string keyGenPath = GenArg.getValue();
        }else {
            string keyPath = KeyArg.getValue();
            string passPath = PassArg.getValue();

        }

        cout << "KeyArg = " << keyPath << endl;
        cout << "PassArg = " << passPath << endl;
        cout << "GenArg = " << keyGenPath << endl;
    } catch(TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }


    cout << "Hello CPP-Pass" << endl;

    return 0;
}