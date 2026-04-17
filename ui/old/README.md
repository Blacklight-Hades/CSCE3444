# Old UI Files (Redundant)

These files are the original flat UI implementation that has been replaced by the
updated modular screen-based UI system.

## Why These Are Redundant

The new UI uses a screen-based architecture located in:
- `ui/app/`       — CasinoApp and AppState (main orchestrator)
- `ui/screens/`   — Individual game screens (MainMenu, GameSelect, BlackjackUI, SlotsUI)
- `ui/componets/` — Reusable UI components (Button)

The old files used a single CasinoApp that routed through SessionManager for all
game logic. The new screens manage game instances directly with a shared bankroll model.

## Files in This Folder

- `AppState.h`         — Old enum with Welcome, BankrollSetup, MainHub, etc. states
- `CasinoApp.h/cpp`    — Old orchestrator that used SessionManager and raw Button pointers
- `BlackjackScreen.h/cpp` — Old blackjack screen that went through SessionManager
- `Button.h/cpp`       — Reusable button class (still available in ui/componets/)

These files are kept for reference only. They are not compiled or linked.
