#include <stdio.h>
#include "ui.h"
#define NORMAL "\x1B[0m"
#define BLACK "\x1b[30m"
#define RED "\x1B[31m"
#define TRUE 1
#define FALSE 0
#define HEART_SYMBOL "\xE2\x99\xA5"
#define SPADE_SYMBOL "\xE2\x99\xA0"
#define DIAMOND_SYMBOL "\xE2\x99\xA6"
#define CLUB_SYMBOL "\xE2\x99\xA3"

void PrintCard(int _card)
{
    Suit color = SUIT(_card);
    switch (color)
    {
        case HEART:
        
            switch (RANK(_card))
            {
                case JACK:
                    printf(RED);
                    printf("J%s\t", HEART_SYMBOL);
                    printf(NORMAL);
                    break;

                case QUEEN:
                    printf(RED);
                    printf("Q%s\t", HEART_SYMBOL);
                    printf(NORMAL);
                    break;

                case KING:
                    printf(RED);
                    printf("K%s\t", HEART_SYMBOL);
                    printf(NORMAL);                            
                    break;

                case ACE:
                    printf(RED);
                    printf("A%s\t", HEART_SYMBOL);
                    printf(NORMAL);
                    break;                    
                                                         
                default:
                    printf(RED);
                    printf("%d%s\t",RANK(_card)+2, HEART_SYMBOL);
                    printf(NORMAL);
                    break;
            }

            break;
        case SPADE:
        
            switch (RANK(_card))
            {
                case JACK:
                    printf(BLACK);
                    printf("J%s\t", SPADE_SYMBOL);
                    printf(NORMAL);
                    break;
    
                case QUEEN:
                    printf(BLACK);
                    printf("Q%s\t", SPADE_SYMBOL);
                    printf(NORMAL);
                    break;
    
                case KING:
                    printf(BLACK);
                    printf("K%s\t", SPADE_SYMBOL);
                    printf(NORMAL);                            
                    break;
    
                case ACE:
                    printf(BLACK);
                    printf("A%s\t", SPADE_SYMBOL);
                    printf(NORMAL);
                    break;
                                    
                default:
                    printf(BLACK);
                    printf("%d%s\t",RANK(_card) +2, SPADE_SYMBOL);
                    printf(NORMAL);            
                    break;
            }
            break;        
    
        case DIAMOND:
        
            switch (RANK(_card))
            {
                case JACK:
                    printf(RED);
                    printf("J%s\t", DIAMOND_SYMBOL);
                    printf(NORMAL);
                    break;

                case QUEEN:
                    printf(RED);
                    printf("Q%s\t", DIAMOND_SYMBOL);
                    printf(NORMAL);
                    break;

                case KING:
                    printf(RED);
                    printf("K%s\t", DIAMOND_SYMBOL);
                    printf(NORMAL);                            
                    break;

                case ACE:
                    printf(RED);
                    printf("A%s\t", DIAMOND_SYMBOL);
                    printf(NORMAL);
                    break;

                default:
                    printf(RED);
                    printf("%d%s\t",RANK(_card) +2, DIAMOND_SYMBOL);
                    printf(NORMAL);            
                    break;
            }
            break;        
        
        case CLUB:
            switch (RANK(_card))
            {
                case JACK:
                    printf(BLACK);
                    printf("J%s\t", CLUB_SYMBOL);
                    printf(NORMAL);
                    break;

                case QUEEN:
                    printf(BLACK);
                    printf("Q%s\t", CLUB_SYMBOL);
                    printf(NORMAL);
                    break;

                case KING:
                    printf(BLACK);
                    printf("K%s\t", CLUB_SYMBOL);
                    printf(NORMAL);                            
                    break;

                case ACE:
                    printf(BLACK);
                    printf("A%s\t", CLUB_SYMBOL);
                    printf(NORMAL);
                    break;

                default:
                    printf(BLACK);
                    printf("%d%s\t",RANK(_card) +2, CLUB_SYMBOL);
                    printf(NORMAL);            
                    break;
            }
            break;

            default:
                break;
    }            

}

void PrintTable(int * _table, int _tableSize)
{
    register size_t i;
    if(_table == NULL)
    {
        return;
    }
    printf("\nCards on table:\n");
    for(i = 0; i < _tableSize; ++i)
    {
        if(_table[i] != DUMMY_CARD)
        {
            printf("\nPlayer %lu: ", i);
            PrintCard(_table[i]);
        }
    }
}



void PrintHand(Player * _player)
{
    register size_t i;
    size_t handSize;
    int card;
    if(_player == NULL)
    {
        return;
    }
    putchar('\n');
    handSize = HandSize(_player);
    for(i = 0; i < handSize; ++i)
    {
        card = ShowCard(_player, i);
        PrintCard(card);
    }
    putchar('\n');
    for(i = 0; i < handSize; ++i)
    {
        printf("%lu\t", i);
    }
    putchar('\n');
    return;
}


void PrintScore(Player ** _players, int * _scoreBoard, size_t _numOfPlayers)
{
    const char * name;
    register size_t i;
    if(_scoreBoard == NULL || _players == NULL)
    {
        return;
    }
    for(i = 0; i < _numOfPlayers; ++i)
    {
        name = ShowPlayerName(_players[i]);
        printf("\nPlayer number:%lu\nName:%s\n \nScore: %d\n", i, name, _scoreBoard[i]);
    }
    return;

}

void PlayerPrompt(int _case, Player * _player)
{
    const char * name;
    if(_player == NULL)
    {
        return;
    }
    name = ShowPlayerName(_player);
    if(_case == FALSE)
    {
        printf("\nInvalid card, choose another one.\n");
    }
    printf("\nHello %s, Choose a card Index\n", name);
}

void PrintWinner(Player * _player, int * _scoreBoard)
{
    const char * name;
    if(_player == NULL || _scoreBoard == NULL)
    {
        return;
    }
    name = ShowPlayerName(_player);
    printf("\nAnd The winner is: %s\n", name);  
}




