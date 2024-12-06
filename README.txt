RUNNING:
To run this program, run this command, g++ -o uno finuno.cpp finmain.cpp, and then run ./uno.

RUlES: 
When prompted pick a number from 2 through 6 for the number of players you want, and then give the names for each individual player.
A card will be drawn from the deckPlayer 1 will go first and each persons turn is will go in increasing numerical order, and when reaches the 
last person, will go back to Player 1.
With any Card, you must put down a card with the same number, color, or type. For example, if you have a green 2 in the discard pile, you may
place a green 4 as its the same color or blue 2 as its the same number. The same applies to special cards as well. If you were to put a
green skip on green 2 or a green +2 that would also count. You may also put a blue +2 on a green +2 or a red skip on a yellow skip
as they are the same type of cards. 
For this game of uno you are not allowed to stack +2's or Wild +4's so if the player beofre your turn placed a blue +2 and you had a 
yellow +2, you are not allowed to put it down to make the next player pick up 4. The same applies to Wild +4's.
When dealing with Wild Cards, you can put down a wild card , +4 or regular, on any type of card in the discard pile and then choose a new 
color. 
If you do not have any valid cards, you will be prompted to pick a card from the deck. If it can be played you are allowed to play it.
If not your turn will be skipped. If you are forced to pick up 2 or 4 cards from the deck from a +2 card or from a Wild +4 card your
turn will also be skipped, and you will not be allowed to put down a card if it is valid. 
If you have 2 cards and can place one of them down , you will be prompted with a input on the terminal where you will need to put in UNO.
If you do not put in UNO or uno, you will be penealized by drawing a card. 
If a player puts down a skip card, it will skip the player after them and go on to the next player. 
If a player puts down a reverse card, it will reverse the order such as if going clockwise, it will go counter-clockwise. 
