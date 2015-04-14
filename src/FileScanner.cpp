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
    char buffer[IEVENT_BUF_LEN];

    int watch_queue_id = inotify_init();

    int ret = inotify_add_watch(watch_queue_id, dirname_, IN_CREATE);
    if (ret) {
        // TODO: Handle errors.
    }
    std::cerr << "watching " << dirname_ << std::endl;
    while(!shutdown_) {
        if (read(watch_queue_id, buffer, IEVENT_BUF_LEN)) {
            reopenStream();
            // TODO: only reopen if it's the monitored filename
            std::cerr << filename_ << " recreated" << std::endl;
        }
    }
    std::cout << "done watching" << std::endl;
}

void FileScanner::scan() {
    reopenStreamTask_ = std::thread(&FileScanner::handleCreateEvent, this);
    // TODO: Scan and populate a rate counter.
    std::cerr << "scanning " << filename_ << std::endl;
    // TODO: tailf the input stream
    while(true) { };
    reopenStreamTask_.join();
}

void FileScanner::reopenStream() {
    if (logfile_.is_open()) {
        logfile_.close();
    }
    logfile_.open(filename_);
    logfile_.seekg(logfile_.end);
}
