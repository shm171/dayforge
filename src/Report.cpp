#include "dayforge/Report.hpp"

#include "dayforge/Entry.hpp"
#include "dayforge/Query.hpp"

#include <map>
#include <sstream>

namespace dayforge {
namespace {

std::map<std::string, std::vector<Entry>> group_by_date(const std::vector<Entry>& entries) {
    std::map<std::string, std::vector<Entry>> grouped;
    for (const auto& entry : entries) {
        auto date = entry_date(entry);
        if (date.empty()) {
            date = "undated";
        }
        grouped[date].push_back(entry);
    }
    return grouped;
}

} // namespace

std::string render_markdown_report(const std::vector<Entry>& entries, const std::string& title) {
    std::ostringstream out;

    out << "# " << title << "\n\n";
    out << "Total entries: " << entries.size() << "\n\n";

    if (entries.empty()) {
        out << "_No entries matched this report._\n";
        return out.str();
    }

    out << "## Tags\n\n";
    for (const auto& [tag, count] : count_by_tag(entries)) {
        out << "- `" << tag << "`: " << count << "\n";
    }

    out << "\n## Entries\n\n";
    for (const auto& [date, dated_entries] : group_by_date(entries)) {
        out << "### " << date << "\n\n";
        for (const auto& entry : dated_entries) {
            out << "- " << entry.title;
            if (!entry.tags.empty()) {
                out << " [";
                for (std::size_t i = 0; i < entry.tags.size(); ++i) {
                    if (i > 0) {
                        out << " ";
                    }
                    out << "`" << entry.tags[i] << "`";
                }
                out << "]";
            }
            out << "\n";

            if (!entry.note.empty()) {
                out << "  - " << entry.note << "\n";
            }
        }
        out << "\n";
    }

    return out.str();
}

} // namespace dayforge
