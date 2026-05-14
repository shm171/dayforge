#pragma once

#include "dayforge/Entry.hpp"

#include <filesystem>
#include <vector>

namespace dayforge {

class Ledger {
public:
    explicit Ledger(std::filesystem::path path);

    const std::filesystem::path& path() const;
    void append(Entry entry) const;
    std::vector<Entry> read_all() const;

private:
    std::filesystem::path path_;
};

} // namespace dayforge
