# Compiler und Flags definieren
CC = g++
CFLAGS = -Wall -g
CXXFLAGS = -std=c++20 -Wall -g
LEX = flex
YACC = bison
YFLAGS = -d

# Zieldatei
TARGET = test

# Objektdateien
OBJS = lex.yy.o test.tab.o main.o functions.o

all: $(TARGET) makefile

run: $(TARGET)
	./$(TARGET)

# Regel zum Erstellen des Hauptprogramms
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) -lreadline

# Regel für parse.tab.c und parse.tab.h
test.tab.c test.tab.h: test.y
	$(YACC) $(YFLAGS) test.y

# Regel für lex.yy.c
lex.yy.c: test.l test.tab.h
	$(LEX) --header-file=lex.yy.h test.l

# Regel für lex.yy.o
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

# Regel für parse.tab.o
test.tab.o: test.tab.c
	$(CC) $(CFLAGS) -c test.tab.c

# Regel für main.o
main.o: main.cpp test.tab.h
	$(CC) $(CFLAGS) -c main.cpp

functions.o: functions.cpp
	$(CC) $(CFLAGS) -c functions.cpp
# Regel für clean
clean:
	rm -f $(TARGET) $(OBJS) lex.yy.c lex.yy.h test.tab.c test.tab.h

