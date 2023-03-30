#pragma once

#include <string>
#include <vector>

struct Options {
    std::string out_dir = "out";
    std::string base = "";
};

namespace Global {
extern std::string current_outdir;
extern std::vector<std::string> scripts;
}