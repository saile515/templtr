#pragma once

#include "globals.h"
#include <string>

std::string read_template_file(std::string tmpl_path);

void build_page(std::string page_path, std::string tmpl);

int build(Options options);