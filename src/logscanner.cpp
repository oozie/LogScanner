#include <iostream>
#include <string>

#include <sys/stat.h>
#include <gflags/gflags.h>

#include "FileScanner.h"


DEFINE_string(config, "", "JSON configuration of the logscanner");
DEFINE_string(logfile, "", "Config override for scanned log file");

using namespace std;

void parseJsonConfig(string config_file) {
    std::ifstream input_stream;
    if (config_file == "-") {
        // TODO: allow for reading config from stdin
    } else {
        input_stream.open(config_file, std::ifstream::binary);
    }
    Json::Value root;
    Json::Reader reader;

    if (reader.parse(input_stream, root, false)) {
        if (FLAGS_logfile.empty()) {
            FLAGS_logfile = root.get("logfile", "").asString();
        }
    } else {
        LOG(FATAL) << "Error reading config\n" << reader.getFormattedErrorMessages();
    }
    // Validate that the file pointed to for a scan is not a directory.
    struct stat sb;
    if (stat(FLAGS_logfile.c_str(), &sb) == 0) {
        if (S_ISDIR(sb.st_mode)) {
            LOG(FATAL) << FLAGS_logfile << " is a directory";
        }
    } // It's okay if the file does not exist yet; we can wait for it.
}

int main(int argc, char **argv) {

    /*
     * A hack to prevent long, irrelevant --help output from gflags.
     * It's sad: https://github.com/xsmart/gflags/issues/32
     * For those who need all the flags --helpfull still works.
     */
    for (int i=0; i < argc; i++) {
        if(strcmp(argv[i], "--help") == 0) {
            argv[i] = (char *)"--helpshort";
        }
    }

    FLAGS_logtostderr = 1;

    string usage("Scan log lines for regex patterns. Usage:\n\n  ");
    usage += argv[0];
    usage += " --config=<logscan.json>\n";
    google::InitGoogleLogging(argv[0]);
    google::SetUsageMessage(usage);
    google::ParseCommandLineFlags(&argc, &argv, true);

    parseJsonConfig(FLAGS_config);

    if (FLAGS_config.empty()) {
        cerr << google::ProgramUsage() << endl;
        exit(1);
    }

    FileScanner linescanner(FLAGS_logfile);
    linescanner.scan();
    return 0;
}
