#ifndef __UI_H__
#define __UI_H__
#include <stddef.h>
#include "vector.h"
#include "player.h"
#include "round.h"
#include "card.h"


/*
description - Prints a card
input - a card in int format
output - prints the card to the stdoutput
error - none
*/
void PrintCard(int _card);

/*
description - Prints a players hand
input - a player address
output - prints players hand to the stdoutput
error - none
*/
void PrintHand(Player * _player);

/*
description - Prints a tables content
input - a table address and the number of cards on the table
output - prints players hand to the stdoutput
error - none
*/
void PrintTable(int * _table, int _tableSize);

/*
description - Prints score
input - an array of players, the scoreboard address and the number of players
output - prints the score of each player to the stdoutput
error - none
*/
void PrintScore(Player ** _players, int * _scoreBoard, size_t _numOfPlayers);

/*
description - Prints a prompt to the human player to play
input - a case and a player address
case meaning: TRUE if prompted for the first time, and FALSE if the player gave an invalid card index
output - prints a prompt to the stdoutput
error - none
*/
void PlayerPrompt(int _case, Player * _player);

/*
description - Prints a congratulation
input - the winning player's address
output - prints a message to the stdoutput
error - none
*/
void PrintWinner(Player * _player, int * _scoreBoard);

#endif /*__UI_H__*/