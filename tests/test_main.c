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
#include "../headers/richieste.h"
#include "../headers/tecnici.h"
#include "../headers/schedule.h"
#include "../headers/utile.h"
#include "../tests/test_condominio.h"

/* =================================================================
   HELPER: confrontaFile
   Confronta due file riga per riga.
   Ritorna 1 se identici, 0 altrimenti.
   ================================================================= */
static int confrontaFile(const char* f1, const char* f2) {
    FILE* a = fopen(f1, "r");
    FILE* b = fopen(f2, "r");
    char r1[512], r2[512];
    int uguale = 1;

    if (!a || !b) { /* se non riesce ad aprire uno dei file, considera diversi */
        if (a) fclose(a);
        if (b) fclose(b);
        return 0;
    }

    while (1) {
        char* l1 = fgets(r1, sizeof(r1), a); /* legge una riga dal file a */
        char* l2 = fgets(r2, sizeof(r2), b); /* legge una riga dal file b */

        if (!l1 && !l2) break; /* entrambi finiti: file identici */

        if (!l1 || !l2) { /* uno finito prima dell'altro: diversi */
            uguale = 0;
            break;
        }

        if (strcmp(r1, r2) != 0) { /* righe diverse */
            uguale = 0;
            break;
        }
    }

    fclose(a);
    fclose(b);
    return uguale;
}

/* =================================================================
   DISPATCHER: eseguiTC
   Apre i file del TC, chiama la funzione corretta,
   confronta output vs oracle e registra l'esito.
   ================================================================= */
static void eseguiTC(const char* tcId, const char* fin,
                     const char* foracle, const char* fout,
                     FILE* result) {
    FILE* input  = fopen(fin,  "r");
    FILE* output = fopen(fout, "w");

    if (output == NULL) {
        fprintf(result, "%s ERROR (impossibile aprire %s)\n", tcId, fout);
        if (input) fclose(input);
        return;
    }

    /* nessun silenziamento: i printf del progetto escono su stdout normalmente */
    if      (strcmp(tcId, "TC1")  == 0) 
      eseguiTC1(input,  output);
    else if (strcmp(tcId, "TC2")  == 0) 
          eseguiTC2(input,  output);
          else if (strcmp(tcId, "TC3")  == 0) 
                eseguiTC3(input,  output);
              else if (strcmp(tcId, "TC4")  == 0) 
                    eseguiTC4(input,  output);
                  else if (strcmp(tcId, "TC5")  == 0) 
                        eseguiTC5(input,  output);
                      else if (strcmp(tcId, "TC6")  == 0) 
                          eseguiTC6(input,  output);
                          else if (strcmp(tcId, "TC7")  == 0) 
                                eseguiTC7(input,  output);
                              else if (strcmp(tcId, "TC8")  == 0) 
                                    eseguiTC8(input,  output);
                                  else if (strcmp(tcId, "TC9")  == 0) 
                                      eseguiTC9(input,  output);
                                      else if (strcmp(tcId, "TC10") == 0) 
                                            eseguiTC10(input, output);
                                          else if (strcmp(tcId, "TC11") == 0) 
                                                eseguiTC11(input, output);
                                              else if (strcmp(tcId, "TC12") == 0) 
                                                    eseguiTC12(input, output);
                                                  else if (strcmp(tcId, "TC13") == 0) 
                                                        eseguiTC13(input, output);
                                                      else fprintf(output, "TC NON RICONOSCIUTO\n");

    if (input) fclose(input);
    fclose(output);

    if (confrontaFile(fout, foracle)) {
        fprintf(result, "%s PASS\n", tcId);
        printf("Eseguito %s -> PASS\n", tcId);
    } else {
        fprintf(result, "%s FAIL\n", tcId);
        printf("Eseguito %s -> FAIL\n", tcId);
    }
}


/* =================================================================
   MAIN
   Supporta due modalita':

   1) Singolo TC (come nella slide del professore):
      ./test_main TC1 TC1_input.txt TC1_oracle.txt TC1_output.txt

   2) Intera suite:
      ./test_main --suite test_suite.txt result.txt
   ================================================================= */
int main(int argc, char* argv[]) {

    if (argc == 5) {
        /* singolo TC */
        fflush(stdout);
        freopen("/dev/null", "w", stdout);
        /* ... esegui TC ... */
        freopen("/dev/stderr", "w", stdout);
        /* stampa esito */
        return 0;
    }

    if (argc == 4 && strcmp(argv[1], "--suite") == 0) {
        FILE* suite  = fopen(argv[2], "r");
        FILE* result = fopen(argv[3], "w");

        /* questi printf sono PRIMA del freopen, quindi appaiono */
        printf("\n=== ESECUZIONE TEST SUITE ===\n");

        char tcId[32];
        while (fscanf(suite, "%s", tcId) == 1) {
            char fin[64], foracle[64], fout[64];
            sprintf(fin,     "%s_input.txt",  tcId);
            sprintf(foracle, "%s_oracle.txt", tcId);
            sprintf(fout,    "%s_output.txt", tcId);
            eseguiTC(tcId, fin, foracle, fout, result);
        }

        fclose(suite);
        fclose(result);
        printf("\n=== COMPLETATO. Risultati in %s ===\n", argv[3]);
        return 0;
    }

    printf("Uso singolo TC:   ./test <TC_id> <input> <oracle> <output>\n");
    printf("Uso intera suite: ./test --suite <suite.txt> <result.txt>\n");
    return 1;
}