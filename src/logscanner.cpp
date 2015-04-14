#include <iostream>
#include <string>

#include "FileScanner.h"

using namespace std;

int main(int argc, char **argv) {

    // TODO: Set up logging.
    cerr << "LogScanner prototype" << endl;
    // TODO: check if a directory.
    // TODO: Parse args.
    FileScanner linescanner(argv[1]);
    // TODO: Extract config.
    linescanner.scan();
    return 0;
}
