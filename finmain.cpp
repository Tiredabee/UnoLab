//Executes the program and classes starting and ending the UNO game                                                                                                                                                                                                      
#include "uno.h"
#include <iostream>


int main() {
    int numPlayers;
    std::cout << "Enter the number of players (2-6): ";
    std::cin >> numPlayers;

    if (numPlayers < 2 || numPlayers > 6) {
        std::cerr << "Invalid number of players. Must be between 2 and 6.\n";
        return 1;
    }

    UnoGame game(numPlayers);

    while (true) {
        game.showGameState();
        game.takeTurn();

        if (game.checkWin()) {
            std::cout << "Game over!" << std::endl;
            break;
        }
    }

    return 0;
}
