#ifndef RICHIESTA_H
#define RICHIESTA_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_STR 100

/* Enumerazione per lo stato della richiesta */
typedef enum {
    APERTA,
    PIANIFICATA,
    IN_LAVORAZIONE,
    CONCLUSA,
    ANNULLATA
} StatoRichiesta;

/* Definizione dell'ADT Richiesta */
typedef struct NodoRichiesta {
    int codice;
    char area[MAX_STR];
    char tipologia[MAX_STR];
    char descrizione[MAX_STR];
    char data[11]; /* Formato GG/MM/AAAA */
    int urgenza; /* Da 1 a 5 */
    StatoRichiesta stato;
    struct NodoRichiesta* next; /* Puntatore per la lista/coda */
} Richiesta;


Richiesta* creaRichiesta(int codice, const char* area, const char* tipologia, const char* descrizione, const char* data, int urgenza);

void aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato);

#endif /* RICHIESTA_H */