#ifndef __DECK_H__
#define __DECK_H__
#include <stddef.h>
typedef struct Deck Deck;

/*
description - creates a standart card deck
input - none
output - a deck pointer on success, NULL on fail
error - NULL if fails to allocate deck
*/
Deck * CreateDeck();

/*
description - destroys a deck
input - a deck pointer address
output - none
error - none
*/
void DestroyDeck(Deck **_deck);

/*
description - shuffles a deck
input - a deck pointer
output - none
error - none
*/
void ShuffleDeck(Deck * _deck);

/*
description - draws a card from a deck
input - a deck pointer
output - a card in int format on success, -1 if not
error - -1 if deck pointer is NULL 
*/
int DrawCard(Deck * _deck);


#endif /*__DECK_H__*/