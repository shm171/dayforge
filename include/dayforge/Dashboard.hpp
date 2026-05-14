#pragma once

#include "dayforge/Entry.hpp"

#include <string>
#include <vector>

namespace dayforge {

std::string render_terminal_dashboard(const std::vector<Entry>& entries);

} // namespace dayforge
