#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/richieste.h"
#include "../headers/tecnici.h"
#include "../headers/utile.h"
#include "../tests/test_condominio.h"

/* TC richieste:
TC1 - verifica registrazione richiesta
TC8 - Test dell'aggiornamento di stato con transizione valida   
TC9 - Verifica dell'aggiornamento di stato con transizione invalida   
TC10 - Test della ricerca per codice (richiesta trovata)   
TC11 - Verifica della ricerca per codice (richiesta non trovata) 
*/


void eseguiTC1(FILE* input, FILE* output) {
    char area[MAX_STR], desc[MAX_STR], data[11];
    int  tipologia, urgenza;

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

 void eseguiTC8(FILE* input, FILE* output) {
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


 void eseguiTC9(FILE* input, FILE* output) {
    int codice, nuovoStato;
    fscanf(input, "%d", &codice);
    fscanf(input, "%d", &nuovoStato);

    {
        Richiesta* lista = NULL;
        Richiesta* r = creaRichiesta(codice, "AreaTest", IDRAULICO,
                                     "Desc", "10/05/2025", 2);
        if (r == NULL) { fprintf(output, "ERRORE allocazione\n"); return; }

        r->stato = CONCLUSA;
        strncpy(r->data_chiusura, "20/05/2025", 10);
        r->data_chiusura[10] = '\0';
        inserisciRichiesta(&lista, r);

        int ok = aggiornaStatoDaCodice(lista, codice,
                                       (StatoRichiesta)nuovoStato);
        if (!ok)
            fprintf(output, "TRANSIZIONE NON PERMESSA\n");
        else
            fprintf(output, "TRANSIZIONE PERMESSA (ERRORE)\n");

        liberaListaRichieste(lista);
    }
}

 void eseguiTC10(FILE* input, FILE* output) {
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

 void eseguiTC11(FILE* input, FILE* output) {
    int codice;
    fscanf(input, "%d", &codice);

    {
        Richiesta* lista = NULL;
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
