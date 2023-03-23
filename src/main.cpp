#include "DevServer.h"
#include "build.h"
#include "globals.h"

#include <chrono>
#include <filesystem>
#include <fmt/core.h>
#include <thread>

static void wrong_input()
{
    fmt::print("\nPlease input a valid command. Available commands are as follows:\n\n\tinit - Init a templtr project\n\tbuild - Build project files\n\tdev - Start a dev server that builds on source change\n\n");
}

static int init()
{
    fmt::print("Initializing project...\n");

    if (!std::filesystem::exists("pages")) {
        std::filesystem::create_directory("pages");
    }

    if (!std::filesystem::exists("components")) {
        std::filesystem::create_directory("components");
    }

    if (!std::filesystem::exists("content")) {
        std::filesystem::create_directory("content");
    }

    if (!std::filesystem::exists("public")) {
        std::filesystem::create_directory("public");
    }

    fmt::print("Project initialized!\n");

    return 0;
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        wrong_input();
        return -1;
    }

    Options options;

    if (argc >= 4) {
        for (int i = 2; i < argc; ++i) {
            if (std::strcmp(argv[i], "-o") == 0) {
                options.out_dir = argv[i + 1];
            } else if (std::strcmp(argv[i], "-b") == 0) {
                options.base = argv[i + 1];
            } else {
                continue;
            }

            ++i;
        }
    }

    if (std::strcmp(argv[1], "init") == 0) {
        init();
    } else if (std::strcmp(argv[1], "build") == 0) {
        if (argc >= 2) {
            if (build(options) < 0) {
                fmt::print("Build failed!\n");
            }
        } else {
            wrong_input();
            return -1;
        }
    } else if (std::strcmp(argv[1], "dev") == 0) {
        if (argc >= 2) {
            DevServer dev_server(options);
            signal(SIGINT, exit);

            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        } else {
            wrong_input();
            return -1;
        }
    } else {
        wrong_input();
        return -1;
    }

    return 0;
}
