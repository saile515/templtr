#include "events.h"

#include "globals.h"

#include <fmt/core.h>
#include <fstream>
#include <math.h>
#include <re2/re2.h>
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
                    std::string json_str = writer.write(val);
                    re2::RE2::GlobalReplace(&json_str, "[\'\"]", "\\\\'");
                    this->value = T(json_str);
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

struct WsListenEvent : public Event {
    EventValue<std::string> target;
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

    int event_status = -1;

    if (event_type == "httpRequest") {
        event_status = http_event(js_out, itr, event_id);
    } else if (event_type == "wsListen") {
        event_status = ws_listen_event(js_out, itr, event_id);
    }

    if (event_status != 0) {
        fmt::print("Error: Failed to parse event \"{}\"", event_id);
    }

    // Minify js
    re2::RE2::GlobalReplace(&js_out, "[\n\r\t]", "");

    // Write to file
    std::string out_filepath = fmt::format("{}/{}.js", Global::current_outdir, event_id);
    std::ofstream out_file(out_filepath);

    out_file << js_out;
    out_file.close();

    // Replace in template
    Global::scripts.push_back(fmt::format("./{}.js", event_id));

    std::string final_key = fmt::format("{}:trigger", key);

    std::string result_string = template_string;
    re2::RE2::GlobalReplace(&result_string, fmt::format("\\{{\\{}\\}}", final_key), fmt::format("{}()", event_id));
    return result_string;
};

int http_event(std::string& out_str, Json::Value::const_iterator itr, std::string id)
{
    HttpEvent event;

    event.id = id;

    event.type.find_in_iterator(itr, "type");
    event.target.find_in_iterator(itr, "target");
    event.method.find_in_iterator(itr, "method");
    event.body.find_in_iterator(itr, "body");

    std::string js_out = fmt::format("function {}(){{", id);

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
        js_out += fmt::format(".then(res => )", event.response_selector.value);
    }

    js_out += "};";

    out_str = js_out;

    return 0;
}

int ws_listen_event(std::string& out_str, Json::Value::const_iterator itr, std::string id)
{
    WsListenEvent event;

    event.id = id;

    event.type.find_in_iterator(itr, "type");
    event.target.find_in_iterator(itr, "target");

    std::string js_out = fmt::format("const socket = new WebSocket('{}');", event.target.value);
    js_out += "socket.addEventListener('message', (event) = > {";
    if (event.response_selector.find_in_iterator(itr, "responseSelector")) {
        js_out += fmt::format("document.querySelectorAll('{}').forEach((match) => match.innerHTML = event.data)", event.response_selector.value);
    }
    js_out += "});";

    out_str = js_out;

    return 0;
}