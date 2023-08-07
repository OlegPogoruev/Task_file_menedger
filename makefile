#цель: зависимость
#[tab] команда


BIN = FileMenedger
SOURCES = main.c
HEADERS = main.h
CC := gcc

all:
	$(CC) $(SOURCES) -lncurses -o $(BIN).out
#	$(CC) $(SOURCES) $(pkg-config ncursesw --libs --cflags) -lncurses -o $(BIN).out
#	gcc `pkg-config --cflags ncursesw` main.c -o FileMenedger.out `pkg-config --libs ncursesw`
#	gcc `pkg-config --cflags ncursesw` main.c -o main `pkg-config --libs ncursesw`
#	$(CC) $(SOURCES) -lncursesw -o $(BIN).out
$(BIN): $(HEADERS)

run: all
	./$(BIN).out

#clean:
#	rm -rf $(BIN) *.o $(BIN).out



















