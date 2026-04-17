#ifndef CASINOAPP_H
#define CASINOAPP_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "AppState.h"
#include "../screens/welcome/MainMenu.h"
#include "../screens/hub/GameSelect.h"
#include "../screens/blackjack/BlackjackUI.h"
#include "../screens/slots/SlotsUI.h"
#include "../screens/roulette/RouletteUI.h"

class CasinoApp
{
public:
    CasinoApp();
    void run();

private:
    void processEvents();
    void render();
    void syncGameSelectBankroll();
    void syncBlackjackBankroll();
    void syncSlotsBankroll();
    void syncRouletteBankroll();
    void resetSessionIfNeeded();

    sf::RenderWindow window;
    sf::Font font;

    std::unique_ptr<MainMenu> mainMenu;
    std::unique_ptr<GameSelect> gameSelect;
    std::unique_ptr<BlackjackUI> blackjackUI;
    std::unique_ptr<SlotsUI> slotsUI;
    std::unique_ptr<RouletteUI> rouletteUI;

    AppState currentState;
    double sharedBankroll;
    bool bankrollInitialized;
};

#endif