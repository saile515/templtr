#include "replace.h"

#include "events.h"

#include <fmt/core.h>
#include <fstream>
#include <regex>
#include <string>

std::string string_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string value = itr->asString();

    std::regex regex(fmt::format("\\{{{}\\}}", key));

    std::smatch file_match;
    std::regex_match(value, file_match, std::regex(":(.*):"));

    if (!file_match.empty()) {
        std::ifstream file(fmt::format("public{}", file_match[1].str()), std::ifstream::binary);
        std::stringstream buffer;
        buffer << file.rdbuf();
        value = buffer.str();
        value = std::regex_replace(value, std::regex("\r\n"), "\n");
    }

    return std::regex_replace(template_string, regex, value);
}

std::string int_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string value = std::to_string(itr->asInt());

    std::regex regex(fmt::format("\\{{{}\\}}", key));
    return std::regex_replace(template_string, regex, value);
}

std::string float_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string value = std::to_string(itr->asFloat());

    std::regex regex(fmt::format("\\{{{}\\}}", key));
    return std::regex_replace(template_string, regex, value);
}

std::string bool_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    bool raw_value = itr->asBool();
    std::string value;

    if (raw_value)
        value = "True";
    else
        value = "False";

    std::regex regex(fmt::format("\\{{{}\\}}", key));
    return std::regex_replace(template_string, regex, value);
}

// This gives a compiler warning for unused parameter.
// This is needed to create consistency with the other replace functions.
std::string null_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::regex regex(fmt::format("\\{{{}\\}}", key));
    return std::regex_replace(template_string, regex, "");
}

std::string array_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string result_string = template_string;
    std::regex regex(fmt::format("\\[([^]*\\{{{}[^\\}}]*\\}}[^]*)\\]", key));
    std::smatch match;

    // Iterate over array occurrences
    while (std::regex_search(result_string, match, regex)) {
        std::string orignal_string = match[0];
        std::string array_items;

        // Temporary fix until I can get the regex working
        size_t index = 0;
        int lbrkt = 0;
        int rbrkt = 0;
        do {
            if (orignal_string[index] == char(91))
                ++lbrkt;
            else if (orignal_string[index] == char(93))
                ++rbrkt;
            index++;
        } while (lbrkt != rbrkt);

        orignal_string.resize(index);

        std::string array_item_template = orignal_string.substr(1, orignal_string.size() - 2);

        // Iterate over array entires
        for (Json::Value::const_iterator item = itr->begin(); item != itr->end(); item++) {
            array_items += replace(array_item_template, item, key);
        }

        result_string.replace(result_string.find(orignal_string), orignal_string.size(), array_items);
    }

    return result_string;
}

std::string object_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string result_string = template_string;

    for (Json::Value::const_iterator sub_key = itr->begin(); sub_key != itr->end(); sub_key++) {
        std::string final_key = fmt::format("{}.{}", key, sub_key.name());

        result_string = replace(result_string, sub_key, final_key);
    }

    return result_string;
}

std::string replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string local_key = itr.name();

    if (local_key[0] == char(36)) { // Is event (36 == $)
        return event_replace(template_string, itr, key);
    } else if (itr->isString()) {
        return string_replace(template_string, itr, key);
    } else if (itr->isInt()) {
        return int_replace(template_string, itr, key);
    } else if (itr->isDouble()) {
        return float_replace(template_string, itr, key);
    } else if (itr->isBool()) {
        return bool_replace(template_string, itr, key);
    } else if (itr->isNull()) {
        return null_replace(template_string, itr, key);
    } else if (itr->isArray()) {
        return array_replace(template_string, itr, key);
    } else if (itr->isObject()) {
        return object_replace(template_string, itr, key);
    } else {
        return template_string;
    }
}