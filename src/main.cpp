#include "replace.h"
#include <chrono>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

static void wrong_input()
{
    fmt::print("\nPlease input a valid command. Available commands are as follows:\n\n\tinit - Init a templtr project\n\tbuild <outdir> - Build project files to specified directory\n\n");
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

    fmt::print("Project initialized!\n");

    return 0;
}

static std::string read_template_file(std::string tmpl_path)
{
    if (std::filesystem::exists(tmpl_path)) {
        std::ifstream tmpl_file(tmpl_path, std::ifstream::binary);
        std::stringstream buffer;
        buffer << tmpl_file.rdbuf();
        std::string tmpl = buffer.str();
        // Minify
        tmpl = std::regex_replace(tmpl, std::regex("[\n\r\t]"), "");
        return tmpl;
    } else {
        return "";
    }
}

static int build(std::string_view outdir)
{
    fmt::print("Started build...\n");

    std::chrono::time_point start_time = std::chrono::high_resolution_clock::now();

    if (std::filesystem::exists(outdir)) {
        std::filesystem::remove_all(outdir);
    }

    std::filesystem::create_directory(outdir);

    std::vector<std::tuple<std::string, std::string>> components;

    for (const auto& component : std::filesystem::directory_iterator("components")) {
        std::string component_name = component.path().stem().string();
        std::string component_path = component.path().string();

        std::string component_tmpl = read_template_file(component_path);
        components.push_back({ component_name, component_tmpl });
    }

    // Iterate over content files
    for (const auto& page : std::filesystem::directory_iterator("content")) {
        std::string page_name = page.path().stem().string();
        std::string page_path = page.path().string();
        fmt::print("Building {}...\n", page_name);

        std::filesystem::create_directory(fmt::format("{}/{}", outdir, page_name));

        // Iterate over static pages
        if (std::filesystem::is_regular_file(page_path)) {
            std::string tmpl_path = fmt::format("pages/{}.html", page_name);

            // Read template file
            std::string tmpl = read_template_file(tmpl_path);
            if (tmpl == "") {
                fmt::print("Error: No matching template for {}", page_name);
                return -1;
            }

            // Populate components
            for (const auto& component : components) {
                tmpl = std::regex_replace(tmpl, std::regex(fmt::format("<{}[^>]*>", std::get<0>(component))), std::get<1>(component));
            }

            std::ifstream entry_file(page_path, std::ifstream::binary);
            Json::Value data;
            entry_file >> data;

            std::string built_page = tmpl;

            // Iterate over keys
            if (data.size() > 0) {
                for (Json::Value::const_iterator itr = data.begin(); itr != data.end(); itr++) {
                    std::string key = itr.name();

                    built_page = replace(built_page, itr, key);
                }
            }

            // Write to file
            std::string out_dir = fmt::format("{}/{}", outdir, page_name);
            std::filesystem::create_directory(out_dir);
            std::ofstream out_file(fmt::format("{}/index.html", out_dir));

            out_file << built_page;

            out_file.close();
        }

        // Iterate over dynamic pages
        if (std::filesystem::is_directory(page_path)) {
            std::string tmpl_path = fmt::format("pages/[{}].html", page_name);

            // Read template file
            std::string tmpl = read_template_file(tmpl_path);
            if (tmpl == "") {
                fmt::print("Error: No matching template for {}", page_name);
                return -1;
            }

            // Populate components
            for (const auto& component : components) {
                tmpl = std::regex_replace(tmpl, std::regex(fmt::format("<{}[^>]*>", std::get<0>(component))), std::get<1>(component));
            }

            for (const auto& entry : std::filesystem::directory_iterator(page_path)) {
                std::string entry_filename = entry.path().string();
                std::string entry_name = entry.path().stem().string();

                std::ifstream entry_file(entry_filename, std::ifstream::binary);
                Json::Value data;
                entry_file >> data;

                std::string built_page = tmpl;

                // Iterate over keys
                if (data.size() > 0) {
                    for (Json::Value::const_iterator itr = data.begin(); itr != data.end(); itr++) {
                        std::string key = itr.name();

                        built_page = replace(built_page, itr, key);
                    }
                }

                // Write to file
                std::string out_dir = fmt::format("{}/{}/{}", outdir, page_name, entry_name);
                std::filesystem::create_directory(out_dir);
                std::ofstream out_file(fmt::format("{}/index.html", out_dir));

                out_file << built_page;

                out_file.close();
            }
        }

        fmt::print("Finished building {}\n", page_name);
    }

    // Calculate build duration
    std::chrono::time_point end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration build_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    fmt::print("Build finished in {} milliseconds\n", build_time.count());

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
                fmt::print("Build failed!");
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
