#include "dayforge/Ledger.hpp"
#include "dayforge/Query.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct ParsedArgs {
    std::string command;
    std::filesystem::path file{"dayforge.tsv"};
    std::string title;
    std::vector<std::string> tags;
    std::string note;
    dayforge::Query query;
};

void print_usage() {
    std::cout
        << "dayforge - local development journal\n\n"
        << "Usage:\n"
        << "  dayforge add --title <text> [--tag <name> ...] [--note <text>] [--file <path>]\n"
        << "  dayforge list [--tag <name>] [--from YYYY-MM-DD] [--to YYYY-MM-DD] [--file <path>]\n"
        << "  dayforge stats [--from YYYY-MM-DD] [--to YYYY-MM-DD] [--file <path>]\n";
}

std::string take_value(int& index, int argc, char* argv[], const std::string& option) {
    if (index + 1 >= argc) {
        throw std::runtime_error("missing value for " + option);
    }
    return argv[++index];
}

ParsedArgs parse_args(int argc, char* argv[]) {
    if (argc < 2) {
        return {};
    }

    ParsedArgs parsed;
    parsed.command = argv[1];

    for (int i = 2; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--file") {
            parsed.file = take_value(i, argc, argv, arg);
        } else if (arg == "--title") {
            parsed.title = take_value(i, argc, argv, arg);
        } else if (arg == "--tag") {
            const auto tag = take_value(i, argc, argv, arg);
            parsed.tags.push_back(tag);
            parsed.query.tag = tag;
        } else if (arg == "--note") {
            parsed.note = take_value(i, argc, argv, arg);
        } else if (arg == "--from") {
            parsed.query.from_date = take_value(i, argc, argv, arg);
        } else if (arg == "--to") {
            parsed.query.to_date = take_value(i, argc, argv, arg);
        } else {
            throw std::runtime_error("unknown argument: " + arg);
        }
    }

    return parsed;
}

void print_entry(const dayforge::Entry& entry) {
    std::cout << entry.timestamp << "  " << entry.title;
    if (!entry.tags.empty()) {
        std::cout << "  [" << dayforge::join_tags(entry.tags) << "]";
    }
    if (!entry.note.empty()) {
        std::cout << "\n  " << entry.note;
    }
    std::cout << "\n";
}

int run(int argc, char* argv[]) {
    const auto args = parse_args(argc, argv);
    if (args.command.empty() || args.command == "--help" || args.command == "-h") {
        print_usage();
        return args.command.empty() ? 1 : 0;
    }

    const dayforge::Ledger ledger(args.file);

    if (args.command == "add") {
        if (args.title.empty()) {
            throw std::runtime_error("add requires --title");
        }

        dayforge::Entry entry;
        entry.title = args.title;
        entry.tags = args.tags;
        entry.note = args.note;

        ledger.append(entry);
        std::cout << "added: " << entry.title << "\n";
        return 0;
    }

    if (args.command == "list") {
        const auto entries = dayforge::filter_entries(ledger.read_all(), args.query);
        for (const auto& entry : entries) {
            print_entry(entry);
        }
        return 0;
    }

    if (args.command == "stats") {
        const auto entries = dayforge::filter_entries(ledger.read_all(), args.query);
        const auto counts = dayforge::count_by_tag(entries);
        for (const auto& [tag, count] : counts) {
            std::cout << tag << ": " << count << "\n";
        }
        return 0;
    }

    throw std::runtime_error("unknown command: " + args.command);
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        return run(argc, argv);
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << "\n\n";
        print_usage();
        return 2;
    }
}
