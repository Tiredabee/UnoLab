#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

enum class Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    NONE
};

enum class Type {
    NUMBER,
    SKIP,
    REVERSE,
    DRAW_TWO,
    WILD,
    WILD_DRAW_FOUR
};

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
