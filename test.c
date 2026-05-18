/* 
 File:        test.c 
 Autori:      Assuntapia Piccolo,Fabiana Sasso, Lucia Schettino
 Matricole:   NF12100574, NF12100245, NF12100025 
 Descrizione: Programma di test automatizzato per il sistema di gestione
              degli interventi di manutenzione in un condominio.
              Per ogni TC legge TCn_oracle.txt, esegue il test in memoria,
              scrive TCn_output.txt e confronta con l'oracolo (PASS/FAIL).
              I risultati dell'intera test suite vengono scritti in result.txt.

Compilazione:
    gcc -c src/utile.c src/richieste.c src/tecnici.c src/schedule.c
   gcc utile.o richieste.o tecnici.o schedule.o test_condominio.c -o test_condominio
 
 Esecuzione singolo TC:
    ./test_condominio TC1 TC1_oracle.txt TC1_output.txt

  Esecuzione intera suite:
   ./test_condominio --suite test_suite.txt result.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/richieste.h"
#include "headers/tecnici.h"
#include "headers/schedule.h"
#include "headers/utile.h"

/* DA INSERIRE:
-funziooni di supporto
-funzioni di test per ogni TC (eseguiTC1, eseguiTC2, ...)
-runner eseguiTC che chiama la funzione giusta in base al nome del TC
-main che gestisce le due modalita' (singolo TC o intera suite)
*/
