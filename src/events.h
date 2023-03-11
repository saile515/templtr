#pragma once

#include <json/json.h>
#include <string>

std::string build_event(const std::string& template_string, Json::Value::const_iterator itr, std::string key);