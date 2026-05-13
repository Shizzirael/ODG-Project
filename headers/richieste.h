#ifndef RICHIESTE_H
#define RICHIESTE_H
 
#include <stdbool.h>
#include "tipi.h"
 
#define MAX_STR  100
#define MAX_HEAP 200   /* capacita' massima della coda a priorita' (heap) */
 

typedef struct NodoRichiesta {
    int                   codice;
    char                  area[MAX_STR];
    Specializzazione      tipologia;
    char                  descrizione[MAX_STR];
    char                  data[11];
    char                  data_chiusura[11];
    char                  tecnico[MAX_STR];
    int                   urgenza;
    StatoRichiesta        stato;
    struct NodoRichiesta* next;
} Richiesta;
 

typedef struct {
    Richiesta* heap[MAX_HEAP + 1]; /* heap[0] non usato (convenzione 1-based) */
    int        size;               /* numero di elementi presenti              */
} PriorityQueue;

 
Richiesta* creaRichiesta(int codice, const char* area, Specializzazione tipologia,
                         const char* descrizione, const char* data, int urgenza);
 

void inserisciRichiesta(Richiesta** testa, Richiesta* nuova);
 

int aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato);

int aggiornaStatoDaCodice(Richiesta* testa, int codice, StatoRichiesta nuovoStato);
 

void menuAggiornaStato(Richiesta* testa);

Richiesta* cercaPerCodice(Richiesta* testa, int codice);

void cercaEStampaPerCodice(Richiesta* testa, int codice);
 

void cercaPerTipologia(Richiesta* testa, Specializzazione tipologia);

void stampaDettaglioRichiesta(Richiesta* r);
 
void stampaRichiesteFiltrate(Richiesta* testa, int valore, TipoFiltro tipoFiltro);
 

void stampaRichiestePerStringa(Richiesta* testa, const char* valore, TipoFiltro tipoFiltro);
 

void stampaRichiestePerTipologia(Richiesta* testa, Specializzazione tipologia);
 

void stampaRichiestePerTecnico(Richiesta* testa, const char* nomeTecnico);
 

void areaPiuProblematica(Richiesta* testa);
 
void liberaListaRichieste(Richiesta* testa);
 
t-condizioni:       pq->size == 0.
 
void initPQ(PriorityQueue* pq);

int emptyPQ(const PriorityQueue* pq);
 

int fullPQ(const PriorityQueue* pq);
 

int insertPQ(PriorityQueue* pq, Richiesta* r);
 

Richiesta* deleteMax(PriorityQueue* pq);
 

Richiesta* peekMax(const PriorityQueue* pq);
 

void stampaHeap(const PriorityQueue* pq);
 
#endif /* RICHIESTE_H */