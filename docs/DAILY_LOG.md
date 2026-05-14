# Daily Log

## 2026-05-14

- Created the `dayforge` C++17 project.
- Added append-only TSV storage with escaping for tabs, newlines, and backslashes.
- Added CLI commands: `add`, `list`, and `stats`.
- Added tests for persistence, filtering, and tag counts.
- Wrote initial design notes and roadmap.

- Added the `today` command for quick daily summaries.
- Reused query filtering and tag aggregation instead of adding a separate reporting path.
- Documented the new command and expanded tests around local date formatting.
