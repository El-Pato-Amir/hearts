#makefile hearts

INCLUDE_PATH = inc
SOURCE_PATH = src

TARGET= hearts.out
CC= gcc
VALGRIND=valgrind
VTARGET= memchek.log
CFLAGS= -ansi -pedantic -g -Werror -Wall -I $(INCLUDE_PATH) -I $(SOURCE_PATH)

OBJECTS= deck.o game.o player.o vector.o round.o heap.o ui.o hearts_main.o
TESTOBJECT= deck.o game.o player.o vector.o round.o heap.o ui.o tests.o
VFLAGS=--log-file=$(VTARGET) --leak-check=full --track-origins=yes -s  

hearts: $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)
	
deck.o: $(SOURCE_PATH)/deck.c $(INCLUDE_PATH)/deck.h $(INCLUDE_PATH)/vector.h $(INCLUDE_PATH)/ADTDefs.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)/deck.c

game.o: $(SOURCE_PATH)/game.c $(INCLUDE_PATH)/game.h $(INCLUDE_PATH)/player.h $(INCLUDE_PATH)/ui.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)/game.c

player.o: $(SOURCE_PATH)/player.c $(INCLUDE_PATH)/player.h $(INCLUDE_PATH)/vector.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)/player.c

vector.o: $(SOURCE_PATH)/vector.c $(INCLUDE_PATH)/vector.h $(INCLUDE_PATH)/ADTDefs.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)/vector.c

heap.o: $(SOURCE_PATH)/heap.c $(INCLUDE_PATH)/heap.h $(INCLUDE_PATH)/vector.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)/heap.c

round.o: $(SOURCE_PATH)/round.c $(INCLUDE_PATH)/round.h $(INCLUDE_PATH)/vector.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)/round.c

ui.o: $(SOURCE_PATH)/ui.c $(INCLUDE_PATH)/ui.h $(INCLUDE_PATH)/vector.h $(INCLUDE_PATH)/player.h $(INCLUDE_PATH)/round.h $(INCLUDE_PATH)/card.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)/ui.c

hearts_main.o: hearts_main.c $(INCLUDE_PATH)/game.h $(INCLUDE_PATH)/round.h
	$(CC) $(CFLAGS) -c hearts_main.c

valgrind:
	$(VALGRIND) $(VFLAGS) ./$(TARGET) 

clean:
	rm -f $(TARGET) $(OBJECTS) $(TESTOBJECT) utest.out
