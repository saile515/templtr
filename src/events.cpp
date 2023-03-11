#include "events.h"

#include <fmt/core.h>
#include <math.h>
#include <regex>

template <class T, bool optional = true>
class EventValue {
public:
    T value;

    int find_in_iterator(Json::Value::const_iterator itr, std::string key)
    {
        if (itr->isMember(key)) {
            try {
                Json::Value val = itr->operator[](key);

                if (val.isObject() || val.isArray()) {
                    Json::FastWriter writer;
                    this->value = T(writer.write(val));
                } else {
                    this->value = T(val.asString());
                }

            } catch (...) {
                fmt::print("Error: Event property '{}' is not of correct type\n", key);
                return -1;
            }
        } else {
            if (!optional) {
                fmt::print("Error: Event property '{}' is not defined\n", key);
            }

            return -1;
        }

        return 0;
    }

    operator=(T new_value)
    {
        value = new_value;
    }

    operator==(const T val)
    {
        return value == val;
    }
};

struct Event {
    EventValue<std::string> type;
    EventValue<std::string> id;
};

struct HttpEvent : public Event {
    EventValue<std::string> target;
    EventValue<std::string> method;
    EventValue<std::string, false> body;
    EventValue<std::string, false> response_selector;
};

static int id = -1;

// Can generate up to 456976 (26^4) IDs
static std::string generate_id()
{
    ++id;
    return fmt::format("{}{}{}{}", char(97 + (int(floor(id / (26 * 26 * 26))) % 26)), char(97 + (int(floor(id / (26 * 26))) % 26)), char(97 + (int(floor(id / 26)) % 26)), char(97 + (id % 26)));
}

std::string event_replace(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    EventValue<std::string> event_type;
    event_type.find_in_iterator(itr, "type");

    std::string event_id = generate_id();

    std::string js_out;

    if (event_type == "httpRequest") {
        http_event(js_out, itr, event_id);
    }

    std::string final_key = fmt::format("{}:trigger", key);

    std::regex regex(fmt::format("\\{{\\{}\\}}", final_key));
    return std::regex_replace(template_string, regex, fmt::format("{}()", event_id));
};

int http_event(std::string& out_str, Json::Value::const_iterator itr, std::string id)
{
    HttpEvent event;

    event.id = id;

    event.type.find_in_iterator(itr, "type");
    event.target.find_in_iterator(itr, "target");
    event.method.find_in_iterator(itr, "method");
    event.body.find_in_iterator(itr, "body");

    std::string js_out = fmt::format("function {}() {{", event.id.value);

    if (event.method == "GET" || event.method == "DELETE") {
        js_out += fmt::format("fetch('{}')", event.target.value);
    } else if (event.method == "POST" || event.method == "PUT") {
        js_out += fmt::format("fetch('{}', {{method: {}, body: {}}})", event.target.value, event.method.value, event.body.value);
    } else {
        fmt::print("{}\n", event.method.value);
        fmt::print("Error: method must be 'GET', 'POST', 'PUT', or 'DELETE'\n");
        return -1;
    }

    if (event.response_selector.find_in_iterator(itr, "responseSelector") == 0) {
        js_out += ".then(res => res.text())";
        js_out += fmt::format(".then(res => document.querySelectorAll('{}').forEach((match) => match.innerHTML = res))", event.response_selector.value);
    }

    js_out += "};";

    out_str = js_out;

    return 0;
}