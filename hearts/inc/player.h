#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <stddef.h>
#include "vector.h"

typedef struct Player Player;

typedef enum PlayerADT
{
    PLAYER_SUCCESS,
    PLAYER_UNINITIALIZED_ERROR,
    PLAYER_INVALID_POINTER,
    PLAYER_HAND_DOUBLE,
    PLAYER_NO_HAND,
    PLAYER_NO_CARDS,
    PLAYER_INVALID_INDEX
}PlayerADT;

typedef enum PlayerType
{
    HUMAN,
    ROBOT
}PlayerType;

/*checks if a card abides by the rules*/
typedef int (*Rules)(int _card, Vector * _hand, int * _table, void * _context);

/*a strategy function for computer players*/
typedef int (*Strategy)(Vector * _cardsArray, int * _table, void * _context);

/*
description - Creates a Player 
input - a players name, player type, and a players id
player type: HUMAN for human players or ROBOT for a computer player
output - a player pointer if successful or NULL if not
error - NULL if the name pointer is NULL
        NULL if player type is not valid
        NULL if memory allocation fails
*/
Player * CreatePlayer(char * _name, PlayerType _type, size_t _id);

/*
description - Destroys a Player struct 
input - a player pointer address
output - none
error - none
*/
void DestroyPlayer(Player **_player);

/*
description - creates an empty hand for the player  
input - a player pointer and handsize
output - PLAYER_SUCCESS if successful, PLAYER_INVALID_POINTER, PLAYER_HAND_DOUBLE,
 and  PLAYER_UNINITIALIZED_ERROR if not
error - PLAYER_INVALID_POINTER if the player address is NULL
      - PLAYER_HAND_DOUBLE if there is already an existing hand
      - PLAYER_UNINITIALIZED_ERROR if the memory allocation fails 
*/
PlayerADT CreateHand(Player * _player, size_t _handSize);

/*
description - destroys a players hand
input - a player pointer
output - PLAYER_SUCCESS if successful, PLAYER_INVALID_POINTER and PLAYER_NO_HAND if not
error - PLAYER_INVALID_POINTER if the player address is NULL
      - PLAYER_NO_HAND if there is no hand in the player's struct 
*/
PlayerADT DestroyHand(Player * _player);

/*
description - recieves a card to hand
input - a player pointer, and a card in int format
output - PLAYER_SUCCESS if successful, PLAYER_INVALID_POINTER, PLAYER_NO_HAND
         and PLAYER_UNINITIALIZED_ERROR if not
error - PLAYER_INVALID_POINTER if the player address is NULL
      - PLAYER_NO_HAND if there is no hand in the player's struct 
      - PLAYER_UNINITIALIZED_ERROR if the card fails to be added to the hand
*/
PlayerADT GetCard(Player * _player, int _card);

/*
description - recieves a card from a player
input - a player pointer, a pointer to a table address, rules function, strategy function and context pointer
output - a card in int format or -1 if not successful
error - -1 if either of the pointers are NULL
        -1 if there is no hand or no cards
        -1 if fails to removecard
*/
int GiveCard(Player * _player, int * _table, Rules _rFunc, void * _context, Strategy _sFunc);

/*
description - sorts a players hand by ascending suit and rank
input - a player pointer
output -PLAYER_OK if successful, PLAYER_INVALID_POINTER, PLAYER_NO_HAND
         and PLAYER_NO_CARDS if not
error - PLAYER_INVALID_POINTER if player address is NULL
        PLAYER_NO_HAND if there is no hand in the player's struct
        PLAYER_NO_CARDS if there are no cards in hand

*/
PlayerADT SortHand(Player * _player);

/*
description - brings a card in hand without removing it
input - a player pointer and a card index
output - a card in int format if successful, -1 if not
         and PLAYER_NO_CARDS if not
error - -1 if of the pointer is NULL
        -1 if there is no hand or no cards
        -1 the index is invalid
*/
int ShowCard(Player * _player, size_t _index);

/*
description - returns a player's handsize
input - a player pointer
output - the player's handsize or 0 on fail
error - 0 if pointer is NULL
        0 if there is no hand
*/
size_t HandSize(Player * _player);

/*
description - returns a player's name
input - a player pointer
output - the player's name or NULL on fail
error - NULL if the players pointer is NULL
*/
const char * ShowPlayerName(Player * _player);

#endif /*__PLAYE_H__*/