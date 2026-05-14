#include "dayforge/Dashboard.hpp"
#include "dayforge/Ledger.hpp"
#include "dayforge/Query.hpp"
#include "dayforge/Report.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace {

std::filesystem::path temp_ledger_path() {
    auto path = std::filesystem::temp_directory_path() / "dayforge_tests.tsv";
    std::filesystem::remove(path);
    return path;
}

void round_trips_entries_with_escaped_fields() {
    const auto path = temp_ledger_path();
    const dayforge::Ledger ledger(path);

    dayforge::Entry entry;
    entry.timestamp = "2026-05-14T12:00:00Z";
    entry.title = "Write\tC++ ledger";
    entry.tags = {"cpp", "tests"};
    entry.note = "Line one\nLine two";

    ledger.append(entry);
    const auto entries = ledger.read_all();

    assert(entries.size() == 1);
    assert(entries[0].title == entry.title);
    assert(entries[0].tags.size() == 2);
    assert(entries[0].tags[0] == "cpp");
    assert(entries[0].note == entry.note);

    std::filesystem::remove(path);
}

void filters_by_date_and_tag() {
    const auto path = temp_ledger_path();
    const dayforge::Ledger ledger(path);

    ledger.append(dayforge::Entry{"", "2026-05-14T09:00:00Z", "Core", {"cpp"}, ""});
    ledger.append(dayforge::Entry{"", "2026-05-15T09:00:00Z", "Docs", {"docs"}, ""});
    ledger.append(dayforge::Entry{"", "2026-05-16T09:00:00Z", "Tests", {"cpp", "tests"}, ""});

    dayforge::Query query;
    query.tag = "cpp";
    query.from_date = "2026-05-15";

    const auto filtered = dayforge::filter_entries(ledger.read_all(), query);

    assert(filtered.size() == 1);
    assert(filtered[0].title == "Tests");

    std::filesystem::remove(path);
}

void counts_tags() {
    std::vector<dayforge::Entry> entries;
    entries.push_back(dayforge::Entry{"", "2026-05-14T09:00:00Z", "Core", {"cpp"}, ""});
    entries.push_back(dayforge::Entry{"", "2026-05-14T10:00:00Z", "Tests", {"cpp", "tests"}, ""});
    entries.push_back(dayforge::Entry{"", "2026-05-14T11:00:00Z", "Untitled", {}, ""});

    const auto counts = dayforge::count_by_tag(entries);

    assert(counts.at("cpp") == 2);
    assert(counts.at("tests") == 1);
    assert(counts.at("untagged") == 1);
}

void counts_entries_by_date() {
    std::vector<dayforge::Entry> entries;
    entries.push_back(dayforge::Entry{"", "2026-05-14T09:00:00Z", "Core", {"cpp"}, ""});
    entries.push_back(dayforge::Entry{"", "2026-05-14T10:00:00Z", "Tests", {"tests"}, ""});
    entries.push_back(dayforge::Entry{"", "2026-05-15T11:00:00Z", "Docs", {"docs"}, ""});
    entries.push_back(dayforge::Entry{"", "", "Untimed", {}, ""});

    const auto counts = dayforge::count_by_date(entries);

    assert(counts.at("2026-05-14") == 2);
    assert(counts.at("2026-05-15") == 1);
    assert(counts.at("undated") == 1);
}

void local_today_uses_date_format() {
    const auto today = dayforge::today_local_date();

    assert(today.size() == 10);
    assert(today[4] == '-');
    assert(today[7] == '-');
}

void local_date_days_ago_uses_date_format() {
    const auto date = dayforge::local_date_days_ago(6);

    assert(date.size() == 10);
    assert(date[4] == '-');
    assert(date[7] == '-');
}

void renders_markdown_report_grouped_by_date() {
    std::vector<dayforge::Entry> entries;
    entries.push_back(dayforge::Entry{"", "2026-05-14T09:00:00Z", "Core", {"cpp"}, "Build the ledger"});
    entries.push_back(dayforge::Entry{"", "2026-05-14T10:00:00Z", "Docs", {"docs"}, ""});

    const auto report = dayforge::render_markdown_report(entries, "Test report");

    assert(report.find("# Test report") != std::string::npos);
    assert(report.find("Total entries: 2") != std::string::npos);
    assert(report.find("### 2026-05-14") != std::string::npos);
    assert(report.find("- Core [`cpp`]") != std::string::npos);
    assert(report.find("Build the ledger") != std::string::npos);
}

void renders_terminal_dashboard() {
    std::vector<dayforge::Entry> entries;
    entries.push_back(dayforge::Entry{"", dayforge::today_local_date() + "T09:00:00Z", "Build dashboard", {"ui", "cpp"}, ""});

    const auto dashboard = dayforge::render_terminal_dashboard(entries);

    assert(dashboard.find("dayforge") != std::string::npos);
    assert(dashboard.find("last 7 days") != std::string::npos);
    assert(dashboard.find("tags") != std::string::npos);
    assert(dashboard.find("Build dashboard") != std::string::npos);
}

} // namespace

int main() {
    round_trips_entries_with_escaped_fields();
    filters_by_date_and_tag();
    counts_tags();
    counts_entries_by_date();
    local_today_uses_date_format();
    local_date_days_ago_uses_date_format();
    renders_markdown_report_grouped_by_date();
    renders_terminal_dashboard();

    std::cout << "dayforge tests passed\n";
    return 0;
}
