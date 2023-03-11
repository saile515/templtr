#include "events.h"

#include <fmt/core.h>
#include <math.h>
#include <regex>

struct Event {
    std::string type;
    std::string id;
};

struct HttpEvent : Event {
    std::string target;
    std::string method;
    std::string body;
};

static int id = -1;

// Can generate up to 456976 (26^4) IDs
static std::string generate_id()
{
    id++;
    return fmt::format("{}{}{}{}", char(97 + (int(floor(id / (26 * 26 * 26))) % 26)), char(97 + (int(floor(id / (26 * 26))) % 26)), char(97 + (int(floor(id / 26)) % 26)), char(97 + (id % 26)));
}

static Json::Value key_exists(Json::Value& out_value, Json::Value::const_iterator itr, std::string name)
{
    if (itr->isMember(name)) {
        out_value = itr->operator[](name);
        return 0;
    } else {
        fmt::print("Error: Event property '{}' is not defined\n", name);
        return -1;
    }
}

static int key_string(std::string& out_str, Json::Value::const_iterator itr, std::string name)
{
    Json::Value value;
    if (key_exists(value, itr, name) == 0) {
        if (value.isString()) {
            out_str = value.asString();
            return 0;
        } else {
            fmt::print("Error: Event property '{}' is not of type 'string'\n", "type");
            return -1;
        }
    } else {
        return -1;
    }
}

static int key_object(std::string& out_str, Json::Value::const_iterator itr, std::string name)
{
    Json::Value value;
    if (key_exists(value, itr, name) == 0) {
        if (value.isObject()) {
            out_str = value.toStyledString();
            return 0;
        } else {
            fmt::print("Error: Event property '{}' is not of type 'object'\n", "type");
            return -1;
        }
    } else {
        return -1;
    }
}

std::string build_event(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    std::string event_type;
    if (key_string(event_type, itr, "type") != 0) {
        fmt::print("Error: Event type not declared for event '{}'\n", key);
        return "";
    }

    std::string event_id = generate_id();

    if (event_type == "httpRequest") {
        http_event(itr, event_id);
    }

    std::string final_key = fmt::format("{}:trigger", key);

    std::regex regex(fmt::format("\\{{\\{}\\}}", final_key));
    return std::regex_replace(template_string, regex, fmt::format("{}()", event_id));
};

int http_event(Json::Value::const_iterator itr, std::string id)
{
    HttpEvent event;

    event.id = id;

    if (key_string(event.type, itr, "type") != 0)
        return -1;

    if (key_string(event.target, itr, "target") != 0)
        return -1;

    if (key_string(event.method, itr, "method") != 0)
        return -1;

    if (event.method == "POST") {
        if (key_object(event.body, itr, "body") != 0)
            return -1;
    }
    return 0;
}