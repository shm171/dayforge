#include "dayforge/Ledger.hpp"

#include <fstream>
#include <stdexcept>
#include <utility>

namespace dayforge {

Ledger::Ledger(std::filesystem::path path) : path_(std::move(path)) {}

const std::filesystem::path& Ledger::path() const {
    return path_;
}

void Ledger::append(Entry entry) const {
    if (entry.timestamp.empty()) {
        entry.timestamp = now_iso_utc();
    }
    if (entry.id.empty()) {
        entry.id = make_entry_id(entry);
    }

    const auto parent = path_.parent_path();
    if (!parent.empty()) {
        std::filesystem::create_directories(parent);
    }

    std::ofstream out(path_, std::ios::app);
    if (!out) {
        throw std::runtime_error("could not open ledger for append: " + path_.string());
    }

    out << serialize_entry(entry) << '\n';
}

std::vector<Entry> Ledger::read_all() const {
    std::vector<Entry> entries;
    std::ifstream in(path_);

    if (!in) {
        return entries;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            entries.push_back(parse_entry_line(line));
        }
    }

    return entries;
}

} // namespace dayforge
