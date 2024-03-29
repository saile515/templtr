#include "build.h"
#include "globals.h"
#include "replace.h"

#include <chrono>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <re2/re2.h>
#include <string>
#include <vector>

std::string Global::current_outdir;
std::vector<std::string> Global::scripts;

std::string read_template_file(std::string tmpl_path)
{
    if (std::filesystem::exists(tmpl_path)) {
        std::ifstream tmpl_file(tmpl_path, std::ifstream::binary);
        std::stringstream buffer;
        buffer << tmpl_file.rdbuf();
        std::string tmpl = buffer.str();
        // Minify
        re2::RE2::GlobalReplace(&tmpl, "[\n\r\t]", "");
        return tmpl;
    } else {
        return "";
    }
}

void build_page(std::string page_path, std::string tmpl)
{
    Global::scripts.clear();
    std::filesystem::create_directories(Global::current_outdir);

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

    // Add scripts
    std::string scripts_str = "";
    for (auto& script : Global::scripts) {
        scripts_str += fmt::format("<script src='{}' async></script>", script);
    }
    re2::RE2::GlobalReplace(&scripts_str, "\\\\", "\\\\\\\\");
    re2::RE2::GlobalReplace(&built_page, "<head>", fmt::format("<head>{}", scripts_str));

    // Write to file
    std::ofstream out_file(fmt::format("{}/index.html", Global::current_outdir));

    out_file << built_page;
    out_file.close();
}

int build(Options options)
{
    fmt::print("Started build...\n");

    std::chrono::time_point start_time = std::chrono::high_resolution_clock::now();

    if (std::filesystem::exists(options.out_dir)) {
        std::filesystem::remove_all(options.out_dir);
    }

    // Move public files to root of out_dir
    std::filesystem::copy("public", options.out_dir, std::filesystem::copy_options::recursive);

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

        std::string tmpl_path;

        if (std::filesystem::is_regular_file(page_path)) { // Static page
            tmpl_path = fmt::format("pages/{}.html", page_name);
        } else if (std::filesystem::is_directory(page_path)) { // Dynamic page
            tmpl_path = fmt::format("pages/[{}].html", page_name);
        }

        // Read template file
        std::string tmpl = read_template_file(tmpl_path);
        if (tmpl == "") {
            fmt::print("Error: No matching template for {}\n", page_name);
            return -1;
        }

        // Populate components
        for (const auto& component : components) {
            re2::RE2::GlobalReplace(&tmpl, fmt::format("<{}[^>]*>", std::get<0>(component)), std::get<1>(component));
        }

        re2::RE2::GlobalReplace(&tmpl, "</head>", fmt::format("{}</head>", fmt::format("<base href=\"{}\"/>", options.base)));

        if (std::filesystem::is_regular_file(page_path)) { // Static page
            if (page_name == "index") {
                Global::current_outdir = options.out_dir;
            } else {
                Global::current_outdir = fmt::format("{}/{}", options.out_dir, page_name);
            }

            build_page(page_path, tmpl);
        } else if (std::filesystem::is_directory(page_path)) { // Dynamic page
            for (const auto& entry : std::filesystem::directory_iterator(page_path)) {
                std::string entry_filename = entry.path().string();
                std::string entry_name = entry.path().stem().string();

                Global::current_outdir = fmt::format("{}/{}/{}", options.out_dir, page_name, entry_name);

                build_page(entry_filename, tmpl);
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