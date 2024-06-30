#ifndef __GAME_H__
#define __GAME_H__
#include "player.h"

typedef struct Game Game;

/*a functions that checks if an endgame condition is met,
and returns the index of the winning player if it is*/
typedef int (*endCondition)(int *  _scoreBoard);

typedef enum GameResults
{
    GG,
    GAME_UNINITIALIZED_ERROR,
    GAME_INVALID_PLAYERS,
    GAME_POINTER_NULL,
    GAME_ALLOCATION_FAILED,
    GAME_ROUND_FAIL

}GameResults;

/*
description - Creates a Game
input - number of players, how many human players there are,
 names array, id array and an endcondition function
output - a Game pointer on success or NULL on fail
error - NULL if either of the function pointers are NULL
        NULL if either of the arrays are NULL
        NULL if the number of players is 0 or if the number of human players
        exceeds the number of players dictated
        NULL if fails to allocate memory
*/
Game * CreateGame(size_t _numOfPlayers, size_t _humans, char * _names[],size_t _id[], endCondition _endCond);

/*
description - destroys a game
input - a game pointer address
output - none
error - none
*/
void DestroyGame(Game ** _game);

/*
description - runs the game untill an endgame condition is met
input - a game pointer
output - GG on Success, GAME_UNINITIALIZED_ERROR or GAME_ROUND_FAIL on Fail
error - GAME_UNINITIALIZED_ERROR if game address i NULL
        GAME_ROUND_FAIL if the round fails for some reason
*/
GameResults RunGame(Game * _game);

#endif /*_GAME_H__*/