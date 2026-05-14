# dayforge Design Notes

## Goal

`dayforge` is a local-first development journal written in modern C++. It should stay small enough to understand, but structured enough to support real daily improvements.

## Current architecture

- `Entry` owns the persisted record shape and TSV serialization.
- `Ledger` owns file IO and directory creation.
- `Query` owns filtering and aggregation behavior.
- `Report` owns Markdown rendering for human-readable summaries.
- `main.cpp` is intentionally thin command routing.

The `today` command is built from the same query and aggregation primitives as `list` and `stats`, so daily summaries stay consistent with broader reports.

## Storage format

The ledger is append-only TSV with five fields:

```text
id    timestamp    title    comma-separated-tags    note
```

Tabs, newlines, and backslashes are escaped, so notes can contain multiline text without breaking the ledger.

## Extension ideas

- Add `edit` and `remove` using tombstone records instead of mutating history.
- Add JSON export.
- Add a richer query grammar: `tag:cpp after:2026-05-14`.
- Add weekly summaries and streak reports.
- Add a terminal UI once the core behavior is stable.
