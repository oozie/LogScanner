#include <iostream>
#include <string>

#include "FileScanner.h"

using namespace std;

int main(int argc, char **argv) {

    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    LOG(INFO) << "LogScanner prototype";
    // TODO: check if a directory.
    // TODO: Parse args.
    FileScanner linescanner(argv[1]);
    // TODO: Extract config.
    linescanner.scan();
    return 0;
}
