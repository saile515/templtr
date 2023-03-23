#include "DevServer.h"

#include <efsw/efsw.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <future>

void UpdateListener::handleFileAction(efsw::WatchID watchid, const std::string& dir,
    const std::string& filename, efsw::Action action,
    std::string oldFilename)
{
    build(out_dir);
}

DevServer::DevServer(std::string out_dir)
{
    listener.out_dir = out_dir;

    for (const auto& sub_dir : std::filesystem::directory_iterator(std::filesystem::current_path().string())) {
        std::string sub_dir_name = sub_dir.path().stem().string();
        if (sub_dir_name != out_dir) {
            file_watcher.addWatch(sub_dir.path().string(), &listener, true);
        }
    }

    file_watcher.watch();

    web_server.set_mount_point("/", out_dir);

    fmt::print("Started dev server\n");

    web_server.listen("0.0.0.0", 3000);
}