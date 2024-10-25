#include <iostream>
#include <vector>
#include <string>
#include <algorithim>
#include <cstdlib>
#include <ctime>

enum class COLOR { RED, GREEN, BLUE, YELLOW, NONE };

enum class TYPE { NUMBER, SKIP, REVERSE, DRAW_TWO, WILD, WILD_DRAW_FOUR };

class Card {
  public:
      Card(Color color, Type type, int value = -1);
      std::string toString() const;
      Color getColor() const;
      Type getType() const;

  private:
      Color color;
      Type type;
      int value;
}

class Deck {
  public:
      Deck();
      void shuffle();
      Card draw();
      bool isEmpty() const;

  private:
      std::vector<Card> cards;
      void initializeDeck();
}

class Player {
  public:
    Player(const std:: string &name);
    void drawCard(Deck &deck);
    void showHand() const;
    std::string getName() const;

  private:
      std::vector<Card> hand;
}
