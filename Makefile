programma: main.o schedule.o richieste.o tecnici.o
	gcc -Wall -g -std=c99 -o programma main.o schedule.o richieste.o tecnici.o

main.o: main.c headers/schedule.h headers/richieste.h headers/tecnici.h headers/tipi.h
	gcc -Wall -g -std=c99 -c main.c -o main.o

schedule.o: funzioni/schedule-lucia.c headers/schedule.h
	gcc -Wall -g -std=c99 -c funzioni/schedule-lucia.c -o schedule.o

richieste.o: funzioni/richieste-assia.c headers/richieste.h headers/tipi.h
	gcc -Wall -g -std=c99 -c funzioni/richieste-assia.c -o richieste.o

tecnici.o: funzioni/tecnici-fabiana.c headers/tecnici.h headers/tipi.h
	gcc -Wall -g -std=c99 -c funzioni/tecnici-fabiana.c -o tecnici.o

clean:
	rm -f *.o programma