#include "DevServer.h"

#include "globals.h"
#include <efsw/efsw.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <future>

void UpdateListener::handleFileAction(efsw::WatchID watchid, const std::string& dir,
    const std::string& filename, efsw::Action action,
    std::string oldFilename)
{
    build(options);
}

DevServer::DevServer(Options options)
{
    build(options);

    listener.options = options;

    for (const auto& sub_dir : std::filesystem::directory_iterator(std::filesystem::current_path().string())) {
        std::string sub_dir_name = sub_dir.path().stem().string();
        if (sub_dir_name != options.out_dir) {
            file_watcher.addWatch(sub_dir.path().string(), &listener, true);
        }
    }

    file_watcher.watch();

    web_server.set_mount_point("/", options.out_dir);

    fmt::print("Started dev server\n");

    web_server.listen("0.0.0.0", 3000);
}