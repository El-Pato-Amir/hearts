#ifndef __CARD_H__
#define __CARD_H__
#define RANK(n) n % NUM_OF_RANK
#define SUIT(n) n / NUM_OF_RANK 

typedef enum Rank
{
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
    NUM_OF_RANK,
    DUMMY_CARD = -1
}Rank;

typedef enum Suit
{
    HEART,
    SPADE,
    DIAMOND,
    CLUB,
    NUM_OF_SUIT,
    DUMMY_SUIT = -1
}Suit;

#endif /*__CARD_H__*/