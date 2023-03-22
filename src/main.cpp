#include "DevServer.h"
#include "build.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <filesystem>
#include <fmt/core.h>

static void wrong_input()
{
    fmt::print("\nPlease input a valid command. Available commands are as follows:\n\n\tinit - Init a templtr project\n\tbuild <outdir> - Build project files to specified directory\n\tdev <outdir> - Start a dev server that builds to outdir on change\n\n");
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

    if (std::strcmp(argv[1], "init") == 0) {
        init();
    } else if (std::strcmp(argv[1], "build") == 0) {
        if (argc == 3) {
            if (build(argv[2]) < 0) {
                fmt::print("Build failed!\n");
            }
        } else {
            wrong_input();
        }
    } else if (std::strcmp(argv[1], "dev") == 0) {
        if (argc == 3) {
            DevServer dev_server(argv[2]);
            signal(SIGINT, exit);

            while (true) {
                Sleep(1);
            }
        } else {
            wrong_input();
        }
    } else {
        wrong_input();
        return -1;
    }

    return 0;
}
