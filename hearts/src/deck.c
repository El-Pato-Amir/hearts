#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include "vector.h"
#include "ADTDefs.h"
#define NUMBER_OF_CARDS 52
#define CARD_RANK(i) i % NUM_OF_RANK
#define CARD_SUIT(i) i / NUM_OF_RANK



struct Deck
{
    Vector * m_deck;
};

static void Swap(Deck * _deck, size_t _index1, size_t _index2);

Deck * CreateDeck()/*number of decks!*/
{
    Vector * temp;
    Deck * deck;
    register int i;
    if((deck = (Deck*)malloc(sizeof(Deck))) == NULL)
    {
        return NULL;
    }
    if((temp = VectorCreate(NUMBER_OF_CARDS, 0)) == NULL)
    {
        free(deck);
        return NULL;
    }
    /*TODO: this should be in a different function*/
    deck->m_deck = temp;
    for(i = 0; i < NUMBER_OF_CARDS; ++i)
    {
        VectorAdd(deck->m_deck, i);
    }
    return deck;

}

void DestroyDeck(Deck **_deck)
{
    if (_deck == NULL || *_deck == NULL)
    {
        return;
    }
    VectorDestroy((*_deck)->m_deck);
    free(*_deck);
    *_deck = NULL;
}

void ShuffleDeck(Deck * _deck)
{
    register size_t i;
    size_t index1, index2;
    size_t numOfCards;
    if (_deck == NULL)
    {
        return;
    }
    VectorItemsNum(_deck->m_deck, &numOfCards);
    if(numOfCards > 1)
    {
        /*TODO: this should be a different function*/
        srand(time(NULL));
        for (i = 0; i < NUMBER_OF_CARDS*2; ++i)
        {
            index1 = rand() % numOfCards;
            index2 = rand() % numOfCards;
            Swap(_deck,index1 ,index2 );
        }
    }
}

int DrawCard(Deck * _deck)/*maybe do a "Deal" function that loops DrawCard?*/
{
    int card;
    if(_deck == NULL)
    {
        return -1;
    }
    if(VectorDelete(_deck->m_deck, &card) == ERR_OK)
    {
        return card;
    }
    return -1;
}

static void Swap(Deck * _deck, size_t _index1, size_t _index2)
{
    int item1, item2;
    VectorGet(_deck->m_deck, _index1, &item1);
    VectorGet(_deck->m_deck, _index2, &item2);
    VectorSet(_deck->m_deck, _index2, item1);
    VectorSet(_deck->m_deck, _index1, item2);
}
