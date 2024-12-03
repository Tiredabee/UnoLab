#include "uno.h"
#include <vector>
#include <algorithm>
#include <cstdlib>


// Card Class


// Deck Class
Deck::Deck() {

    initializeDeck();

}

void Deck::shuffle() {
    random_shuffle(cards.begin(), cards.end());
}

Card Deck::draw() {
    if (isEmpty())
        return Card(COLOR::NONE, TYPE::WILD, -1);

    Card topCard = cards.back(); // Get the last card
    cards.pop_back(); // Remove from the deck
    return topCard;
}

bool isEmpty() const {
    return cards.size() == 0;
}

void Deck::initializeDeck() {
    cards.clear(); // clear the deck

    for (int i = 0; i < 4; i++) { // four colors
        Color color = static_cast<COLOR>(i);

        // 0 (one per color)
        cards.push_back(Card(color, TYPE::NUMBER, 0));

        // 1-9 (two per color)
        for (int number = 1; number <= 9; number++) {
            cards.push_back(Card(color, TYPE::NUMBER, number));
            cards.push_back(Card(color, TYPE::NUMBER, number));
        }

        // Action cards (two per color)
        for (int j = 0; j < 2; j++) {
            cards.push_back(Card(color, TYPE::SKIP));
            cards.push_back(Card(color, TYPE::REVERSE));
            cards.push_back(Card(color, TYPE::DRAW_TWO));
        }
    }

    // Wild cards (four each)
    for (int i = 0; i < 4; i++) {
        cards.push_back(Card(COLOR::NONE, TYPE::WILD));
        cards.push_back(Card(COLOR::NONE, TYPE::WILD_DRAW_FOUR));
    }
}

Player::Player(const std::string &name) : playerName(name) {}

void Player::drawCard(Deck &deck) {
    if (!deck.isEmpty()) {
        Card drawnCard = deck.draw();
        hand.push_back(drawnCard);
    }
}

void Player::showHand() const {
    std::cout << playerName << "'s hand:" << std::endl;
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << i + 1 << ". " << hand[i].toString() << std::endl;
    }
}

std::string Player::getName() const {
    return playerName;
}                                              
