/* Final Lab - Uno
 *
 * Noelle Whidden, Raj Khot, Abbi Upchurch
 *
 * For our final lab we decided to create an Uno game. It allows for 2-6 players to play, deals them a hand of 7 random
 * cards, and keeps track of a discard pile. It correctly checks for the colors of discarded cards as well as face value,
 * and enforces the official rules of Uno throughout the game. 
 *
 * finuno.h is the header file for finuno.cpp, and initializes the functions and other components used in the program. */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

// Colors of cards used in Uno
enum class Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    NONE
};

// Types of cards used in Uno
enum class Type {
    NUMBER,
    SKIP,
    REVERSE,
    DRAW_TWO,
    WILD,
    WILD_DRAW_FOUR
};

// The card class contains the functions used in regards to the specific cards
class Card {
    public:
        Card(Color color, Type type, int value = 0);
        std::string toString() const;
        Color getColor() const;
        Type getType() const;
        int getValue() const;
        bool operator==(const Card& other) const;
        std::string colortoString(Color color) const;
        std::string typetoString(Type type) const;

    private:
        Color color;
        Type type;
        int value;


};

// The deck class contains functions used in regards to the deck of cards as a whole
class Deck {
    public:
        Deck();
        void shuffle();
        Card draw();
        bool isEmpty() const;

    private:
        std::vector<Card> cards;
        void initializeDeck();
};

// The player class contains functions used in regards to the players of the game
class Player {
    public:
        Player(const std::string &name);
        void drawCard(Deck &deck);
        void showHand() const;
        bool playCard(const Card &card, std::vector<Card> &discardPile);
        std::string getName() const;
        bool hasWon() const;
        const std::vector<Card>& getHand() const;


    private:
        std::vector<Card> hand;
        std::string playername;
};

// The UnoGame class contains functions used to control the actual playing of the game of Uno
class UnoGame {
    public:
        UnoGame(int numPlayers);
        void startGame();
        void showGameState() const;
        bool checkWin() const;
        void takeTurn();

    private:
        std::vector<Player> players;
        Deck deck;
        std::vector<Card> discardPile;
        int currentPlayerIndex;
        bool reverseOrder;
        Color currentColor;
        Type currentType;


        void nextPlayer();
        void skipTurn();
        void reverseTurnOrder();
        void drawTwo();
        void wildDrawFour();
        bool isValidPlay(const Card &card) const;
        void shuffleDeck();
        std::string colorToString(Color color) const;

};
