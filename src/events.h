#pragma once

#include <json/json.h>
#include <string>

int http_event(Json::Value::const_iterator itr, std::string id);
std::string build_event(const std::string& template_string, Json::Value::const_iterator itr, std::string key);