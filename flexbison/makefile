calc: lex.yy.c calc.tab.c
	gcc -g lex.yy.c calc.tab.c -o calc

lex.yy.c: calc.tab.c calc.l
	lex calc.l

calc.tab.c: calc.y
	bison -d calc.y

clean: 
	rm -rf lex.yy.c calc.tab.c y.tab.h calc calc.dSYM

