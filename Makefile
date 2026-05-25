programma: gestione_condominio.o schedule.o richieste.o tecnici.o utile.o
	gcc -Wall -g -std=c99 -o programma gestione_condominio.o schedule.o richieste.o tecnici.o utile.o

gestione_condominio.o: gestione_condominio.c headers/schedule.h headers/richieste.h headers/tecnici.h headers/tipi.h headers/utile.h
	gcc -Wall -g -std=c99 -c gestione_condominio.c -o gestione_condominio.o

schedule.o: src/schedule.c headers/schedule.h
	gcc -Wall -g -std=c99 -c src/schedule.c -o schedule.o

richieste.o: src/richieste.c headers/richieste.h headers/tipi.h
	gcc -Wall -g -std=c99 -c src/richieste.c -o richieste.o

tecnici.o: src/tecnici.c headers/tecnici.h headers/tipi.h
	gcc -Wall -g -std=c99 -c src/tecnici.c -o tecnici.o

utile.o: src/utile.c headers/utile.h
	gcc -Wall -g -std=c99 -c src/utile.c -o utile.o

test: tests/test_main.c tests/test_richieste.c tests/test_tecnici.c tests/test_schedule.c src/richieste.c src/tecnici.c src/schedule.c src/utile.c
	gcc -Wall -g -std=c99 -o test \
		tests/test_main.c tests/test_richieste.c tests/test_tecnici.c tests/test_schedule.c \
		src/richieste.c src/tecnici.c src/schedule.c src/utile.c

esegui_test: test
	cd tests && ../test

clean:
	rm -f *.o programma test