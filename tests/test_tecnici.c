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


 void eseguiTC2(FILE* input, FILE* output) {
    ListaTecnici lista = nuovaLista();
    lista = aggiungiTecnico(lista, input);

    if (lista != NULL && lista->tecnico != NULL)
        fprintf(output, "%s  %s  %s  richieste: 0\n",
                lista->tecnico->codice_ID,
                lista->tecnico->nome,
                spec_to_string(lista->tecnico->specializzazione));
    else
        fprintf(output, "ERRORE registrazione tecnico\n");

    liberaLista(lista);
}


 void eseguiTC3(FILE* input, FILE* output) {
    char area[MAX_STR], desc[MAX_STR], data[11];
    int  tipologia, urgenza;

    ListaTecnici lista = nuovaLista();
    lista = aggiungiTecnico(lista, input);

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
    }

    liberaLista(lista);
}


 void eseguiTC4(FILE* input, FILE* output) {
    char area[MAX_STR], desc[MAX_STR], data[11];
    int  tipologia, urgenza;

    ListaTecnici lista = nuovaLista();
    lista = aggiungiTecnico(lista, input);

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

    }

    liberaLista(lista);
}


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

        ListaTecnici lista = nuovaLista(); 
        lista = assegnaRichiesta(lista, (Specializzazione)tipologia, r);

        if (r->tecnico[0] == '\0')
            fprintf(output, "NESSUN TECNICO DISPONIBILE\n");
        else
            fprintf(output, "TECNICO ASSEGNATO ERRONEAMENTE\n");

        free(r);
    }
}