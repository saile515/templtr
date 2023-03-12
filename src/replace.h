#pragma once

#include <json/json.h>
#include <string>

std::string string_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
std::string int_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
std::string float_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
std::string bool_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
std::string null_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
std::string array_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
std::string object_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
std::string replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);