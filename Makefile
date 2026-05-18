gestione_condominio: gestione_condominio.o schedule.o richieste.o tecnici.o utile.o
	gcc -Wall -g -std=c99 -o programma gestione_condominio.o schedule.o richieste.o tecnici.o utile.o

utile.o: funzioni/utile.c headers/utile.h
	gcc -Wall -g -std=c99 -c funzioni/utile.c -o utile.o

schedule.o: funzioni/schedule-lucia.c headers/schedule.h
	gcc -Wall -g -std=c99 -c funzioni/schedule-lucia.c -o schedule.o

richieste.o: funzioni/richieste-assia.c headers/richieste.h headers/tipi.h
	gcc -Wall -g -std=c99 -c funzioni/richieste-assia.c -o richieste.o

tecnici.o: funzioni/tecnici-fabiana.c headers/tecnici.h headers/tipi.h
	gcc -Wall -g -std=c99 -c funzioni/tecnici-fabiana.c -o tecnici.o

utile.o: funzioni/utile.c headers/utile.h
	gcc -Wall -g -std=c99 -c funzioni/utile.c -o utile.o

clean:
	rm -f *.o gestione_condominio