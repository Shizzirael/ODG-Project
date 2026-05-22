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

/* _________________________________________________________________
   HELPER: confrontaFile
   Confronta due file riga per riga.
   Ritorna 1 se identici, 0 altrimenti.
    */
static int confrontaFile(const char* f1, const char* f2) {
    FILE* a = fopen(f1, "r");
    FILE* b = fopen(f2, "r");
    char r1[512], r2[512]; // buffer per leggere le righe dei file
    int uguale = 1;

    if (!a || !b) { // se non riesce ad aprire uno dei file, considera i file diversi
        if (a) fclose(a);
        if (b) fclose(b);
        return 0;
    }

    while (1) {
      char* l1 = fgets(r1, sizeof(r1), a); //legge una linea dal file
      char* l2 = fgets(r2, sizeof(r2), b);
      if (!l1 && !l2) break; // se entrambi i file sono arrivati alla fine, esci dal ciclo

      if (!l1 || !l2) { // se uno dei file è arrivato alla fine prima dell'altro, i file sono diversi
        uguale = 0;
        break; 
      }

      if (strcmp(r1, r2) != 0) { // se le righe sono diverse, i file sono diversi
        uguale = 0; 
        break; 
      }
    }

    fclose(a);
    fclose(b);
    return uguale;
}

/* _________________________________________________________________
   DISPATCHER: eseguiTC
   Apre i file del TC, chiama la funzione corretta,
   confronta output vs oracle e registra l'esito.
 */
static void eseguiTC(const char* tcId, FILE* result) {
    char fin[64], foracle[64], fout[64];
    FILE *input, *output;

    sprintf(fin,     "%s_input.txt",  tcId);
    sprintf(foracle, "%s_oracle.txt", tcId);
    sprintf(fout,    "%s_output.txt", tcId);

    input  = fopen(fin,  "r");
    output = fopen(fout, "w");

    if (output == NULL) {
        fprintf(result, "%s ERROR (impossibile aprire %s)\n", tcId, fout);
        if (input) fclose(input);
        return;
    }
    /* input puo' essere NULL per TC senza file di input (es. TC13) */

    if (strcmp(tcId, "TC1")  == 0) 
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

    if (input)  fclose(input);
      fclose(output);

    if (confrontaFile(fout, foracle)) {
        fprintf(result, "%s PASS\n", tcId);
        printf("Eseguito %s -> PASS\n", tcId);
    } else {
        fprintf(result, "%s FAIL\n", tcId);
        printf("Eseguito %s -> FAIL (controlla %s vs %s)\n",
               tcId, fout, foracle);
    }
}


int main(void) {
    FILE* suite  = fopen("test_suite.txt", "r");
    FILE* result = fopen("result.txt",     "w");

    if (suite == NULL) {
        printf("Errore: impossibile aprire test_suite.txt\n");
        return 1;
    }
    if (result == NULL) {
        printf("Errore: impossibile aprire result.txt in scrittura\n");
        fclose(suite);
        return 1;
    }

    printf("\n=== ESECUZIONE TEST SUITE ===\n");

    {
        char tcId[32];
        while (fscanf(suite, "%s", tcId) == 1)
            eseguiTC(tcId, result);
    }

    fclose(suite);
    fclose(result);

    printf("\n=== COMPLETATO. Risultati in result.txt ===\n");
    
return 0;
}