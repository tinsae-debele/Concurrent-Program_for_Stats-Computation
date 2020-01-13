myapp: STATS.o
	gcc -o myapp STATS.o
STATS.o: STATS.c format.h semun.h 
	gcc -c STATS.c 
