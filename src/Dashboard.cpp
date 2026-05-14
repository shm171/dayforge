#include "dayforge/Dashboard.hpp"

#include "dayforge/Entry.hpp"
#include "dayforge/Query.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace dayforge {
namespace {

constexpr int width = 72;

std::string line(const char left, const char fill, const char right) {
    return std::string(1, left) + std::string(width - 2, fill) + right + "\n";
}

std::string fit_text(const std::string& text, const std::size_t size) {
    if (text.size() <= size) {
        return text + std::string(size - text.size(), ' ');
    }
    if (size <= 3) {
        return text.substr(0, size);
    }
    return text.substr(0, size - 3) + "...";
}

std::string row(const std::string& text) {
    return "| " + fit_text(text, width - 4) + " |\n";
}

std::vector<Entry> latest_entries(std::vector<Entry> entries, const std::size_t limit) {
    std::sort(entries.begin(), entries.end(), [](const Entry& left, const Entry& right) {
        return left.timestamp > right.timestamp;
    });

    if (entries.size() > limit) {
        entries.resize(limit);
    }
    return entries;
}

std::string progress_bar(const int value, const int maximum) {
    constexpr int bar_width = 24;
    const auto filled = maximum == 0 ? 0 : (value * bar_width) / maximum;

    std::string bar = "[";
    for (int i = 0; i < bar_width; ++i) {
        bar += i < filled ? '#' : '.';
    }
    bar += "]";
    return bar;
}

} // namespace

std::string render_terminal_dashboard(const std::vector<Entry>& entries) {
    std::ostringstream out;
    const auto today = today_local_date();

    Query today_query;
    today_query.from_date = today;
    today_query.to_date = today;
    const auto today_entries = filter_entries(entries, today_query);

    std::vector<std::string> week_dates;
    week_dates.reserve(7);
    for (int days_ago = 6; days_ago >= 0; --days_ago) {
        week_dates.push_back(local_date_days_ago(days_ago));
    }

    Query week_query;
    week_query.from_date = week_dates.front();
    week_query.to_date = week_dates.back();
    const auto week_entries = filter_entries(entries, week_query);
    const auto daily_counts = count_by_date(week_entries);
    const auto tag_counts = count_by_tag(week_entries);

    auto max_daily_count = 0;
    for (const auto& date : week_dates) {
        const auto found = daily_counts.find(date);
        if (found != daily_counts.end()) {
            max_daily_count = std::max(max_daily_count, found->second);
        }
    }

    out << line('+', '=', '+');
    out << row("dayforge terminal dashboard");
    out << line('+', '=', '+');
    out << row("today: " + std::to_string(today_entries.size()) +
               " entries   week: " + std::to_string(week_entries.size()) +
               " entries   all: " + std::to_string(entries.size()) + " entries");
    out << line('+', '-', '+');
    out << row("last 7 days");
    for (const auto& date : week_dates) {
        auto count = 0;
        const auto found = daily_counts.find(date);
        if (found != daily_counts.end()) {
            count = found->second;
        }

        std::ostringstream day;
        day << date << " " << progress_bar(count, max_daily_count) << " " << count;
        out << row(day.str());
    }

    out << line('+', '-', '+');
    out << row("top tags this week");
    if (tag_counts.empty()) {
        out << row("no tags yet");
    } else {
        std::vector<std::pair<std::string, int>> tags(tag_counts.begin(), tag_counts.end());
        std::sort(tags.begin(), tags.end(), [](const auto& left, const auto& right) {
            if (left.second == right.second) {
                return left.first < right.first;
            }
            return left.second > right.second;
        });

        const auto limit = std::min<std::size_t>(5, tags.size());
        for (std::size_t i = 0; i < limit; ++i) {
            out << row(tags[i].first + ": " + std::to_string(tags[i].second));
        }
    }

    out << line('+', '-', '+');
    out << row("latest entries");
    const auto latest = latest_entries(entries, 5);
    if (latest.empty()) {
        out << row("no entries yet; add one with: dayforge add --title \"...\"");
    } else {
        for (const auto& entry : latest) {
            auto text = entry.timestamp + "  " + entry.title;
            if (!entry.tags.empty()) {
                text += " [" + join_tags(entry.tags) + "]";
            }
            out << row(text);
        }
    }
    out << line('+', '=', '+');

    return out.str();
}

} // namespace dayforge
