programma: gestione_condominio.o schedule.o richieste.o tecnici.o utile.o
	gcc -Wall -g -std=c99 -o programma gestione_condominio.o schedule.o richieste.o tecnici.o utile.o

gestione_condominio.o: gestione_condominio.c headers/schedule.h headers/richieste.h headers/tecnici.h headers/tipi.h headers/utile.h
	gcc -Wall -g -std=c99 -c gestione_condominio.c -o gestione_condominio.o

schedule.o: funzioni/schedule.c headers/schedule.h
	gcc -Wall -g -std=c99 -c funzioni/schedule.c -o schedule.o

richieste.o: funzioni/richieste.c headers/richieste.h headers/tipi.h
	gcc -Wall -g -std=c99 -c funzioni/richieste.c -o richieste.o

tecnici.o: funzioni/tecnici.c headers/tecnici.h headers/tipi.h
	gcc -Wall -g -std=c99 -c funzioni/tecnici.c -o tecnici.o

utile.o: funzioni/utile.c headers/utile.h
	gcc -Wall -g -std=c99 -c funzioni/utile.c -o utile.o

clean:
	rm -f *.o programma