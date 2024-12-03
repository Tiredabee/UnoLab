                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
#include "uno.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <string>


// Card Class
Card::Card(Color color, Type type, int value)
    : color(color), type(type), value(value) {}

    std::string Card::colortoString(Color color) const {
        std::string colorStr;
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

std::string Card::toString() const {
    return colortoString(color) + " " + typetoString(type);
}


Color Card::getColor() const {
    return color;
}

Type Card::getType() const {
    return type;
}

int Card::getValue() const {
    if (type == Type::NUMBER) {
        return value;
    }
    return -1;
}

bool Card::operator==(const Card &other) const {
    return color == other.color && type == other.type && value == other.value;
}


// Deck Class
Deck::Deck() {
    initializeDeck();
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}


Card Deck::draw() {
    if (isEmpty()) throw std::out_of_range("Deck is empty");
    Card drawnCard = cards.back();
    cards.pop_back();
    return drawnCard;
}

bool Deck::isEmpty() const {
    return cards.empty();
}

void Deck::initializeDeck() {
    cards.clear(); // clear the deck

    for (int i = 0; i < 4; i++) { // four colors
        Color color = static_cast<Color>(i);

        // 1-9 (two per color)
        for (int number = 1; number <= 9; number++) {
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

Player::Player(const std::string &name) : playername(name) {}

void Player::drawCard(Deck &deck) {
    hand.push_back(deck.draw());
}

void Player::showHand() const {
    std::cout << "\n" << playername << "'s Hand:\n";
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << i << ": " << hand[i].toString() << std::endl;
    }
}

bool Player::playCard(const Card &card, std::vector<Card> &discardPile) {
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end()) {
        discardPile.push_back(card);
        hand.erase(it);
        return true;
    }
    return false;
}

std::string Player::getName() const {
    return playername;
}

bool Player::hasWon() const {
    return hand.empty();
}

const std::vector<Card>& Player::getHand() const {
    return hand;
}



UnoGame::UnoGame(int numPlayers) : currentPlayerIndex(0), reverseOrder(false), currentColor(Color::NONE), currentType(Type::NUMBER) {
    for (int i = 0; i < numPlayers; ++i) {
        std::string playerName;
        std::cout << "Enter name for player " << (i + 1) << ": ";
        std::cin >> playerName;
        players.emplace_back(playerName);
    }
    shuffleDeck();
    startGame();
}


void UnoGame::startGame() {
    for (auto& player : players) {
        for (int i = 0; i < 7; ++i) {
            player.drawCard(deck);
        }
    }

    Card firstCard = deck.draw();
    while (firstCard.getType() == Type::WILD || firstCard.getType() == Type::WILD_DRAW_FOUR) {
        deck.shuffle();
        deck.draw();
        firstCard = deck.draw();
    }
    discardPile.push_back(firstCard);
    currentColor = firstCard.getColor();
    currentType = firstCard.getType();
}

void UnoGame::shuffleDeck() {
    deck.shuffle();
}

void UnoGame::showGameState() const {
    std::cout << "\nCurrent Player: " << players[currentPlayerIndex].getName() << std::endl;
    std::cout << "Discard Pile: " << discardPile.back().toString() << std::endl;
    players[currentPlayerIndex].showHand();
}

bool UnoGame::checkWin() const {
    return players[currentPlayerIndex].hasWon();
}

std::string UnoGame::colorToString(Color color) const {
    switch (color) {
        case Color::RED: return "Red";
        case Color::GREEN: return "Green";
        case Color::BLUE: return "Blue";
        case Color::YELLOW: return "Yellow";
        default: return "Invalid Color";
    }
}



void UnoGame::takeTurn() {
    bool gameOver = false;  // Declare the game over flag locally

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
            // Player draws a card
            currentPlayer.drawCard(deck);
            std::cout << "You drew: " << currentPlayer.getHand().back().toString() << std::endl;

            // Check if the drawn card is valid
            if (isValidPlay(currentPlayer.getHand().back())) {
                std::cout << "You can play the card you drew." << std::endl;
            } else {
                std::cout << "You still have no valid cards to play. Skipping your turn." << std::endl;
                nextPlayer();
                return;
            }
        } else {
            std::cout << "Invalid input. Skipping your turn." << std::endl;
            nextPlayer();
            return;
        }
    }

    // Check if the player has only one card left
    if (currentPlayer.getHand().size() == 1) {
        std::cout << currentPlayer.getName() << ", you have one card left! Say 'UNO' to continue." << std::endl;
        std::string unoResponse;
        std::cin >> unoResponse;

        // If the player doesn't say 'UNO', you can enforce a penalty or just print a message
        if (unoResponse != "UNO" && unoResponse != "uno") {
            std::cout << "You didn't say 'UNO'! You have been penalized." << std::endl;
            // Implement penalty logic, e.g., drawing a card, skipping a turn, etc.
            currentPlayer.drawCard(deck);  // Example of a penalty (draw a card)
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
        } else {
            Card cardToPlay = currentPlayer.getHand()[cardIndex];

            if (isValidPlay(cardToPlay)) {
                currentPlayer.playCard(cardToPlay, discardPile);
                validPlay = true;

                currentColor = cardToPlay.getColor();
                currentType = cardToPlay.getType();

                // Handle special cards (Wilds, Skips, Reverses, etc.)
                if (cardToPlay.getType() == Type::WILD || cardToPlay.getType() == Type::WILD_DRAW_FOUR) {
                    std::cout << "Choose a color (Red, Green, Blue, Yellow): ";
                    std::string colorChoice;
                    std::cin >> colorChoice;
                    if (colorChoice == "Red") {
                        currentColor = Color::RED;
                    } else if (colorChoice == "Green") {
                        currentColor = Color::GREEN;
                    } else if (colorChoice == "Blue") {
                        currentColor = Color::BLUE;
                    } else if (colorChoice == "Yellow") {
                        currentColor = Color::YELLOW;
                    } else {
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
                } else if (currentType == Type::REVERSE) {
                    reverseTurnOrder();
                } else if (currentType == Type::DRAW_TWO) {
                    drawTwo();
                } else if (currentType == Type::WILD_DRAW_FOUR) {
                    wildDrawFour();
                }
            }
        }
    }

    nextPlayer();
}






void UnoGame::nextPlayer() {
    if (reverseOrder) {
        currentPlayerIndex = (currentPlayerIndex - 1 + players.size()) % players.size();
    } else {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }
}

void UnoGame::skipTurn() {
    std::cout << "Next player's turn is skipped!\n";
    nextPlayer();
}

void UnoGame::reverseTurnOrder() {
    std::cout << "The turn order is reversed!\n";
    reverseOrder = !reverseOrder;
}

void UnoGame::drawTwo() {
    std::cout << "Next player draws two cards!\n";
    nextPlayer();
    players[currentPlayerIndex].drawCard(deck);
    players[currentPlayerIndex].drawCard(deck);
}

void UnoGame::wildDrawFour() {
    std::cout << "Next player draws four cards!\n";
    nextPlayer();
    for (int i = 0; i < 4; i++) {
        players[currentPlayerIndex].drawCard(deck);
    }
}


bool UnoGame::isValidPlay(const Card &card) const {
    // Get the current card color, type, and value from the discard pile
    Color currentColor = discardPile.back().getColor();
    Type currentType = discardPile.back().getType();
    int currentValue = discardPile.back().getValue();

    // If the card is Wild or Wild Draw Four, it's always valid
    if (card.getType() == Type::WILD || card.getType() == Type::WILD_DRAW_FOUR) {
        return true;
    }

    // Check for a match in color or value
    if (card.getColor() == currentColor || card.getValue() == currentValue) {
        return true;
    }

    // If it’s a special type (like Skip, Reverse, Draw Two) that matches the type, allow it
    if (card.getType() == currentType) {
        return true;
    }

    // If no match in color, value, or type, it’s not a valid play
    return false;
}






