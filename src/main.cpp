#include <iostream>
#include <tclap/CmdLine.h>
#include "include/disk_storage.h"

using namespace std;

int main(int argc, char** argv) {
    try {
        TCLAP::CmdLine cmd("CPP Password Manager", ' ', "0.9");
        TCLAP::ValueArg<std::string> KeyArg("k", "key", "Location of keyfile", false, "/dev/rand", "string");
        TCLAP::ValueArg<std::string> PassArg("p", "pass", "Location of password file", false, "/dev/rand", "string");
        TCLAP::ValueArg<std::string> GenArg("g", "gen", "Generate key file at specified location", false, "~/CPPPasskey.bin", "string");
        cmd.add(KeyArg);
        cmd.add(PassArg);
        cmd.add(GenArg);

        // Parse the argv array.
        cmd.parse(argc, (const char * const * ) argv);

        diskStorage d;
        // Get the value parsed by each arg, depending on which are set.
        if(GenArg.isSet()) {
            string keyGenPath = GenArg.getValue();
            d.genKey(keyGenPath);
        }else if(PassArg.isSet() && KeyArg.isSet()){
            string keyPath = KeyArg.getValue();
            string passPath = PassArg.getValue();

            if(d.readFromFile(keyPath, passPath)) {
                d.Menu();
                d.writeToFile(passPath);
            }else{
                cerr << "Operations failed. No changes were made" << endl;
            }
        }
        else{
            cerr << "Invalid Parameters!\n";
        }

    } catch(TCLAP::ArgException &e) {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    }

    return 0;
}