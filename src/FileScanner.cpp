/*
 * FileScanner.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: oozie
 */

#include "FileScanner.h"

#define MAX_EVENTS 4
#define LEN_NAME 0x100
#define EVENT_SIZE     ( sizeof (struct inotify_event) )
#define IEVENT_BUF_LEN ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))

FileScanner::FileScanner(std::string filename) :
    filename_(filename) {
    dirname_ = dirname(strdup(filename_.c_str()));
}

void FileScanner::handleCreateEvent() {
    char event_buffer[IEVENT_BUF_LEN];

    std::cerr << "watching dir " << dirname_ << std::endl;
    while(!shutdown_) {
        if (read(dirwatch_qid_, event_buffer, IEVENT_BUF_LEN)) {
            reopenStream();
            std::cerr << filename_ << " recreated" << std::endl;
            inotify_rm_watch(filewatch_qid_, file_ret_);
            file_ret_ = inotify_add_watch(filewatch_qid_, filename_.c_str(), IN_MODIFY);
            // TODO: only reopen if it's the monitored filename
        }
    }
    std::cout << "done watching" << std::endl;
}

void FileScanner::scan() {
    char event_buffer[IEVENT_BUF_LEN];
    std::string logline;

    // Allocate watch queues.
    filewatch_qid_ = inotify_init();
    dirwatch_qid_ = inotify_init();
    int dir_ret_ = inotify_add_watch(dirwatch_qid_, dirname_, IN_CREATE);
    if (dir_ret_) {
        // TODO: Handle errors.
    }
    file_ret_ = inotify_add_watch(filewatch_qid_, filename_.c_str(), IN_MODIFY);
    if (file_ret_) {
        // TODO: Handle errors.
    }
    // Open the file.
    reopenStream();
    reopenStreamTask_ = std::thread(&FileScanner::handleCreateEvent, this);

    // TODO: Scan and populate a rate counter.
    std::cerr << "scanning " << filename_ << std::endl;
    int curpos = -1;
    while(!shutdown_) {
        read(filewatch_qid_, event_buffer, IEVENT_BUF_LEN);
        getline(logfile_, logline);
        curpos = logfile_.tellg();
        if (curpos == -1) { // File truncated.
            // TODO: Don't drop first line after truncation.
            reopenStream();
        } else {
            std::cerr << "CUR: " << curpos << " "
                      << "LOGLINE: "<<  logline << std::endl;
        }
    };
    reopenStreamTask_.join();
}

void FileScanner::reopenStream() {
    if (logfile_.is_open()) {
        logfile_.close();
    }
    logfile_.open(filename_.c_str(), std::ios::in);
    logfile_.seekg(0, logfile_.end);
}
