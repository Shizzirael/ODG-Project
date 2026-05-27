condominio: gestione_condominio.o schedule.o richieste.o tecnici.o utile.o
	gcc -Wall -g -std=c99 -o condominio gestione_condominio.o schedule.o richieste.o tecnici.o utile.o

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

compile_test: tests/test_main.c tests/test_tecnici.c tests/test_richieste.c \
              tests/test_schedule.c tecnici.o richieste.o schedule.o utile.o
	gcc -I./headers tests/test_main.c tests/test_tecnici.c tests/test_richieste.c \
	    tests/test_schedule.c tecnici.o richieste.o schedule.o utile.o \
	    -o tests/esegui_test

test: compile_test
	cd tests && ./esegui_test --suite test_suite.txt result.txt

clean:
	rm -f *.o src/*.o tests/*.o condominio tests/esegui_test
	rm -f tests/TC*_output.txt