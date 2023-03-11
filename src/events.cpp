#include "events.h"

#include <fmt/core.h>
#include <math.h>
#include <regex>

static int id = -1;

// Can generate up to 456976 (26^4) IDs
static std::string generate_id()
{
    id++;
    return fmt::format("{}{}{}{}", char(97 + (int(floor(id / (26 * 26 * 26))) % 26)), char(97 + (int(floor(id / (26 * 26))) % 26)), char(97 + (int(floor(id / 26)) % 26)), char(97 + (id % 26)));
}

std::string build_event(const std::string& template_string, Json::Value::const_iterator itr, std::string key)
{
    Json::Value event = &itr;
    // fmt::print("Event type: {}", event["type"].asString());

    std::string final_key = fmt::format("{}:trigger", key);

    std::string event_id = generate_id();

    std::regex regex(fmt::format("\\{{\\{}\\}}", final_key));
    fmt::print("{} {}", final_key, fmt::format("\\{{\\{}\\}}", final_key));
    return std::regex_replace(template_string, regex, fmt::format("{}()", event_id));
};