#include "events.h"

#include <fmt/core.h>
#include <math.h>
#include <regex>
#include <string>

template <class T, bool optional = false>
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
                    this->value = T(std::regex_replace(writer.write(val), std::regex("[\'\"]"), "\\'"));
                } else {
                    this->value = T(val.asString());
                }

            } catch (...) {
                fmt::print("Error: Event property '{}' is not of correct type\n", key);
                return -1;
            }
        } else {
            if constexpr (!optional) {
                fmt::print("Error: Event property '{}' is not defined\n", key);
            }

            return -1;
        }

        return 0;
    }

    void operator=(T new_value)
    {
        value = new_value;
    }

    bool operator==(const T val)
    {
        return value == val;
    }

    operator T&()
    {
        return value;
    }

    operator T() const
    {
        return value;
    }
};

struct Event {
    EventValue<std::string> type;
    EventValue<std::string> id;
};

struct HttpEvent : public Event {
    EventValue<std::string> target;
    EventValue<std::string> method;
    EventValue<std::string, true> body;
    EventValue<std::string, true> response_selector;
};

static int next_id = -1;

// Can generate up to 456976 (26^4) IDs
static std::string generate_id()
{
    ++next_id;
    return fmt::format("{}{}{}{}", char(97 + (int(floor(next_id / (26 * 26 * 26))) % 26)), char(97 + (int(floor(next_id / (26 * 26))) % 26)), char(97 + (int(floor(next_id / 26)) % 26)), char(97 + (next_id % 26)));
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

    js_out = std::regex_replace(js_out, std::regex("[\'\"]"), "&quot;");
    js_out = std::regex_replace(js_out, std::regex("[\n\r\t]"), "");

    std::string final_key = fmt::format("{}:trigger", key);

    std::regex regex(fmt::format("\\{{\\{}\\}}", final_key));
    return std::regex_replace(template_string, regex, js_out);
};

int http_event(std::string& out_str, Json::Value::const_iterator itr, std::string id)
{
    HttpEvent event;

    event.id = id;

    event.type.find_in_iterator(itr, "type");
    event.target.find_in_iterator(itr, "target");
    event.method.find_in_iterator(itr, "method");
    event.body.find_in_iterator(itr, "body");

    std::string js_out = "";

    if (event.method == "GET" || event.method == "DELETE") {
        js_out += fmt::format("fetch('{}')", event.target.value);
    } else if (event.method == "POST" || event.method == "PUT") {
        js_out += fmt::format("fetch('{}', {{method: '{}', body: '{}'}})", event.target.value, event.method.value, event.body.value);
    } else {
        fmt::print("Error: method must be 'GET', 'POST', 'PUT', or 'DELETE'\n");
        return -1;
    }

    if (event.response_selector.find_in_iterator(itr, "responseSelector") == 0) {
        js_out += ".then(res => res.text())";
        js_out += fmt::format(".then(res => document.querySelectorAll('{}').forEach((match) => match.innerHTML = res))", event.response_selector.value);
    }

    out_str = js_out;

    return 0;
}