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

    int dirwatch_qid = inotify_init();

    int ret = inotify_add_watch(dirwatch_qid, dirname_, IN_CREATE);
    if (ret) {
        // TODO: Handle errors.
    }

    filewatch_qid_ = inotify_init();
    file_ret_ = inotify_add_watch(filewatch_qid_, filename_.c_str(), IN_MODIFY);
    if (file_ret_) {
        // TODO: Handle errors.
    }
    std::cerr << "watching dir" << dirname_ << std::endl;
    while(!shutdown_) {
        if (read(dirwatch_qid, event_buffer, IEVENT_BUF_LEN)) {
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
    reopenStreamTask_ = std::thread(&FileScanner::handleCreateEvent, this);
    char event_buffer[IEVENT_BUF_LEN];

    // TODO: Scan and populate a rate counter.
    std::cerr << "scanning " << filename_ << std::endl;
    // TODO: tailf the input stream
    std::cerr << "watching " << filename_ << std::endl;
    while(!shutdown_) {
        read(filewatch_qid_, event_buffer, IEVENT_BUF_LEN);
        std::cerr << "file modified" << std::endl;
    };
    reopenStreamTask_.join();
}

void FileScanner::reopenStream() {
    if (logfile_.is_open()) {
        logfile_.close();
    }
    logfile_.open(filename_.c_str(), std::ios::in);
    logfile_.seekg(logfile_.end);
}
