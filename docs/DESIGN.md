# dayforge Design Notes

## Goal

`dayforge` is a local-first development journal written in modern C++. It should stay small enough to understand, but structured enough to support real daily improvements.

## Current architecture

- `Entry` owns the persisted record shape and TSV serialization.
- `Ledger` owns file IO and directory creation.
- `Query` owns filtering and aggregation behavior.
- `main.cpp` is intentionally thin command routing.

## Storage format

The ledger is append-only TSV with five fields:

```text
id    timestamp    title    comma-separated-tags    note
```

Tabs, newlines, and backslashes are escaped, so notes can contain multiline text without breaking the ledger.

## Extension ideas

- Add `edit` and `remove` using tombstone records instead of mutating history.
- Add JSON and Markdown export.
- Add a richer query grammar: `tag:cpp after:2026-05-14`.
- Add weekly summaries and streak reports.
- Add a terminal UI once the core behavior is stable.
