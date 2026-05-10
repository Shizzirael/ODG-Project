#ifndef RICHIESTA_H
#define RICHIESTA_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_STR 100

//FAB: claude mi ha detto che dovevi mettere sti quattro define per i filtri che non avevi specificato, poi controlla perché noccapito dove li hai usati
#define FILTRO_STATO     0
#define FILTRO_URGENZA   1
#define FILTRO_TIPOLOGIA 2
#define FILTRO_AREA      3

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

int aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato); //FAB: in richiesta.c avevi messo valore di ritorno int quindi ho cambiato

//FAB: queste funzioni le avevi scritte in c ma non le avevi dichiarate in h
void        inserisciRichiesta(Richiesta** testa, Richiesta* nuova);
int         aggiornaStatoDaCodice(Richiesta* testa, int codice, StatoRichiesta nuovoStato);
void        menuAggiornaStato(Richiesta* testa);
Richiesta*  cercaPerCodice(Richiesta* testa, int codice);
void        stampaDettaglioRichiesta(Richiesta* r);
void        stampaRichiesteFiltrate(Richiesta* testa, int filtro, int tipoFiltro);
void        stampaRichiestePerStringa(Richiesta* testa, const char* valore, int tipoFiltro);
void        liberaListaRichieste(Richiesta* testa);


#endif /* RICHIESTA_H */