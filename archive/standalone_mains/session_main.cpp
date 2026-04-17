#include <iostream>
#include "SessionManager.h"

int main()
{
    std::cout << "========================================\n";
    std::cout << "      Welcome to Galactic Casino!\n";
    std::cout << "========================================\n";

    SessionManager session;
    session.runTerminalSession();

    std::cout << "\nThanks for playing at Galactic Casino!\n";
    return 0;
}