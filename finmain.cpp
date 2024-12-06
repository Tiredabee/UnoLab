/* Final Lab - Uno
 *
 * Noelle Whidden, Raj Khot, Abbi Upchurch
 *
 * For our final lab we decided to create an Uno game. It allows for 2-6 players to play, deals them a hand of 7 random
 * cards, and keeps track of a discard pile. It correctly checks for the colors of discarded cards as well as face value,
 * and enforces the official rules of Uno throughout the game. 
 *
 * finmain.cpp controls the calling of the larger-scale functions that start the game and keep it running, while it
 * calls the smaller-scale functions on its own. */ 

#include "uno.h"
#include <iostream>

int main() {
    // Gathers the number of players wanting to play the game
    int numPlayers;
    std::cout << "Enter the number of players (2-6): ";
    std::cin >> numPlayers;

    // This program only permits 2-6 players
    if (numPlayers < 2 || numPlayers > 6) {
        std::cerr << "Invalid number of players. Must be between 2 and 6.\n";
        return 1;
    }

    // Starts the game
    UnoGame game(numPlayers);

    // Constantly shows the game interface, allows players to take turns, and checks if anyone has won
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
