grep : grep0.o KMP.o option.o pipe.o
	gcc -o grep grep0.o KMP.o option.o pipe.o -lncurses
grep0.o : grep0.c
	gcc -c -o grep0.o grep0.c -lncurses
KMP.o : KMP.c
	gcc -c -o KMP.o KMP.c
option.o : option.c
	gcc -c -o option.o option.c -lncurses
pipe.o : pipe.c
	gcc -c -o pipe.o pipe.c -lncurses
clean :
	rm *.o grep
