#include "dayforge/Entry.hpp"

#include <chrono>
#include <functional>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace dayforge {
namespace {

std::string escape_field(const std::string& value) {
    std::string escaped;
    escaped.reserve(value.size());

    for (const char ch : value) {
        switch (ch) {
        case '\\':
            escaped += "\\\\";
            break;
        case '\t':
            escaped += "\\t";
            break;
        case '\n':
            escaped += "\\n";
            break;
        default:
            escaped += ch;
            break;
        }
    }

    return escaped;
}

std::string unescape_field(const std::string& value) {
    std::string unescaped;
    unescaped.reserve(value.size());

    for (std::size_t i = 0; i < value.size(); ++i) {
        if (value[i] != '\\') {
            unescaped += value[i];
            continue;
        }

        if (i + 1 >= value.size()) {
            throw std::runtime_error("dangling escape in ledger field");
        }

        const char next = value[++i];
        switch (next) {
        case '\\':
            unescaped += '\\';
            break;
        case 't':
            unescaped += '\t';
            break;
        case 'n':
            unescaped += '\n';
            break;
        default:
            throw std::runtime_error("unknown escape in ledger field");
        }
    }

    return unescaped;
}

std::vector<std::string> split_tsv(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;

    for (const char ch : line) {
        if (ch == '\t') {
            fields.push_back(unescape_field(current));
            current.clear();
        } else {
            current += ch;
        }
    }

    fields.push_back(unescape_field(current));
    return fields;
}

} // namespace

std::string now_iso_utc() {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &time);
#else
    gmtime_r(&time, &tm);
#endif

    std::ostringstream out;
    out << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return out.str();
}

std::string make_entry_id(const Entry& entry) {
    const std::string seed = entry.timestamp + "\n" + entry.title + "\n" + join_tags(entry.tags) + "\n" + entry.note;
    const auto hash = std::hash<std::string>{}(seed);

    std::ostringstream out;
    out << std::hex << hash;
    return out.str();
}

std::string serialize_entry(const Entry& entry) {
    return escape_field(entry.id) + "\t" + escape_field(entry.timestamp) + "\t" +
           escape_field(entry.title) + "\t" + escape_field(join_tags(entry.tags)) + "\t" +
           escape_field(entry.note);
}

Entry parse_entry_line(const std::string& line) {
    const auto fields = split_tsv(line);
    if (fields.size() != 5) {
        throw std::runtime_error("ledger line must have exactly 5 fields");
    }

    return Entry{fields[0], fields[1], fields[2], split_tags(fields[3]), fields[4]};
}

std::string join_tags(const std::vector<std::string>& tags) {
    std::string joined;
    for (std::size_t i = 0; i < tags.size(); ++i) {
        if (i > 0) {
            joined += ",";
        }
        joined += tags[i];
    }
    return joined;
}

std::vector<std::string> split_tags(const std::string& text) {
    std::vector<std::string> tags;
    std::string current;

    for (const char ch : text) {
        if (ch == ',') {
            if (!current.empty()) {
                tags.push_back(current);
            }
            current.clear();
        } else {
            current += ch;
        }
    }

    if (!current.empty()) {
        tags.push_back(current);
    }

    return tags;
}

} // namespace dayforge
