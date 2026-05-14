#pragma once

#include "dayforge/Entry.hpp"

#include <string>
#include <vector>

namespace dayforge {

std::string render_markdown_report(const std::vector<Entry>& entries, const std::string& title);

} // namespace dayforge
