#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/richieste.h"
#include "../headers/tecnici.h"
#include "../headers/utile.h"
#include "../tests/test_condominio.h"

/* TC richieste:
TC1 - verifica registrazione richiesta
TC8: Test dell'aggiornamento di stato con transizione valida   
TC9: Verifica dell'aggiornamento di stato con transizione invalida   
TC10: Test della ricerca per codice (richiesta trovata)   
TC11: Verifica della ricerca per codice (richiesta non trovata) 
*/

//* =================================================================
   TC1 - Registrazione di una richiesta (Lista collegata)

   Verifica: creaRichiesta() + inserisciRichiesta()
   Il nodo deve avere i campi corretti e stato = APERTA.

   TC1_input.txt:
     Appartamento_3B
     0
     Perdita_sotto_lavello
     10/05/2025
     3

   TC1_oracle.txt:
     Richiesta inserita: codice=1 area=Appartamento_3B stato=APERTA
   ================================================================= */
void eseguiTC1(FILE* input, FILE* output) {
    char area[MAX_STR], desc[MAX_STR], data[11];
    int  tipologia, urgenza;

    /* lettura dei campi dal file di input, uno per riga */
    fscanf(input, "%s",  area);
    fscanf(input, "%d",  &tipologia);
    fscanf(input, "%s",  desc);
    fscanf(input, "%s",  data);
    fscanf(input, "%d",  &urgenza);

    {
        Richiesta* lista = NULL;
        Richiesta* r = creaRichiesta(1, area,
                                     (Specializzazione)tipologia,
                                     desc, data, urgenza);
        if (r == NULL) { fprintf(output, "ERRORE allocazione\n"); return; }

        inserisciRichiesta(&lista, r);

        /* verifica campi e scrive esito */
        if (r->codice == 1 &&
            strcmp(r->area, area) == 0 &&
            r->stato == APERTA)
            fprintf(output,
                "Richiesta inserita: codice=%d area=%s stato=APERTA\n",
                r->codice, r->area);
        else
            fprintf(output, "ERRORE campi non corretti\n");

        liberaListaRichieste(lista);
    }
}

/* =================================================================
   TC8 - Aggiornamento stato valido (APERTA -> PIANIFICATA)

   Verifica: aggiornaStatoDaCodice() accetta la transizione
   lecita e aggiorna correttamente r->stato.

   TC8_input.txt:
     1
     1

   TC8_oracle.txt:
     STATO AGGIORNATO: PIANIFICATA
   ================================================================= */
static void eseguiTC8(FILE* input, FILE* output) {
    int codice, nuovoStato;
    fscanf(input, "%d", &codice);
    fscanf(input, "%d", &nuovoStato);

    {
        Richiesta* lista = NULL;
        Richiesta* r = creaRichiesta(codice, "AreaTest", IDRAULICO,
                                     "Desc", "10/05/2025", 2);
        if (r == NULL) { fprintf(output, "ERRORE allocazione\n"); return; }
        inserisciRichiesta(&lista, r);

        int ok = aggiornaStatoDaCodice(lista, codice,
                                       (StatoRichiesta)nuovoStato);
        if (ok && r->stato == (StatoRichiesta)nuovoStato) {
            fprintf(output, "STATO AGGIORNATO: ");
            switch (r->stato) {
                case APERTA:         fprintf(output, "APERTA\n");         break;
                case PIANIFICATA:    fprintf(output, "PIANIFICATA\n");    break;
                case IN_LAVORAZIONE: fprintf(output, "IN_LAVORAZIONE\n"); break;
                case CONCLUSA:       fprintf(output, "CONCLUSA\n");       break;
                case ANNULLATA:      fprintf(output, "ANNULLATA\n");      break;
                default:             fprintf(output, "SCONOSCIUTO\n");    break;
            }
        } else {
            fprintf(output, "AGGIORNAMENTO FALLITO\n");
        }

        liberaListaRichieste(lista);
    }
}

/* =================================================================
   TC9 - Aggiornamento stato NON valido (transizione illegale)

   Verifica: aggiornaStatoDaCodice() rifiuta CONCLUSA -> APERTA
   e ritorna 0 senza modificare lo stato.

   TC9_input.txt:
     1
     0

   TC9_oracle.txt:
     TRANSIZIONE NON PERMESSA
   ================================================================= */
static void eseguiTC9(FILE* input, FILE* output) {
    int codice, nuovoStato;
    fscanf(input, "%d", &codice);
    fscanf(input, "%d", &nuovoStato);

    {
        Richiesta* lista = NULL;
        Richiesta* r = creaRichiesta(codice, "AreaTest", IDRAULICO,
                                     "Desc", "10/05/2025", 2);
        if (r == NULL) { fprintf(output, "ERRORE allocazione\n"); return; }

        /* setup: portiamo la richiesta a CONCLUSA prima del test */
        r->stato = CONCLUSA;
        strncpy(r->data_chiusura, "20/05/2025", 10);
        r->data_chiusura[10] = '\0';
        inserisciRichiesta(&lista, r);

        /* tentativo di tornare ad APERTA: deve fallire */
        int ok = aggiornaStatoDaCodice(lista, codice,
                                       (StatoRichiesta)nuovoStato);
        if (!ok)
            fprintf(output, "TRANSIZIONE NON PERMESSA\n");
        else
            fprintf(output, "TRANSIZIONE PERMESSA (ERRORE)\n");

        liberaListaRichieste(lista);
    }
}

/* =================================================================
   TC10 - Ricerca per codice: richiesta trovata

   Verifica: cercaPerCodice() restituisce il nodo corretto
   quando il codice e' presente nella lista.

   TC10_input.txt:
     1

   TC10_oracle.txt:
     TROVATA codice=1
   ================================================================= */
static void eseguiTC10(FILE* input, FILE* output) {
    int codice;
    fscanf(input, "%d", &codice);

    {
        Richiesta* lista = NULL;
        Richiesta* r = creaRichiesta(codice, "Appartamento_3B", IDRAULICO,
                                     "Perdita", "10/05/2025", 3);
        if (r == NULL) { fprintf(output, "ERRORE allocazione\n"); return; }
        inserisciRichiesta(&lista, r);

        Richiesta* trovata = cercaPerCodice(lista, codice);
        if (trovata != NULL)
            fprintf(output, "TROVATA codice=%d\n", trovata->codice);
        else
            fprintf(output, "NON TROVATA\n");

        liberaListaRichieste(lista);
    }
}

/* =================================================================
   TC11 - Ricerca per codice: richiesta NON trovata

   Verifica: cercaPerCodice() restituisce NULL quando il codice
   non e' presente nella lista (caso limite).

   TC11_input.txt:
     999

   TC11_oracle.txt:
     NON TROVATA
   ================================================================= */
static void eseguiTC11(FILE* input, FILE* output) {
    int codice;
    fscanf(input, "%d", &codice);

    {
        Richiesta* lista = NULL;
        /* inseriamo una richiesta con codice diverso da quello cercato */
        Richiesta* r = creaRichiesta(1, "AreaTest", IDRAULICO,
                                     "Desc", "10/05/2025", 2);
        if (r == NULL) { fprintf(output, "ERRORE allocazione\n"); return; }
        inserisciRichiesta(&lista, r);

        Richiesta* trovata = cercaPerCodice(lista, codice);
        if (trovata != NULL)
            fprintf(output, "TROVATA codice=%d\n", trovata->codice);
        else
            fprintf(output, "NON TROVATA\n");

        liberaListaRichieste(lista);
    }
}
