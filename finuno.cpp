/* Final Lab - Uno
 *
 * Noelle Whidden, Raj Khot, Abbi Upchurch
 *
 * For our final lab we decided to create an Uno game. It allows for 2-6 players to play, deals them a hand of 7 random
 * cards, and keeps track of a discard pile. It correctly checks for the colors of discarded cards as well as face value,
 * and enforces the official rules of Uno throughout the game. 
 *
 * finuno.cpp runs almost the entirety of the game, including initializing the cards, deck, and players as well as the
 * gameplay. */ 

#include "uno.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <string>

// Card Class - this class contains functions that concern specific cards
Card::Card(Color color, Type type, int value)
    : color(color), type(type), value(value) {}

    // Assigns colors to the Uno cards as a string value
    std::string Card::colortoString(Color color) const {
        std::string colorStr;

        // Switch statements made it easier to assign the correct color to the cards over doing if/else statements
        switch (color) {
            case Color::RED: colorStr = "Red"; break;
            case Color::GREEN: colorStr = "Green"; break;
            case Color::BLUE: colorStr = "Blue"; break;
            case Color::YELLOW: colorStr = "Yellow"; break;
            case Color::NONE: colorStr = "None"; break;
            default: return "Invalid Color";
        }

        return colorStr;
    }

// Assigns the type (number/action cards) to each card
std::string Card::typetoString(Type type) const {
    std::string typeStr;
    switch (type) {
        case Type::NUMBER: typeStr = std::to_string(value); break;
        case Type::SKIP: typeStr = "Skip"; break;
        case Type::REVERSE: typeStr = "Reverse"; break;
        case Type::DRAW_TWO: typeStr = "Draw Two"; break;
        case Type::WILD: typeStr = "Wild"; break;
        case Type::WILD_DRAW_FOUR: typeStr = "Wild Draw Four"; break;
        default: return "Invalid Type";
    }
    
    return typeStr;
}

// Takes the color and type of each card and turns that description into a string
std::string Card::toString() const {
    return colortoString(color) + " " + typetoString(type);
}

// Returns the color of the card
Color Card::getColor() const {
    return color;
}

// Returns the type of the card
Type Card::getType() const {
    return type;
}

// Returns the value if it is a number card
int Card::getValue() const {
    if (type == Type::NUMBER) {
        return value;
    }
    return -1;
}

// Compares two cards to see if they are identical
bool Card::operator==(const Card &other) const {
    return color == other.color && type == other.type && value == other.value;
}

// Deck Class - this class contains functions that concern the deck as a whole
Deck::Deck() {
    initializeDeck();
}

// Uses the shuffle function to shuffle the deck
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

// Draws the top card off of the deck
Card Deck::draw() {
    if (isEmpty()) throw std::out_of_range("Deck is empty");
    Card drawnCard = cards.back();
    cards.pop_back();
    return drawnCard;
}

// Checks to see if the deck is empty
bool Deck::isEmpty() const {
    return cards.empty();
}

// Initializes all of the cards needed for an Uno deck
void Deck::initializeDeck() {
    // Clears the deck
    cards.clear();

    for (int i = 0; i < 4; i++) { 
        Color color = static_cast<Color>(i);

        // Numbers 1-9 (two per color)
        for (int number = 1; number <= 9; number++) {
            cards.push_back(Card(color, Type::NUMBER, number));
            cards.push_back(Card(color, Type::NUMBER, number));
        }

        // Action cards (two per color)
        for (int j = 0; j < 2; j++) {
            cards.push_back(Card(color, Type::SKIP, 0));
            cards.push_back(Card(color, Type::REVERSE, 0));
            cards.push_back(Card(color, Type::DRAW_TWO, 0));
        }
    }

    // Wild cards (four each)
    for (int i = 0; i < 4; i++) {
        cards.push_back(Card(Color::NONE, Type::WILD, 0));
        cards.push_back(Card(Color::NONE, Type::WILD_DRAW_FOUR, 0));
    }
}

// Player Class - this class contains functions concerning the players
Player::Player(const std::string &name) : playername(name) {}

// How the players draw cards from the deck
void Player::drawCard(Deck &deck) {
    hand.push_back(deck.draw());
}

// Shows the current player their hand
void Player::showHand() const {
    std::cout << "\n" << playername << "'s Hand:\n";
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << i << ": " << hand[i].toString() << std::endl;
    }
}

// Allows the current player to play a card
bool Player::playCard(const Card &card, std::vector<Card> &discardPile) {
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end()) {
        discardPile.push_back(card);
        hand.erase(it);
        return true;
    }
    return false;
}

// Returns the current player's name
std::string Player::getName() const {
    return playername;
}

// Checks if a player has won
bool Player::hasWon() const {
    return hand.empty();
}

// Returns the player's hand
const std::vector<Card>& Player::getHand() const {
    return hand;
}

// UnoGame Class - takes in players and starts game
UnoGame::UnoGame(int numPlayers) : currentPlayerIndex(0), reverseOrder(false), currentColor(Color::NONE), currentType(Type::NUMBER) {
    // Taking in the players' names, and starting the game
    for (int i = 0; i < numPlayers; ++i) {
        std::string playerName;
        std::cout << "Enter name for player " << (i + 1) << ": ";
        std::cin >> playerName;
        players.emplace_back(playerName);
    }
    shuffleDeck();
    startGame();
}

// Deals cards to players and draws the discard card
void UnoGame::startGame() {
    // Deals each player a hand of 7 cards
    for (auto& player : players) {
        for (int i = 0; i < 7; ++i) {
            player.drawCard(deck);
        }
    }

    // Draws the first card to start the game
    Card firstCard = deck.draw();
    
    // If the first card is a wild card, reshuffle the deck and draw a new card to start
    while (firstCard.getType() == Type::WILD || firstCard.getType() == Type::WILD_DRAW_FOUR) {
        deck.shuffle();
        deck.draw();
        firstCard = deck.draw();
    }
    discardPile.push_back(firstCard);
    currentColor = firstCard.getColor();
    currentType = firstCard.getType();
    currentValue = firstCard.getValue();
}

// Shuffles the deck
void UnoGame::shuffleDeck() {
    deck.shuffle();
}

// Controls the user interface
// Displays the current user's name, hand, and the card on the top of the discard pile
void UnoGame::showGameState() const {
    std::cout << "\nCurrent Player: " << players[currentPlayerIndex].getName() << std::endl;
    std::cout << "Discard Pile: " << discardPile.back().toString() << std::endl;
    players[currentPlayerIndex].showHand();
}

// Checks if the current player has won
bool UnoGame::checkWin() const {
    return players[currentPlayerIndex].hasWon();
}

// Assigns the color value of the current card as a string
std::string UnoGame::colorToString(Color color) const {
    switch (color) {
        case Color::RED: return "Red";
        case Color::GREEN: return "Green";
        case Color::BLUE: return "Blue";
        case Color::YELLOW: return "Yellow";
        default: return "Invalid Color";
    }
}

// Controls each player's turn
void UnoGame::takeTurn() {
    bool gameOver = false;  // Declare the game over flag locally

    // Ends if the game has been declared over
    if (gameOver) {
        std::cout << "The game is already over!" << std::endl;
        return;
    }

    Player &currentPlayer = players[currentPlayerIndex];

    bool validPlay = false;
    bool hasValidCard = false;

    // Check if the player has any valid cards
    for (const auto& card : currentPlayer.getHand()) {
        if (isValidPlay(card)) {
            hasValidCard = true;
            break;
        }
    }

    // If no valid card, prompt for a draw
    if (!hasValidCard) {
        std::cout << currentPlayer.getName() << ", you have no valid cards to play. Draw a card (y/Y): ";
        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            currentPlayer.drawCard(deck);
            std::cout << "You drew: " << currentPlayer.getHand().back().toString() << std::endl;

            // Check if the drawn card is valid
            if (isValidPlay(currentPlayer.getHand().back())) {
                std::cout << "You can play the card you drew." << std::endl;
            }
            else {
                std::cout << "You still have no valid cards to play. Skipping your turn." << std::endl;
                nextPlayer();
                return;
            }
        }
        else {
            std::cout << "Invalid input. Skipping your turn." << std::endl;
            nextPlayer();
            return;
        }
    }

    // Check if the player has only one card left
    if (currentPlayer.getHand().size() == 2) {
        std::cout << "\n" << currentPlayer.getName() << ": ";
        std::string unoResponse;
        std::cin >> unoResponse;

        // If the player doesn't say 'UNO', you can enforce a penalty or just print a message
        if (unoResponse != "UNO" && unoResponse != "uno") {
            std::cout << "You didn't say 'UNO'! You have been penalized." << std::endl;
            currentPlayer.drawCard(deck); 
        }
    }

    // Now allow the player to select a valid card to play
    while (!validPlay) {
        std::cout << "\n" << currentPlayer.getName() << ", choose a card to play (enter the card number): ";
        int cardIndex;
        std::cin >> cardIndex;

        if (std::cin.fail() || cardIndex < 0 || cardIndex >= currentPlayer.getHand().size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input, please try again.\n";
        }
        else {
            Card cardToPlay = currentPlayer.getHand()[cardIndex];

            if (isValidPlay(cardToPlay)) {
                currentPlayer.playCard(cardToPlay, discardPile);
                validPlay = true;

                currentColor = cardToPlay.getColor();
                currentType = cardToPlay.getType();
                currentValue = cardToPlay.getValue();

                // Handle special cards (Wilds, Skips, Reverses, etc.)
                if (cardToPlay.getType() == Type::WILD || cardToPlay.getType() == Type::WILD_DRAW_FOUR) {
                    std::cout << "Choose a color (Red, Green, Blue, Yellow): ";
                    std::string colorChoice;
                    std::cin >> colorChoice;
                    if (colorChoice == "Red" || colorChoice == "red") {
                        currentColor = Color::RED;
                    }
                    else if (colorChoice == "Green" || colorChoice == "green") {
                        currentColor = Color::GREEN;
                    }
                    else if (colorChoice == "Blue" || colorChoice == "blue") {
                        currentColor = Color::BLUE;
                    }
                    else if (colorChoice == "Yellow" || colorChoice == "yellow") {
                        currentColor = Color::YELLOW;
                    }
                    else {
                        std::cout << "Invalid color. Defaulting to Red." << std::endl;
                        currentColor = Color::RED;
                    }
                    std::cout << "The new color is " << colorToString(currentColor) << "." << std::endl;
                }

                // Check if the current player has won
                if (currentPlayer.hasWon()) {
                    std::cout << currentPlayer.getName() << " wins!\n";
                    gameOver = true; // End the game if someone has won
                    return;
                }

                // Special actions for cards like Skip, Reverse, Draw Two, Wild Draw Four
                if (currentType == Type::SKIP) {
                    skipTurn();
                }
                else if (currentType == Type::REVERSE) {
                    reverseTurnOrder();
                }
                else if (currentType == Type::DRAW_TWO) {
                    drawTwo();
                }
                else if (currentType == Type::WILD_DRAW_FOUR) {
                    wildDrawFour();
                }
            }

            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input, please try again.\n";

            }
        }
    }

    nextPlayer();
}

// Moves onto the next player for their turn, depending on if anyone has played a reverse card or not
void UnoGame::nextPlayer() {
    if (reverseOrder) {
        currentPlayerIndex = (currentPlayerIndex - 1 + players.size()) % players.size();
    } else {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }
}

// If someone plays a skip card
void UnoGame::skipTurn() {
    std::cout << "Next player's turn is skipped!\n";
    nextPlayer();
}

// If someone plays a reverse card
void UnoGame::reverseTurnOrder() {
    std::cout << "The turn order is reversed!\n";
    reverseOrder = !reverseOrder;
}

// If someone plays a draw 2 card
void UnoGame::drawTwo() {
    std::cout << "Next player draws two cards!\n";
    nextPlayer();
    players[currentPlayerIndex].drawCard(deck);
    players[currentPlayerIndex].drawCard(deck);
}

// If someone plays a wild draw 4 card
void UnoGame::wildDrawFour() {
    std::cout << "Next player draws four cards!\n";
    nextPlayer();
    for (int i = 0; i < 4; i++) {
        players[currentPlayerIndex].drawCard(deck);
    }
}

// Checks if the player can put down a card
bool UnoGame::isValidPlay(const Card &card) const {
    if (card.getColor() == currentColor || card.getColor() == Color::NONE) {
        return true;
    }

    if (card.getType() != Type::NUMBER) {
        if (card.getType() == currentType) {
            return true;
        }

    }

    else {
        if (card.getValue() == currentValue) {
            return true;
        }
    }

    return false;
}







