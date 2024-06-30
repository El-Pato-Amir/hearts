#include <stdlib.h>
#include "ui.h"
#include "game.h"
#include "round.h"

struct Game
{
    Player ** m_players;
    size_t m_numOfPlayers;
    size_t m_nRound;
    int * m_score;
    Round * m_round;
    endCondition m_endCondition;
};

static GameResults CheckCGParameters(size_t _numOfPlayers, size_t _humans, char * _names[],size_t _id[], endCondition _endCond);
static GameResults CreateGamePlayers(Game * _game, size_t _numOfPlayers, size_t _humans, char * _names[], size_t _id[]);


Game * CreateGame(size_t _numOfPlayers, size_t _humans, char * _names[],size_t _id[], endCondition _endCond)
{
    Game * tempGame;
    /*TODO: make all the allocations in different functions*/
    if(CheckCGParameters(_numOfPlayers, _humans, _names, _id, _endCond) != GG)
    {
        return NULL;
    }
    if((tempGame = (Game*)malloc(sizeof(Game))) == NULL)
    {
        return NULL;
    }
    if((tempGame->m_players = (Player**)malloc(sizeof(Player*)*_numOfPlayers)) == NULL)
    {
        free(tempGame);
        return NULL;
    }
    if((tempGame->m_score = (int*)calloc(sizeof(int), _numOfPlayers)) == NULL)
    {
        free(tempGame->m_players);
        free(tempGame);
        return NULL;
    }
    if(CreateGamePlayers(tempGame, _numOfPlayers, _humans, _names, _id) != GG)
    {
        free(tempGame->m_score);
        free(tempGame->m_players);
        free(tempGame);
        return NULL;
    }
    if((tempGame->m_round = CreateRound(tempGame->m_players, tempGame->m_score,_numOfPlayers)) == NULL)
    {
        free(tempGame->m_score);
        free(tempGame->m_players);
        free(tempGame);
        return NULL;
    }
    tempGame->m_numOfPlayers = _numOfPlayers;
    tempGame->m_nRound = 0;
    tempGame->m_endCondition = _endCond;
    return tempGame;
}

void DestroyGame(Game ** _game)
{
    size_t numOfPlayers;
    Round * roundAddress = (*_game)->m_round;
    register size_t i;
    if(_game == NULL || *_game == NULL)
    {
        return;
    }
    numOfPlayers = (*_game)->m_numOfPlayers;
    for(i = 0; i < numOfPlayers; ++i)
    {
        DestroyPlayer(&((*_game)->m_players[i]));
    }
    free((*_game)->m_players);
    free((*_game)->m_score);
    DestroyRound(&roundAddress);
    free(*_game);
    *_game = NULL;
    return;
}

GameResults RunGame(Game * _game)
{
    int winner;
    if(_game == NULL)
    {
        return GAME_UNINITIALIZED_ERROR;
    }
    while((winner = _game->m_endCondition( _game->m_score)) == -1)
    {
        if(RunRound(_game->m_round, _game->m_nRound) != ROUND_OK)
        {
            return GAME_ROUND_FAIL;
        }
        ++_game->m_nRound;
    }
    PrintWinner(_game->m_players[winner], _game->m_score);
    return GG;
}

static GameResults CheckCGParameters(size_t _numOfPlayers, size_t _humans, char * _names[],size_t _id[], endCondition _endCond)
{
    if( _endCond == NULL || _names == NULL || _id == NULL)
    {
        return GAME_POINTER_NULL;
    }
    if(_numOfPlayers == 0 || _humans > _numOfPlayers)
    {
        return GAME_INVALID_PLAYERS;
    }
    return GG;
}

static GameResults CreateGamePlayers(Game * _game, size_t _numOfPlayers, size_t _humans, char * _names[], size_t _id[])
{
    register size_t i;
    Player *tempPlayer;
    /*TODO: make a function for each of these loops*/
    for(i = 0; i < _humans; ++i)
    {
        if ((tempPlayer = CreatePlayer(_names[i], HUMAN, _id[i])) == NULL)
        {
            return GAME_UNINITIALIZED_ERROR;
        }
        _game->m_players[i] = tempPlayer;
    }
    for(i = _humans; i < _numOfPlayers; ++i)
    {
        if((tempPlayer = CreatePlayer(_names[i], ROBOT, _id[i])) == NULL)
        {
            return GAME_UNINITIALIZED_ERROR;
        }
        _game->m_players[i] = tempPlayer;
    }
    return GG;    
}

