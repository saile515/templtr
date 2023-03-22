#include "build.h"
#include <efsw/efsw.hpp>
#include <string>

class UpdateListener : public efsw::FileWatchListener {
public:
    std::string out_dir;

    void handleFileAction(efsw::WatchID watchid, const std::string& dir,
        const std::string& filename, efsw::Action action,
        std::string oldFilename) override;
};

class DevServer {
private:
    efsw::FileWatcher file_watcher;
    UpdateListener listener;

public:
    DevServer(std::string outdir);
};