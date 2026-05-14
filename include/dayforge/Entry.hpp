#pragma once

#include <string>
#include <vector>

namespace dayforge {

struct Entry {
    std::string id;
    std::string timestamp;
    std::string title;
    std::vector<std::string> tags;
    std::string note;
};

std::string now_iso_utc();
std::string make_entry_id(const Entry& entry);
std::string serialize_entry(const Entry& entry);
Entry parse_entry_line(const std::string& line);
std::string join_tags(const std::vector<std::string>& tags);
std::vector<std::string> split_tags(const std::string& text);

} // namespace dayforge
