#include "replace.h"

#include "events.h"

#include <fmt/core.h>
#include <fstream>
#include <maddy/parser.h>
#include <re2/re2.h>
#include <regex>
#include <string>

std::string string_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string value = itr->asString();

    re2::RE2 regex(fmt::format("{{{}}}", key));

    std::string file_match;

    if (re2::RE2::FullMatch(value, ":(.*):", &file_match)) {
        std::ifstream file(fmt::format("public{}", file_match), std::ifstream::binary);
        std::stringstream buffer;
        buffer << file.rdbuf();
        value = buffer.str();
        re2::RE2::GlobalReplace(&value, "\r\n", "\n\n");

        if (re2::RE2::PartialMatch(file_match, "\\.md")) {
            std::stringstream markdown_input(value);
            std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
            value = parser->Parse(markdown_input);
        }
    }
    re2::RE2::GlobalReplace(&value, "\\\\", "\\\\\\\\");

    std::string out_str = template_string;
    re2::RE2::GlobalReplace(&out_str, regex, value);

    return out_str;
}

std::string int_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string value = std::to_string(itr->asInt());

    re2::RE2 regex(fmt::format("{{{}}}", key));
    std::string out_str = template_string;
    re2::RE2::GlobalReplace(&out_str, regex, value);

    return out_str;
}

std::string float_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string value = std::to_string(itr->asFloat());

    re2::RE2 regex(fmt::format("{{{}}}", key));
    std::string out_str = template_string;
    re2::RE2::GlobalReplace(&out_str, regex, value);

    return out_str;
}

std::string bool_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    bool raw_value = itr->asBool();
    std::string value;

    if (raw_value)
        value = "true";
    else
        value = "false";

    re2::RE2 regex(fmt::format("{{{}}}", key));
    std::string out_str = template_string;
    re2::RE2::GlobalReplace(&out_str, regex, value);

    return out_str;
}

// This gives a compiler warning for unused parameter.
// This is needed to create consistency with the other replace functions.
std::string null_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    re2::RE2 regex(fmt::format("{{{}}}", key));
    std::string out_str = template_string;
    re2::RE2::GlobalReplace(&out_str, regex, "");

    return out_str;
}

std::string array_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string result_string = template_string;
    re2::RE2 regex(fmt::format("(\\[[^\\{{]*\\{{{}[^\\}}]*\\}}[\\s\\S]*\\])", key));
    std::string match;

    // Iterate over array occurrences
    while (re2::RE2::PartialMatch(result_string, regex, &match)) {
        std::string array_items;

        // Temporary fix until I can get the regex working
        size_t index = 0;
        int lbrkt = 0;
        int rbrkt = 0;
        do {
            char character = match[index];
            if (character == char(91))
                ++lbrkt;
            else if (character == char(93))
                ++rbrkt;
            ++index;
        } while (lbrkt != rbrkt);

        match.resize(index);

        std::string array_item_template = match.substr(1, match.size() - 2);

        // Iterate over array entires
        for (Json::Value::const_iterator item = itr->begin(); item != itr->end(); item++) {
            array_items += replace(array_item_template, item, key);
        }

        result_string.replace(result_string.find(match), match.size(), array_items);
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