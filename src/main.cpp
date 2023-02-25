#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <json/json.h>
#include <fmt/core.h>

void wrong_input()
{
    fmt::print("\nPlease specify a command. Available commands are as follows:\n\n\tinit - Init a templtr project\n\tbuild <outdir> - Build project files to specified directory\n\n");
}

int init()
{
    fmt::print("Initializing project...\n");

    if (!std::filesystem::exists("pages"))
    {
        std::filesystem::create_directory("pages");
    }

    if (!std::filesystem::exists("content"))
    {
        std::filesystem::create_directory("content");
    }

    fmt::print("Project initialized!\n");

    return 0;
}

int build(const char *outdir)
{
    fmt::print("Started build...\n");

    std::chrono::time_point start_time = std::chrono::high_resolution_clock::now();

    for (const auto &page : std::filesystem::directory_iterator("content"))
    {
        std::string page_name = page.path().stem().string();
        fmt::print("Building {}...\n", page_name);

        std::string content_path = fmt::format("content/{}", page_name);

        if (std::filesystem::exists(content_path))
        {
            for (const auto &entry : std::filesystem::directory_iterator(content_path))
            {
                std::string entry_filename = entry.path().string();
                std::string entry_name = entry.path().stem().string();

                std::ifstream entry_file(entry_filename, std::ifstream::binary);
                Json::Value data;
                entry_file >> data;
            }
        }

        fmt::print("Finished building {}!\n", page_name);
    }

    std::chrono::time_point end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration build_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    fmt::print("Build finished in {} milliseconds!\n", build_time.count());

    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        wrong_input();
        return -1;
    }

    if (std::strcmp(argv[1], "init") == 0)
    {
        init();
    }
    else if (std::strcmp(argv[1], "build") == 0)
    {
        if (argc == 3)
        {
            build(argv[2]);
        }
        else
        {
            wrong_input();
        }
    }
    else
    {
        wrong_input();
        return -1;
    }

    return 0;
}