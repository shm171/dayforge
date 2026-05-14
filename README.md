# dayforge

`dayforge` is a small C++17 command line project journal. It stores daily work entries in a plain TSV ledger, then lets you list, filter, and summarize them.

The project is intentionally built to grow day by day: the core is useful now, while the roadmap leaves room for query language improvements, exports, richer statistics, and a future TUI.

## Build

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Usage

```powershell
.\build\Debug\dayforge.exe add --title "Start dayforge" --tag cpp --tag cli --note "Initial ledger implementation"
.\build\Debug\dayforge.exe list --tag cpp
.\build\Debug\dayforge.exe stats
```

By default, entries are stored in `dayforge.tsv` in the current directory. Use `--file <path>` to choose another ledger.

## Commands

- `add --title <text> [--tag <name> ...] [--note <text>]`
- `list [--tag <name>] [--from YYYY-MM-DD] [--to YYYY-MM-DD]`
- `stats [--from YYYY-MM-DD] [--to YYYY-MM-DD]`

## Why this project

This is not a throwaway commit filler. It is a lightweight tool for making daily work visible, testable, and reviewable. Each future commit can add one real improvement: a parser rule, storage behavior, report format, test case, or documentation note.
