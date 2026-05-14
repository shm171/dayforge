#pragma once

#include "dayforge/Entry.hpp"

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace dayforge {

struct Query {
    std::optional<std::string> tag;
    std::optional<std::string> from_date;
    std::optional<std::string> to_date;
};

bool matches_query(const Entry& entry, const Query& query);
std::vector<Entry> filter_entries(const std::vector<Entry>& entries, const Query& query);
std::map<std::string, int> count_by_date(const std::vector<Entry>& entries);
std::map<std::string, int> count_by_tag(const std::vector<Entry>& entries);
std::string entry_date(const Entry& entry);
std::string local_date_days_ago(int days_ago);
std::string today_local_date();

} // namespace dayforge
