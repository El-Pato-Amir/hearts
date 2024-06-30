#include "game.h"
#include "round.h"

/*TODO: fix the sorting of cards in hand so it shows cards in groups by suite*/
/*TODO: run more tests for this thing*/
/*TODO: make the UI better*/


char * gNames[] = {"Amir", "Noam", "Dekel", "Arad"};
size_t gId[] = {1234, 2345, 3456, 4567};

int main(void)
{
    Game * hearts;
    hearts = CreateGame(4, 1, gNames, gId, WinCondition);
    RunGame(hearts);
    DestroyGame(&hearts);
    return 0;
}