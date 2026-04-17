# SpaceCasinoUI — Redundant Core Files

This folder contains copies of game logic and session management files that were
used during standalone UI development. They are now **redundant** because the
main project uses the canonical versions in the project root's folders:

| This Folder                  | Canonical Location          |
|-----------------------------|-----------------------------|
| `core/blackjack/`           | `games/blackjack/`          |
| `core/slots/`               | `games/slots/`              |
| `core/session/Bankroll.*`   | `core/Bankroll.*`           |
| `core/session/SessionManager.*` | `core/SessionManager.*` |
| `core/session/SessionStats.*`   | `core/SessionStats.*`   |
| `core/legacy/`              | Not used (old standalone mains) |

## Why They Exist

These files were kept inside `SpaceCasinoUI/` so that the UI project could be
built independently (e.g., in Visual Studio on Windows) without depending on
the main project's folder structure.

## Why They Are Redundant

The integrated build (via the root `Makefile`) compiles all game logic from
`games/` and `core/`. The UI files in `ui/` now reference those canonical
locations directly. These copies are not compiled or linked.

## What to Do

These files are safe to ignore. They are kept for reference in case the
SpaceCasinoUI project needs to be rebuilt standalone in the future.
