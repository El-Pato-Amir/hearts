#include <stdlib.h>
#include "game.h"
#include "deck.h"
#include "card.h"
#include "player.h"
#include "vector.h"
#include "round.h"
#include "ui.h"
#define END_GAME 100
#define NUM_OF_PLAYERS 4
#define HAND_SIZE 13
#define MOVE(player,roundNum) (player + roundNum + roundNum + 1) % 4
#define PLAYER(j,n) (j + n) % NUM_OF_PLAYERS
#define ROUND_NUM(i) i % 4
#define QUEEN_S(i) (SUIT(i) == SPADE && RANK(i) == QUEEN)
#define TRUE 1
#define FALSE 0
#define SAFE 1
#define NOT_SAFE -1
#define NO_SUIT 0
#define CARD_CHANCE(card, cards) (cards / cards) * 100
#define CARD_INDEX(card) RANK(card) + SUIT(card)*NUM_OF_RANK

/*we dont get any access to the player struct! you need to change each instance of player members to player api*/

struct Round
{
    Player ** m_list;
    int * m_table;
    int * m_score;
    RoundInfo * m_info;
};

struct RoundInfo
{
    int * m_whichCardsPlayed;
    float * m_suitsPlayed;
    int m_brokenHeart;
    int m_queenOfSpades;
    float m_nCardsPlayed;
    Suit m_currSuit;
    int m_numCardsOnTable;
    int m_trickNum;
};


/*assit functions------------------------------------------------------------*/
static RoundErrors PlayTrick(Round * _round, size_t _playerNum, int _trickStartMark);
static void FinishRound(Round * _round);
static RoundErrors CreateAllHands(Player ** _players);
static RoundErrors DealCards(Player * _player, Deck * _deck);
int TrickRules(int _card, Vector  * _hand, int * _table, void * _context);
int TrickStrategy(Vector * _hand, int * _table, void * _context);
int SwapRules(int _card, Vector * _hand, int * _table, void * _context);
int SwapStrategy(Vector * _hand, int * _table, void * _info);
static void UpdateInfo(int _card, RoundInfo * _info);
static RoundErrors DealHand(Player ** _players);
static RoundErrors SwapCards(Player ** _players, size_t _roundNum);
static int FindTwoClubs(Player ** _players);
static int CalculateScore(Round * _round, int * _tempScore, int _startMark);
static void DestroyRoundInfo(Round * _round);
static RoundInfo* CreateRoundInfo();
/*static void CheckForMoon(int * _tempScore);*/
/*static void UpdateScore(Round * _round, int *tempScore);*/
static int StarterStrategy(Vector * _hand, int * _table,RoundInfo * _info);
static int DefineTrickState(Vector * _hand, int * _table, RoundInfo * _info);
static int FindBestCard(Vector * _hand, RoundInfo * _info, int * _table);
static int FindSafestCard(Vector * _hand, RoundInfo * _info, int * _table);
static int FindStinkiestCard(Vector * _hand);
static void CheckHand(Vector* _hand, size_t _handSize,int * suits, char * isQueenSInsHand);
static int IsNoSuit(int * suits);
static void CheckTable(int * _table, size_t * _heartsInTable, char * _isQSInTable);
static int IsSafish(Vector * _hand, RoundInfo * _info , Suit _suit);
static int IsKingsAndAces(int * _table);
static int FindHighestCardInTable(int *_table);
static int PotentialCards(Vector * _hand, RoundInfo * _info, int _index1, int _index2);
static int IsInHand(Vector * _hand, int _card);
static void CleanTable(Round * _round);

/*check parameters function------------------------------------------------*/
static RoundErrors CheckRoundCorrectnes(Round * _round, size_t _roundNum);
static RoundErrors CheckCRParameters(Player ** _playerlist, int _scoreBoard[], size_t _numOfPlayers);
static RoundErrors CheckSCParameters(Player ** _players, size_t _roundNum);
static RoundErrors CheckRunRoundParameters(Round * _round, size_t _roundNum);

/*----------------------------------*/
Round * CreateRound(Player ** _playerlist, int * _scoreBoard, size_t _numOfPlayers)
{
    Round * temp;
    if((CheckCRParameters(_playerlist, _scoreBoard, _numOfPlayers)) != ROUND_OK)
    {
        return NULL;
    }
    if((temp = (Round*)malloc(sizeof(Round))) == NULL)
    {
        return NULL;
    }
    if((temp->m_table = (int*)malloc(NUM_OF_PLAYERS*sizeof(int))) == NULL)
    {
        free(temp);
        return NULL;
    }
    temp->m_list = _playerlist;
    temp->m_score = _scoreBoard;
    temp->m_info = NULL;
    return temp;
}

void DestroyRound(Round ** _round)
{
    if(_round == NULL || *_round == NULL)
    {
        return;
    }
    DestroyRoundInfo((*_round));
    free((*_round)->m_table);
    free(*_round);
    *_round = NULL;
}

RoundErrors RunRound(Round * _round, size_t _roundNum)
{
    RoundErrors check;
    int trickStartMark;
    register size_t trickNum, player;
    /*TODO: fix this broken thing*/
    if ((check = CheckRoundCorrectnes(_round, _roundNum)) != ROUND_OK)
    {
        return check;
    }
    DealHand(_round->m_list);/*need printing*/
    if(ROUND_NUM(_roundNum) != 3)
    {
        SwapCards(_round->m_list, ROUND_NUM(_roundNum));/*need printing*/
    }
    trickStartMark = FindTwoClubs(_round->m_list);
    for(trickNum = 0; trickNum < HAND_SIZE; ++trickNum)
    {
        CleanTable(_round);        
        for(player = 0; player < NUM_OF_PLAYERS; ++player)
        {
            if((check = PlayTrick(_round, player, trickStartMark)) != ROUND_OK)
            {
                return check;
            }
        }
        /*TODO: think if this trickNum is necessary*/
        _round->m_info->m_trickNum = trickNum;
        trickStartMark = CalculateScore(_round,_round->m_score, trickStartMark);
        PrintScore(_round->m_list, _round->m_score, NUM_OF_PLAYERS);
    }
    FinishRound(_round);
    return ROUND_OK;
}

static void FinishRound(Round *_round)
{
    register size_t player;
    for(player = 0; player < NUM_OF_PLAYERS; ++player)
    {
        /*TODO: maybe we don't need to destroy the hand each time*/
        DestroyHand(_round->m_list[player]);
    }
    /*TODO: maybe instead of destroying and allocating each round we can just zero it down*/
    DestroyRoundInfo(_round);
    PrintScore(_round->m_list, _round->m_score, NUM_OF_PLAYERS);    
}

int WinCondition(int _scoreBoard[])
{
    /*TODO: break into functions*/
    int i, check = -1, winner;
    int min = END_GAME;
    if(_scoreBoard == NULL)
    {
        return 0;
    }
    for (i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        if(_scoreBoard[i] >= END_GAME)
        {
            check = 0;
        }
        if(_scoreBoard[i] < min)
        {
            min = _scoreBoard[i];
            winner = i;
        }
    }
    if(check != -1)
    {
        return winner;
    }
    return check;
}

int SwapRules(int _card, Vector * _hand, int * _table, void * _context)
{
    return TRUE;
}

int TrickRules(int _card, Vector  * _hand, int * _table, void * _context) /*change hand to vector*/
{
    /*TODO: break into functions*/
    /*TODO: fix this fing thing*/
    int suitMark = 0, twoCMark = 0, tempCard;
    size_t suit[4] = {0};/*check if all hearts*/
    register size_t i;
    size_t handSize; 
    RoundInfo * info = (RoundInfo*)_context;
    VectorItemsNum(_hand, &handSize);
    for(i = 0; i < handSize; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(info->m_currSuit == SUIT(tempCard))
        {
            suitMark = 1;
        }
        if(SUIT(tempCard) == CLUB && RANK(tempCard) == TWO)
        {
            twoCMark = 1;
        }
        ++suit[SUIT(tempCard)];
    }
    if(twoCMark == 1 && !(SUIT(_card) == CLUB && RANK(_card) == TWO))
    {
        return FALSE;
    }
    if(suitMark == 1 && SUIT(_card) != info->m_currSuit)
    {
        return FALSE;
    }
    if(info->m_numCardsOnTable == 0 && info->m_brokenHeart == FALSE && SUIT(_card) == HEART)/*change*/
    {
        if(suit[HEART] == handSize && SUIT(_card) == HEART)
        {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

int SwapStrategy(Vector * _hand, int * _table, void * _info)
{
    int maxCard = 0, tempCard;
    register size_t i;
    size_t handSize;
    VectorItemsNum(_hand, &handSize);
    for(i = 0; i < handSize; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(QUEEN_S(tempCard))
        {
            maxCard = tempCard;
            break;
        }
        if(RANK(tempCard) > RANK(maxCard) || (RANK(tempCard) >= RANK(maxCard) && SUIT(tempCard == HEART)))
        {
            maxCard = tempCard;
        }
    }
    return maxCard;
}



int TrickStrategy(Vector * _hand, int * _table, void * _context)
{
    size_t handSize;
    int trickState, card;
    RoundInfo * info = (RoundInfo*)_context;
    VectorItemsNum(_hand, &handSize);
    if(handSize == 1)
    {

        VectorGet(_hand, 0, &card);
        return card;
    }
    if(info->m_numCardsOnTable == 0)
    {
        card = StarterStrategy(_hand, _table,info);
        return card;
    }
    trickState = DefineTrickState(_hand,_table,info);
    switch (trickState)
    {
        case SAFE:
            card = FindBestCard(_hand, info, _table);
            break;
        case NOT_SAFE:
            if((card = FindSafestCard(_hand, info, _table)) == -1)
            {
                
                card = FindBestCard(_hand, info, _table);

            }
            break;

        case NO_SUIT:
            card = FindStinkiestCard(_hand);
            break;

        default:
            break;
    }
    return card;
}


/*----------------------------------------------------------------------------------------*/

static RoundErrors CheckCRParameters(Player ** _playerlist, int _scoreBoard[], size_t _numOfPlayers)
{
    if(_playerlist == NULL || _scoreBoard == NULL)
    {
        return ROUND_POINTER_NULL;
    }
    if(_numOfPlayers != NUM_OF_PLAYERS)
    {
        return ROUND_INVALID_PLAYERS;
    }
    return ROUND_OK;
}

static RoundErrors CheckRunRoundParameters(Round * _round, size_t _roundNum)
{
    if(_round == NULL)
    {
        return ROUND_POINTER_NULL;
    }
    if(_round->m_info != NULL)
    {
        return ROUND_UNITIALIZED_ERROR;
    }
    return ROUND_OK;
}

static void CleanTable(Round * _round)
{
    register size_t j;
    for(j = 0; j < NUM_OF_PLAYERS; ++j)
    {
        _round->m_table[j] = DUMMY_CARD;
    }
    _round->m_info->m_numCardsOnTable = 0;
    _round->m_info->m_currSuit = DUMMY_SUIT;
}

static RoundErrors PlayTrick(Round * _round, size_t _playerNum, int _trickStartMark)
{
    int card;
    card = GiveCard(_round->m_list[PLAYER(_playerNum, _trickStartMark)], _round->m_table, TrickRules, _round->m_info, TrickStrategy);/*need printing*/
    /*TODO: make a define for false card*/
    if(card == -1)
    {
        return ROUND_DEAL_ERROR;
    }
   _round->m_table[PLAYER(_playerNum, _trickStartMark)] = card;
   UpdateInfo(card, _round->m_info);
   if(_playerNum == 0)
   {
        _round->m_info->m_currSuit = SUIT(card);
   }
   ++_round->m_info->m_numCardsOnTable;
   PrintTable(_round->m_table, 4);
   return ROUND_OK;
}

static RoundErrors DealHand(Player ** _players)
{
    Deck * deck;
    register size_t i;
    RoundErrors check;
    if((deck = CreateDeck()) == NULL)
    {
        return ROUND_UNITIALIZED_ERROR;
    }
    /*TODO: break into function*/
    ShuffleDeck(deck);
    if ((check = CreateAllHands(_players)) != ROUND_OK) {
        return check;
    }
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        if ((check = DealCards(_players[i], deck)) != ROUND_OK) {
            return check;
        }
        SortHand(_players[i]); /*now we need to print _hand!*/
    }
    DestroyDeck(&deck);
    return ROUND_OK;
}


static RoundErrors SwapCards(Player ** _players, size_t _roundNum)
{
    /*TODO: break into functions*/
    int temp[NUM_OF_PLAYERS*3] = {0};
    register size_t i, j;
    RoundErrors check;
    int tempCard;
    if((check = CheckSCParameters(_players, _roundNum)) != ROUND_OK)/*redundant!*/
    {
        return check;
    }
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        for(j = 0; j < 3; ++j)
        {
            tempCard = GiveCard(_players[i], NULL, SwapRules, NULL, SwapStrategy);/*need printing*/
            temp[i*3 + j] = tempCard;
        }
    }
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        for(j = 0; j < 3; ++j)
        {
            GetCard(_players[MOVE(i,_roundNum)], temp[i*3 + j]);
        }
    }
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        SortHand(_players[i]);/*now wee need print hand*/
    }
    return ROUND_OK;
}

static RoundErrors CreateAllHands(Player ** _players)
{
    size_t i;
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        if(CreateHand(_players[i], HAND_SIZE) != PLAYER_SUCCESS)
        {
            return ROUND_DEAL_ERROR;
        }
    }
    return ROUND_OK;    
}

static RoundErrors DealCards(Player * _player, Deck * _deck)
{
    size_t i;
    int card;
    for(i = 0; i < HAND_SIZE; ++i)
    {
        card = DrawCard(_deck);
        if(card == -1)
        {
            return ROUND_DEAL_ERROR;
        }
        if(GetCard(_player, card) != PLAYER_SUCCESS)
        {
            return ROUND_DEAL_ERROR;
        }
    }
    return ROUND_OK;    
}

static int DefineTrickState(Vector * _hand, int * _table, RoundInfo * _info)
{
    size_t heartsInTable = 0, handSize;
    char isQueenSInsHand = 0, isQSInTable = 0;
    int suits[4] = {0};
    VectorItemsNum(_hand, &handSize);
    CheckHand(_hand, handSize, suits, &isQueenSInsHand);
    if(IsNoSuit(suits) == TRUE)
    {
        return NO_SUIT;
    }
    if(_info->m_currSuit == HEART)
    {
        return NOT_SAFE;
    }
    CheckTable(_table, &heartsInTable, &isQSInTable);
    if(isQSInTable == 1)
    {
        return NOT_SAFE;
    }
    if(_info->m_numCardsOnTable == 3 && heartsInTable <= 1)
    {
        return SAFE;
    }
    if(CARD_CHANCE(NUM_OF_RANK - _info->m_suitsPlayed[_info->m_currSuit] - handSize, NUM_OF_RANK) < 25 ||
     heartsInTable >= 1)
    {
        return NOT_SAFE;
    }
    if(_info->m_queenOfSpades == FALSE && isQueenSInsHand == 0 &&
    CARD_CHANCE(NUM_OF_RANK - _info->m_suitsPlayed[_info->m_currSuit] - handSize, NUM_OF_RANK) < 25)
    {
        return NOT_SAFE;
    }
    return SAFE;
}

static int StarterStrategy(Vector * _hand, int * _table,RoundInfo * _info)
{
    register size_t i;
    size_t handSize;
    char isQueenSInsHand = 0;
    int tempCard, card = DUMMY_CARD;
    int suits[4] = {0};
    VectorItemsNum(_hand, &handSize);
    CheckHand(_hand, handSize, suits, &isQueenSInsHand);
    if(_info->m_queenOfSpades == FALSE && suits[SPADE] > 0 && isQueenSInsHand == 0)
    {
        for(i = 0; i < handSize; ++i)
        {
            VectorGet(_hand, i, &tempCard);
            if(SUIT(tempCard) != SPADE)
            {
                continue;
            }
            if(RANK(tempCard) >= KING)
            {
                continue;
            }
            if(RANK(tempCard) > RANK(card))
            {
                card = tempCard;
            }
        }
    }
    for(i = 0; i < NUM_OF_SUIT; ++i)
    {
        if(suits[i] > 0)
        {
            if((card = IsSafish(_hand, _info, i)) != -1)
            {
                return card;
            }
        }
    }
    card = FindBestCard(_hand, _info, _table);
    return card;
}

static int FindBestCard(Vector * _hand, RoundInfo * _info, int * _table)
{
    register size_t i;
    int maxCard, tempCard;
    size_t handSize;
    char isQueenInHand = 0;
    int suits[NUM_OF_SUIT];
    VectorGet(_hand, 0, &maxCard);
    VectorItemsNum(_hand, &handSize);
    CheckHand(_hand, handSize, suits, &isQueenInHand);
    for(i = 1; i < handSize; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(QUEEN_S(tempCard) && IsKingsAndAces(_table) == FALSE)
        {
            continue;
        }
        if((RANK(tempCard >= KING) && SUIT(tempCard) == SPADE) &&
          _info->m_queenOfSpades == FALSE && isQueenInHand == 0)
        {
            continue;
        }
        if(RANK(tempCard) > RANK(maxCard))
        {
            maxCard = tempCard;
        }
    }
    return maxCard;
}

static int IsKingsAndAces(int * _table)
{
    register size_t i;
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        if(SUIT(_table[i]) == SPADE && RANK(_table[i] >= KING))
        {
            return TRUE;
        }
    }
    return FALSE;
}

static int FindSafestCard(Vector * _hand, RoundInfo * _info, int * _table)
{
    register size_t i;
    size_t handSize;
    int safest = -1, tempCard, highestInTable;
    highestInTable = FindHighestCardInTable(_table);
    VectorItemsNum(_hand, &handSize);
    for(i = 0; i < handSize; ++i)
    {
        VectorGet(_hand, handSize - 1 - i, &tempCard);
        if(QUEEN_S(tempCard))
        {
            continue;
        }
        if(RANK(tempCard) < RANK(highestInTable))
        {
            safest = tempCard;
        }
    }
    if(safest == -1 && _info->m_numCardsOnTable < 3)
    {
        safest = IsSafish(_hand, _info, _info->m_currSuit);
    }
    return safest;

}

static int IsSafish(Vector * _hand, RoundInfo * _info , Suit _suit)
{
    register size_t i;
    size_t handSize;
    int safish = -1, tempCard, potentialHigher, potentialLower;
    VectorItemsNum(_hand, &handSize);    
    for(i = 0; i < handSize; ++i)
    {
        VectorGet(_hand, handSize - 1 - i, &tempCard);
        if(SUIT(tempCard) != _suit)
        {
            continue;
        }
        if(QUEEN_S(tempCard))
        {
            continue;
        }
        if((potentialHigher = PotentialCards(_hand, _info, CARD_INDEX(tempCard) + 1, ACE*_suit)) == 0)
        {
            continue;
        }
        potentialLower = PotentialCards(_hand, _info, TWO*_suit, CARD_INDEX(tempCard));
        if(potentialHigher - potentialLower <= 0)
        {
            continue;
        }
            safish = tempCard;
    }
    return safish;    
}

static int PotentialCards(Vector * _hand, RoundInfo * _info, int _index1, int _index2)
{
    register size_t i;
    int howMany = 0;
    for(i = _index1; i < _index2; ++i)
    {
        if(_info->m_whichCardsPlayed[i] == 0 && IsInHand(_hand, _info->m_whichCardsPlayed[i]) == FALSE)
        {
            howMany++;
        }
    }
    return howMany;
}

static int IsInHand(Vector * _hand, int _card)
{
    register size_t i;
    size_t handSize;
    int tempCard;
    VectorItemsNum(_hand, &handSize);
    for(i = 0; i < handSize; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(RANK(tempCard) == RANK(_card) && SUIT(tempCard) == SUIT(_card))
        {
            return TRUE;
        }
    }
    return FALSE;
}

static void CheckHand(Vector* _hand, size_t _handSize,int * suits, char * isQueenSInsHand)
{
    register size_t i;
    int tempCard;
    for(i = 0; i < _handSize; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(QUEEN_S(tempCard))
        {
            *isQueenSInsHand = 1;
        }
        ++suits[SUIT(tempCard)];
    }
    return;
}

static int IsNoSuit(int * suits)
{
    register size_t i;
    char noSuit = 0;
    for(i = 0; i < NUM_OF_SUIT; ++i)
    {
        if(suits[i] > 0)
        {
            ++noSuit;
        }
    }
    if(noSuit > 1)
    {
        return TRUE;
    }
    return FALSE;
}

static void CheckTable(int * _table, size_t * _heartsInTable, char * _isQSInTable)
{
    register size_t i;
    for(i = 0; i < NUM_OF_PLAYERS; i++)
    {
        if(_table[i] == DUMMY_CARD)
        {
            continue;
        }
        if(SUIT(_table[i]) == HEART)
        {
            *_heartsInTable += 1;
        }
        if(QUEEN_S(_table[i]))
        {
            *_isQSInTable = 1;
        }
    }
}

/*
static int FindHighestCard(Vector * _hand, RoundInfo  * _info, int * _table)
{
    register size_t i, j;
    int tempCard = DUMMY_CARD, maxCard = DUMMY_CARD;
    size_t length;
    VectorItemsNum(_hand, &length);
    if(_info->m_numCardsOnTable == 0)
    {
        if(_info->m_queenOfSpades == FALSE)
        for(i = 0; i < length; ++i)
        {
            VectorGet(_hand, i, &tempCard);
            if(SUIT(tempCard) == SPADE && RANK(tempCard) > RANK(DUMMY_CARD) &&
             (RANK(tempCard != QUEEN) || RANK(tempCard != KING)))
            {
                maxCard = tempCard;
            }
        }
        if(maxCard != DUMMY_CARD)
        {
            return maxCard;
        }
    }
    for(i = 0; i < length; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(QUEEN_S(tempCard))
        {
            for(j = 0; j < NUM_OF_PLAYERS; ++j)
            {
                if(RANK(_table[j]) == KING && SUIT(_table[j] == SPADE))
                return tempCard;
            }
            continue;
        }
        if((RANK(tempCard) == KING && SUIT(tempCard == SPADE)) && _info->m_queenOfSpades == FALSE)
        {
            continue;
        }
        if(RANK(tempCard) > RANK(maxCard))
        {
            maxCard = tempCard;
        }
    }
    return maxCard;
}
*/
/*
static int FindLowestCard(Vector * _hand)
{
    register size_t i;
    int minCa_sFunc(tempVec, _table,_context);rd, tempCard;
    size_t length;
    VectorGet(_hand, 0, &minCard);
    VectorItemsNum(_hand, &length);
    for(i = 1; i < length; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(RANK(tempCard) < RANK(minCard))
        {
            minCard = tempCard;
        }
    }
    return minCard;
}
*/
static int FindHighestCardInTable(int *_table)
{
    register size_t i;
    int maxCard = 0;
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        if(RANK(_table[i]) > RANK(maxCard))
        {
            maxCard = _table[i];
        }
    }
    return maxCard;
}

static int FindStinkiestCard(Vector * _hand)
{
    register size_t i;
    int stinkiestCard, tempCard;
    size_t length;
    VectorGet(_hand, 0, &stinkiestCard);
    VectorItemsNum(_hand, &length);
    for(i = 1; i < length; ++i)
    {
        VectorGet(_hand, i, &tempCard);
        if(QUEEN_S(tempCard))
        {
            return tempCard;
        }
        if(RANK(tempCard) > RANK(stinkiestCard))
        {
            if(SUIT(tempCard) != HEART && SUIT(stinkiestCard) == HEART)
            {
                continue;
            }
            stinkiestCard = tempCard;
        }
    }
    return stinkiestCard;    
}

static RoundErrors CheckSCParameters(Player ** _players, size_t _roundNum)
{
    if (!_players)
    {
        return ROUND_POINTER_NULL;
    }
    if(_roundNum > 3)
    {
        return ROUND_DEAL_ERROR;
    }
    return ROUND_OK;
}

static int FindTwoClubs(Player ** _players)
{
    int tempCard;
    register size_t i, j;
    /*TODO: the hands are sorted so there's no need to go over all the player's hand!*/
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        for(j = 0; j < HAND_SIZE; ++j)
        {
            tempCard = ShowCard(_players[i], j);
            if(RANK(tempCard) == TWO && SUIT(tempCard) == CLUB)
            {
        
                return i;
            }
        }
    }
    return FALSE;
}

static void UpdateInfo(int _card, RoundInfo * _info)
{
    ++_info->m_whichCardsPlayed[CARD_INDEX(_card)];
    ++_info->m_nCardsPlayed;
    _info->m_suitsPlayed[SUIT(_card)] += 1;
    if(_info->m_suitsPlayed[HEART] > 0)
    {
        _info->m_brokenHeart = TRUE;
    }
    if(QUEEN_S(_card))
    {
        _info->m_queenOfSpades = TRUE;
    }
}

static int CalculateScore(Round * _round, int * _tempScore, int _startMark)
{
    register size_t i;
    int nextStarter = _startMark, maxCard, score = 0;
    int suit = _round->m_info->m_currSuit;
    maxCard = -1;
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        if(SUIT(_round->m_table[i]) == HEART)
        {
            ++score;
        }
        if(QUEEN_S(_round->m_table[i]))
        {
            score += 13;
        }
        if(SUIT(_round->m_table[i]) != suit)
        {
            continue;
        }
        if(RANK(_round->m_table[i]) > RANK(maxCard))
        {
            nextStarter = i;
            maxCard = _round->m_table[i];
        }
    }
    _tempScore[nextStarter] += score;
    return nextStarter;
}
/*
static void CheckForMoon(int * _tempScore)
{
    register size_t i;
    int moonToken = -1;
    for(i = 0; i < NUM_OF_PLAYERS; ++i)
    {
        if(_tempScore[i] == 26)
        {
            moonToken = i;
            break;
        }
    }
    if(moonToken != -1)
    {
        for(i = 0; i < NUM_OF_PLAYERS; ++i)
        {
            if(i == moonToken)
            {
                continue;
            }
            _tempScore[i] = 26;
        }
    }
}*/

static RoundErrors CheckRoundCorrectnes(Round * _round, size_t _roundNum)
{
    RoundErrors check;
    if((check = CheckRunRoundParameters(_round, _roundNum)) != ROUND_OK)
    {
        return check;
    }
    if((_round->m_info = CreateRoundInfo()) == NULL)
    {
        return ROUND_UNITIALIZED_ERROR;
    }
    return ROUND_OK;    
}

static RoundInfo* CreateRoundInfo()
{
    /*TODO: Break into functions*/
    RoundInfo * info;
    if((info = (RoundInfo*)malloc(sizeof(RoundInfo))) == NULL)
    {
        return NULL;
    }
    if ((info->m_whichCardsPlayed = (int*)calloc(NUM_OF_PLAYERS*HAND_SIZE, sizeof(int))) == NULL)
    {
        free(info);
        return NULL;
    }
    if((info->m_suitsPlayed = (float*)calloc(NUM_OF_SUIT, sizeof(float))) == NULL)
    {
        free(info->m_whichCardsPlayed);
        free(info);
        return NULL;
    }
    info->m_brokenHeart = FALSE;
    info->m_queenOfSpades = FALSE;
    info->m_numCardsOnTable = 0;
    info->m_nCardsPlayed = 0;
    info->m_trickNum = 0;
    info->m_currSuit = DUMMY_SUIT;
    return info;
}

static void DestroyRoundInfo(Round * _round)
{
    if(_round == NULL || _round->m_info == NULL)
    {
        return;
    }
    free(_round->m_info->m_suitsPlayed);
    free(_round->m_info->m_whichCardsPlayed);
    free(_round->m_info);
    _round->m_info = NULL;
    return;
}
/*
static void UpdateScore(Round * _round, int *tempScore)
{
    register size_t i;
    for (i = 0; i < NUM_OF_PLAYERS; i++)
    {
        _round->m_score[i] = tempScore[i];
    }
    
}*/