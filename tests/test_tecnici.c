#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/richieste.h"
#include "../headers/tecnici.h"
#include "../headers/utile.h"
#include "../tests/test_condominio.h"

/* TC tecnici:
TC2 - Registrazione tecnico (lista tecnici)
TC3 - Assegnazione corretta richiesta <-> tecnico
TC4 - Assegnazione con fallback su GENERICO
TC5 - Assegnazione fallita (nessun tecnico disponibile)
*/

/* =================================================================
   TC2 - Registrazione di un tecnico (Lista tecnici)

   Verifica: aggiungiTecnico() con FILE* + monitoraCarico()
   NB: richiede che aggiungiTecnico accetti FILE* come parametro
   (opzione 1 del refactoring discusso).

   TC2_input.txt:
     123456789
     Mario Rossi
     0
     1

   TC2_oracle.txt:
     123456789  Mario Rossi  Idraulico  richieste: 0
   ================================================================= */
 void eseguiTC2(FILE* input, FILE* output) {
    /* aggiungiTecnico legge direttamente dal file di input del TC
       grazie al refactoring che aggiunge FILE* come parametro */
    ListaTecnici lista = nuovaLista();
    lista = aggiungiTecnico(lista, input);

    /* verifica che il tecnico sia stato inserito correttamente */
    if (lista != NULL && lista->tecnico != NULL)
        fprintf(output, "%s  %s  %s  richieste: 0\n",
                lista->tecnico->codice_ID,
                lista->tecnico->nome,
                spec_to_string(lista->tecnico->specializzazione));
    else
        fprintf(output, "ERRORE registrazione tecnico\n");

    liberaLista(lista);
}

/* =================================================================
   TC3 - Assegnazione corretta richiesta <-> tecnico

   Verifica: assegnaRichiesta() sceglie il tecnico compatibile
   con la specializzazione richiesta. Dopo l'assegnazione
   r->tecnico deve contenere il nome del tecnico.

   TC3_input.txt:
     123456789
     Mario Rossi
     0
     1
     Appartamento_3B
     0
     Perdita_lavello
     10/05/2025
     3

   TC3_oracle.txt:
     ASSEGNAZIONE OK tecnico=Mario Rossi
   ================================================================= */
 void eseguiTC3(FILE* input, FILE* output) {
    char area[MAX_STR], desc[MAX_STR], data[11];
    int  tipologia, urgenza;

    /* legge il tecnico dalle prime 4 righe tramite aggiungiTecnico */
    ListaTecnici lista = nuovaLista();
    lista = aggiungiTecnico(lista, input);

    /* legge i dati della richiesta dalle righe successive */
    fscanf(input, "%s",  area);
    fscanf(input, "%d",  &tipologia);
    fscanf(input, "%s",  desc);
    fscanf(input, "%s",  data);
    fscanf(input, "%d",  &urgenza);

    {
        Richiesta* r = creaRichiesta(1, area,
                                     (Specializzazione)tipologia,
                                     desc, data, urgenza);
        if (r == NULL) {
            fprintf(output, "ERRORE allocazione\n");
            liberaLista(lista);
            return;
        }

        lista = assegnaRichiesta(lista, (Specializzazione)tipologia, r);

        if (r->tecnico[0] != '\0')
            fprintf(output, "ASSEGNAZIONE OK tecnico=%s\n", r->tecnico);
        else
            fprintf(output, "ASSEGNAZIONE FALLITA\n");

        free(r);
    }

    liberaLista(lista);
}

/* =================================================================
   TC4 - Fallback su tecnico GENERICO

   Verifica: assegnaRichiesta() usa un GENERICO quando non trova
   un tecnico con la specializzazione richiesta.
   Il tecnico e' GENERICO (4) ma la richiesta e' ELETTRICISTA (1).

   TC4_input.txt:
     111111111
     Luigi Bianchi
     4
     1
     Appartamento_1A
     1
     Problema_elettrico
     11/05/2025
     5

   TC4_oracle.txt:
     ASSEGNAZIONE OK tecnico=Luigi Bianchi
   ================================================================= */
 void eseguiTC4(FILE* input, FILE* output) {
    char area[MAX_STR], desc[MAX_STR], data[11];
    int  tipologia, urgenza;

    /* legge tecnico GENERICO dalle prime 4 righe */
    ListaTecnici lista = nuovaLista();
    lista = aggiungiTecnico(lista, input);

    /* legge richiesta ELETTRICISTA dalle righe successive */
    fscanf(input, "%s",  area);
    fscanf(input, "%d",  &tipologia);
    fscanf(input, "%s",  desc);
    fscanf(input, "%s",  data);
    fscanf(input, "%d",  &urgenza);

    {
        Richiesta* r = creaRichiesta(1, area,
                                     (Specializzazione)tipologia,
                                     desc, data, urgenza);
        if (r == NULL) {
            fprintf(output, "ERRORE allocazione\n");
            liberaLista(lista);
            return;
        }

        /* assegnaRichiesta non trova ELETTRICISTA,
           fa fallback automatico su GENERICO */
        lista = assegnaRichiesta(lista, (Specializzazione)tipologia, r);

        if (r->tecnico[0] != '\0')
            fprintf(output, "ASSEGNAZIONE OK tecnico=%s\n", r->tecnico);
        else
            fprintf(output, "ASSEGNAZIONE FALLITA\n");

        free(r);
    }

    liberaLista(lista);
}

/* =================================================================
   TC5 - Nessun tecnico disponibile (lista vuota)

   Verifica: assegnaRichiesta() su lista vuota lascia
   r->tecnico vuoto (nessuna assegnazione possibile).

   TC5_input.txt:
     Appartamento_2C
     2
     Crepa_nel_muro
     12/05/2025
     4

   TC5_oracle.txt:
     NESSUN TECNICO DISPONIBILE
   ================================================================= */
 void eseguiTC5(FILE* input, FILE* output) {
    char area[MAX_STR], desc[MAX_STR], data[11];
    int  tipologia, urgenza;

    fscanf(input, "%s",  area);
    fscanf(input, "%d",  &tipologia);
    fscanf(input, "%s",  desc);
    fscanf(input, "%s",  data);
    fscanf(input, "%d",  &urgenza);

    {
        Richiesta* r = creaRichiesta(1, area,
                                     (Specializzazione)tipologia,
                                     desc, data, urgenza);
        if (r == NULL) { fprintf(output, "ERRORE allocazione\n"); return; }

        ListaTecnici lista = nuovaLista(); /* lista vuota: nessun tecnico */
        lista = assegnaRichiesta(lista, (Specializzazione)tipologia, r);

        /* r->tecnico deve essere rimasto vuoto */
        if (r->tecnico[0] == '\0')
            fprintf(output, "NESSUN TECNICO DISPONIBILE\n");
        else
            fprintf(output, "TECNICO ASSEGNATO ERRONEAMENTE\n");

        free(r);
    }
}