#include <string>

static std::string string_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
static std::string int_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
static std::string float_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
static std::string bool_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
static std::string null_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
static std::string array_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
static std::string object_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);
static std::string replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key);