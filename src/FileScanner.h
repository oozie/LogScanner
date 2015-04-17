/*
 * FileScanner.h
 *
 *  Created on: Apr 12, 2015
 *      Author: oozie
 */

#ifndef FILESCANNER_H_
#define FILESCANNER_H_

#include <sys/inotify.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>

#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>

// TODO: inherit from a scanner interface.
class FileScanner {

public:
    FileScanner(std::string filename);

    void scan();

private:
    std::string filename_;
    char *dirname_;
    int filewatch_qid_ = -1;
    int dirwatch_qid_ = -1;
    int file_ret_ = -1;
;
    bool shutdown_ = false;
    void handleCreateEvent();
    void reopenStream();
    std::thread reopenStreamTask_;

    std::ifstream logfile_;
};

#endif /* FILESCANNER_H_ */
