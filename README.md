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
.\build\Debug\dayforge.exe today
.\build\Debug\dayforge.exe list --tag cpp
.\build\Debug\dayforge.exe stats
.\build\Debug\dayforge.exe export-md --output reports\week.md
```

By default, entries are stored in `dayforge.tsv` in the current directory. Use `--file <path>` to choose another ledger.

## Commands

- `add --title <text> [--tag <name> ...] [--note <text>]`
- `today [--file <path>]`
- `list [--tag <name>] [--from YYYY-MM-DD] [--to YYYY-MM-DD]`
- `stats [--from YYYY-MM-DD] [--to YYYY-MM-DD]`
- `export-md --output <path> [--tag <name>] [--from YYYY-MM-DD] [--to YYYY-MM-DD]`

## Windows toolchain

One simple setup is MSYS2 with MinGW g++:

```powershell
winget install MSYS2.MSYS2
```

Open **MSYS2 UCRT64** from the Start menu, then install the compiler and CMake:

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

Add this directory to your Windows `Path`:

```text
C:\msys64\ucrt64\bin
```

Open a new PowerShell and verify:

```powershell
g++ --version
cmake --version
ninja --version
```

Then build with:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build
```

## Why this project

This is not a throwaway commit filler. It is a lightweight tool for making daily work visible, testable, and reviewable. Each future commit can add one real improvement: a parser rule, storage behavior, report format, test case, or documentation note.
