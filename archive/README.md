# Archive — Redundant Files

These files are kept for reference but are **no longer used** by the build. They have been superseded by the current project structure.

## standalone_mains/

Old test entry points that ran individual games directly from the terminal. These are now replaced by the unified `SessionManager::runTerminalSession()` method, which is invoked from `main.cpp` (option 2 — Terminal Stats Tester).

| File | What It Did | Replaced By |
|------|-------------|-------------|
| `blackjack_main.cpp` | Standalone Blackjack terminal game | `SessionManager::runTerminalBlackjack()` in `core/SessionManager.cpp` |
| `slots_main.cpp` | Standalone Slots terminal game | `SessionManager::runTerminalSlotsPlaceholder()` in `core/SessionManager.cpp` |
| `session_main.cpp` | Standalone session runner using `SessionStats` | `main.cpp` option 2 → `SessionManager::runTerminalSession()` |

## duplicate_root_files/

These were older copies of game logic files that originally lived in the project root. They were duplicates of the files in `core/` (now moved to `games/blackjack/`). The root copies were not used in the final build — the Makefile compiled from `core/` instead.

| File | Active Version Now Located At |
|------|-------------------------------|
| `BlackjackGame.h/.cpp` | `games/blackjack/BlackjackGame.h/.cpp` |
| `BlackjackStrategy.h/.cpp` | `games/blackjack/BlackjackStrategy.h/.cpp` |
| `BlackjackTypes.h` | `games/blackjack/BlackjackTypes.h` |
| `Card.h/.cpp` | `games/blackjack/Card.h/.cpp` |
| `Hand.h/.cpp` | `games/blackjack/Hand.h/.cpp` |
| `Shoe.h/.cpp` | `games/blackjack/Shoe.h/.cpp` |
