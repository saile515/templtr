#pragma once

#include <json/json.h>
#include <string>

int http_event(std::string& out_str, Json::Value::const_iterator itr, std::string id);
std::string event_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);