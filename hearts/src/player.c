#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "player.h"
#include "heap.h"
#include "ui.h"
#define TRUE 1
#define FALSE 0
#define HAND_SIZE 13



struct Player
{
    char * m_name;
    Vector * m_hand;
    size_t m_nCards;
    size_t m_id;
    PlayerType m_type;
};

static PlayerADT CheckCHParameters(Player *_player);
static PlayerADT CheckPGetCardParameters(Player * _player);
static PlayerADT CheckSHParameters(Player * _player, size_t _index);
static PlayerADT CheckDHParameters(Player *_player);
static PlayerADT CheckPlayerGiveCardParameters(Player * _player, int * _table, Rules _rFunc, void * _context, Strategy _sFunc);
static Vector * FilterValidCards(Player * _player,int * _table, void * _context, Rules _rFunc);
static size_t FindCardIndex(Vector * _hand, int _card);
static void Swap(Vector * _vec, size_t _index1, size_t _index2);


Player * CreatePlayer(char * _name, PlayerType _type, size_t _id)
{
    Player * temp;
    if (_name == NULL || _type > ROBOT || _type < HUMAN )
    {
        return NULL;
    }
    if((temp = (Player*)malloc(sizeof(Player))) == NULL)
    {
        return NULL;
    }
    temp->m_name = _name;
    temp->m_id = _id;
    temp->m_hand = NULL;
    temp->m_nCards = 0;
    temp->m_type = _type;
    return temp;
}

void DestroyPlayer(Player **_player)
{
    if (_player == NULL || *_player == NULL)
    {
        return;
    }
    VectorDestroy((*_player)->m_hand);
    free(*_player);
    *_player = NULL;
}

PlayerADT CreateHand(Player * _player, size_t _handSize)
{
    PlayerADT check;
    Vector * temp;
    if ((check = CheckCHParameters(_player)) != PLAYER_SUCCESS)
    {
        return check;
    }
    if((temp = VectorCreate(_handSize, 1)) == NULL)
    {
        return PLAYER_UNINITIALIZED_ERROR;
    }
    _player->m_hand = temp;
    _player->m_nCards = 0;
    return check;
}

PlayerADT DestroyHand(Player * _player)
{
    PlayerADT check;
    if((check = CheckDHParameters(_player)) != PLAYER_SUCCESS)
    {
        return check;
    }
    VectorDestroy(_player->m_hand);
    _player->m_hand = NULL;
    return check;
}


PlayerADT GetCard(Player * _player, int _card)
{
    PlayerADT check;
    if ((check = CheckPGetCardParameters(_player)) != PLAYER_SUCCESS)
    {
        return check;
    }
    /*TODO: maybe it's not the player module responsibility
    to check the validity of cards*/
    if(_card == -1)
    {
        return PLAYER_UNINITIALIZED_ERROR;
    }
    if(VectorAdd(_player->m_hand, _card) != ERR_OK)
    {
        return PLAYER_UNINITIALIZED_ERROR;
    }

    ++_player->m_nCards;
    return PLAYER_SUCCESS;
}




/*TODO: break into functions*/
/*TODO: fix this broken thing*/
int GiveCard(Player * _player, int * _table, Rules _rFunc, void * _context, Strategy _sFunc)
{
    int card, tempCard;
    Vector * tempVec;
    size_t index;
    if(CheckPlayerGiveCardParameters(_player, _table,  _rFunc, _context, _sFunc) != PLAYER_SUCCESS)
    {
        return -1;
    }
    /*TODO: maybe break into 2 functions: Human turn and Robot turn*/
    if(_player->m_type == ROBOT)
    {
        if ((tempVec = FilterValidCards(_player, _table, _context, _rFunc)) == NULL)
        {
            return -1;
        }
        tempCard = _sFunc(tempVec, _table,_context);
        index = FindCardIndex(_player->m_hand, tempCard);
        Swap(_player->m_hand, index, _player->m_nCards - 1);
        VectorDelete(_player->m_hand, &card);
        VectorDestroy(tempVec);
    }
    else 
    {
        PlayerPrompt(TRUE, _player);
        PrintHand(_player);
        scanf("%lu", &index);
        VectorGet(_player->m_hand, index, &tempCard);
        while(index >= _player->m_nCards || _rFunc(tempCard, _player->m_hand, _table, _context) != TRUE)
        {
            PlayerPrompt(FALSE, _player);
            scanf("%lu", &index);
            VectorGet(_player->m_hand, index, &tempCard);
        }
        Swap(_player->m_hand, index, _player->m_nCards - 1);
        VectorDelete(_player->m_hand, &card);
    }
    --_player->m_nCards;
    SortHand(_player);
    return card;
}

PlayerADT SortHand(Player * _player)/*maybe print hand each time we sort hand?*/
{
    Heap * temp;
    PlayerADT check;
    if ((check = CheckSHParameters(_player, _player->m_nCards - 1)) != PLAYER_SUCCESS)
    {
        return check;
    }
    temp = HeapBuild(_player->m_hand);
    HeapSort(temp);
    HeapDestroy(temp);
    return check;
}

int ShowCard(Player * _player, size_t _index)
{
    PlayerADT check;
    int card;
    if ((check = CheckSHParameters(_player, _index)) != PLAYER_SUCCESS)
    {
        return check;
    }
    VectorGet(_player->m_hand, _index, &card);
    return card;

}

size_t HandSize(Player * _player)
{
    if(_player == NULL || _player->m_hand == NULL)
    {
        return 0;
    }
    return _player->m_nCards;
}

const char * ShowPlayerName(Player * _player)
{
    if(_player == NULL)
    {
        return NULL;
    }
    return _player->m_name;
}
/*----------------------------------------------------------------------------------------------------*/

static size_t FindCardIndex(Vector * _hand, int _card)
{
    register size_t i;
    size_t size;
    int tempCard;
    VectorItemsNum(_hand, &size);
    for(i = 0; i < size; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(tempCard == _card)
        {
            break;
        }
    }
    return i;
}

static void Swap(Vector * _vec, size_t _index1, size_t _index2)
{
    int temp1, temp2;
    VectorGet(_vec, _index1, &temp1);
    VectorGet(_vec, _index2, &temp2);
    VectorSet(_vec, _index1, temp2);
    VectorSet(_vec, _index2, temp1);
}

static PlayerADT CheckCHParameters(Player *_player)
{
    if(_player == NULL)
    {
        return PLAYER_INVALID_POINTER;
    }
    if(_player->m_hand != NULL)
    {
        return PLAYER_HAND_DOUBLE;
    }
    return PLAYER_SUCCESS;
}

static PlayerADT CheckPGetCardParameters(Player * _player)
{
    if (_player == NULL)
    {
        return PLAYER_INVALID_POINTER;
    }
    if(_player->m_hand == NULL)
    {
        return PLAYER_NO_HAND;
    }
    return PLAYER_SUCCESS;
}

static PlayerADT CheckPlayerGiveCardParameters(Player * _player, int * _table, Rules _rFunc, void * _context, Strategy _sFunc)
{
    if (!_player || !_rFunc || !_sFunc || !_context)
    {
        return PLAYER_INVALID_POINTER;
    }
    if(_player->m_hand == NULL )
    {
        return PLAYER_NO_HAND;
    }
    if(_player->m_nCards == 0)
    {
        return PLAYER_NO_CARDS;
    }
    return PLAYER_SUCCESS;
}

static PlayerADT CheckSHParameters(Player * _player, size_t _index)
{
    if(_player == NULL)
    {
        return PLAYER_INVALID_POINTER;
    }
    if(_player->m_hand == NULL)
    {
        return PLAYER_NO_HAND;
    }
    if(_player->m_nCards == 0)
    {
        return PLAYER_NO_CARDS;
    }
    if(_index >= _player->m_nCards)
    {
        return PLAYER_INVALID_INDEX;
    }
    return PLAYER_SUCCESS;
}

static PlayerADT CheckDHParameters(Player *_player)
{
    if(_player == NULL)
    {
        return PLAYER_INVALID_POINTER;
    }
    if(_player->m_hand == NULL)
    {
        return PLAYER_NO_HAND;
    }
    return PLAYER_SUCCESS;
}


static Vector * FilterValidCards(Player * _player,int * _table, void * _context, Rules _rFunc)
{
    Vector * temp;
    size_t i;
    int card;
    if((temp = VectorCreate(_player->m_nCards,0)) == NULL)
    {
        return NULL;
    }
    for(i = 0; i < _player->m_nCards; ++i)
    {
        VectorGet(_player->m_hand, i, &card);
        if(_rFunc(card, _player->m_hand ,_table, _context) == TRUE)
        {
            if (VectorAdd(temp, card) != ERR_OK ) {
                VectorDestroy(temp);
                return NULL;
            }
        }
    }
    return temp;    
}