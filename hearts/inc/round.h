#ifndef __ROUND_H__
#define __ROUND_H__

typedef enum RoundErrors
{
    ROUND_OK,
    ROUND_POINTER_NULL,
    ROUND_INVALID_PLAYERS,
    ROUND_INVALID_NUMBER,
    ROUND_UNITIALIZED_ERROR,
    ROUND_DEAL_ERROR,
    ROUND_INVALID_ROUND_NUMBER
}RoundErrors;

typedef struct Round Round;
typedef struct RoundInfo RoundInfo;

/*
description - creates a game round 
input - an array of players, a scoreboard address, and the number of players
output - Round type pointer if successful, NULL if not
error - NULL if either of the pointers are NULL
        NULL if the number of players is not 4
        NULL if fails to allocate memory for the struct
*/
Round * CreateRound(Player ** _playerlist, int _scoreBoard[], size_t _numOfPlayers);

/*
description - frees a game round 
input - an address of a round pointer
output - none
error - none
*/
void DestroyRound(Round ** _round);

/*
description - runs one game round 
input - a round address and the number of current round
output - ROUND_OK if successful, ROUND_POINTER_NULL or ROUND_UNITIALIZED_ERROR if not 
error - ROUND_POINTER_NULL if the round address is NULL
        ROUND_UNITIALIZED_ERROR if there is an existing Info allocated to memory or if failed to allocate said Info
*/
RoundErrors RunRound(Round * _round, size_t _roundNum);

/*
description - checks a round score to decide if the game is over
input - a scoreboard array
output - -1 if the game is not over, or the index of the winning player or 0 if the pointer is NULL 
error - 0 if the scoreboard array is NULL
*/
int WinCondition(int _scoreBoard[]);



#endif /*_ROUND_H__*/