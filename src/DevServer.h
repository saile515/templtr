#pragma once

#include "build.h"
#include "globals.h"
#include <efsw/efsw.hpp>
#include <httplib.h>
#include <string>

class UpdateListener : public efsw::FileWatchListener {
public:
    Options options;

    void handleFileAction(efsw::WatchID watchid, const std::string& dir,
        const std::string& filename, efsw::Action action,
        std::string oldFilename) override;
};

class DevServer {
private:
    efsw::FileWatcher file_watcher;
    UpdateListener listener;
    httplib::Server web_server;

public:
    DevServer(Options options);
};