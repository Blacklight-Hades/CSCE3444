#include "ui/app/CasinoApp.h"
#include "core/SessionManager.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "Welcome! Select application mode:\n";
    cout << "1. Run Graphical UI\n";
    cout << "2. Run Terminal Stats Tester\n";
    cout << "Choice: ";
    
    int choice;
    if (cin >> choice && choice == 2) {
        SessionManager session;
        session.runTerminalSession();
    } else {
        CasinoApp app;
        app.run();
    }
    return 0;
}