#include "dayforge/Query.hpp"

#include <algorithm>

namespace dayforge {

std::string entry_date(const Entry& entry) {
    if (entry.timestamp.size() < 10) {
        return {};
    }
    return entry.timestamp.substr(0, 10);
}

bool matches_query(const Entry& entry, const Query& query) {
    const auto date = entry_date(entry);

    if (query.from_date && date < *query.from_date) {
        return false;
    }
    if (query.to_date && date > *query.to_date) {
        return false;
    }
    if (query.tag) {
        const auto found = std::find(entry.tags.begin(), entry.tags.end(), *query.tag);
        if (found == entry.tags.end()) {
            return false;
        }
    }

    return true;
}

std::vector<Entry> filter_entries(const std::vector<Entry>& entries, const Query& query) {
    std::vector<Entry> filtered;
    for (const auto& entry : entries) {
        if (matches_query(entry, query)) {
            filtered.push_back(entry);
        }
    }
    return filtered;
}

std::map<std::string, int> count_by_tag(const std::vector<Entry>& entries) {
    std::map<std::string, int> counts;
    for (const auto& entry : entries) {
        if (entry.tags.empty()) {
            ++counts["untagged"];
            continue;
        }
        for (const auto& tag : entry.tags) {
            ++counts[tag];
        }
    }
    return counts;
}

} // namespace dayforge
