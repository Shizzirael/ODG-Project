/* File:        richieste.h (Modulo di gestione delle Richieste)
   Autore:      Assuntapia Piccolo
   Matricola:   NF12100574
   Descrizione: Interfaccia per la gestione delle richieste di manutenzione
                utilizzando una lista collegata (Linked List) e una coda
                a priorita' (Max-Heap per urgenza).
                La lista mantiene lo storico degli interventi in ordine
                cronologico di inserimento e supporta ricerche e filtri.
                L'heap consente di estrarre sempre la richiesta piu' urgente
                in tempo O(log n). */
			  
#ifndef RICHIESTE_H
#define RICHIESTE_H
 
#include <stdbool.h>
#include "tipi.h"
 
#define MAX_STR  100
#define MAX_HEAP 200   // capacita' massima della coda a priorita' (heap) 
 
// Struttura per rappresentare una singola richiesta di intervento tecnico, con tutti i dati necessari a tracciarne il ciclo di vita
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
    Richiesta* heap[MAX_HEAP + 1]; 
    int        size;               
} PriorityQueue;
 
/

/*
Funzione: creaRichiesta
Alloca in memoria una nuova richiesta con i dati forniti.

Parametri:
- codice:      identificativo univoco della richiesta
- area:        stringa che identifica l'area dell'intervento (non NULL)
- tipologia:   specializzazione richiesta per l'intervento
- descrizione: testo descrittivo del problema (non NULL)
- data:        data di apertura nel formato GG/MM/AAAA (non NULL)
- urgenza:     livello di urgenza compreso tra 1 e 5

Pre-condizioni:
- i puntatori a stringa non devono essere NULL
- urgenza compreso tra 1 e 5

Post-condizioni:
- restituisce un puntatore a Richiesta con stato=APERTA e next=NULL
- restituisce NULL in caso di fallimento malloc

Ritorna:
- puntatore alla nuova Richiesta allocata, oppure NULL
*/
Richiesta* creaRichiesta(int codice, const char* area, Specializzazione tipologia, const char* descrizione, const char* data, int urgenza);

/*
Funzione: inserisciRichiesta
Aggiunge una richiesta in coda alla lista, mantenendo l'ordine cronologico di inserimento.

Parametri:
- testa: puntatore al puntatore della testa della lista (non NULL)
- nuova: puntatore alla richiesta da inserire (non NULL)

Pre-condizioni:
- testa != NULL
- nuova != NULL

Post-condizioni:
- la richiesta e' inserita come ultimo nodo della lista

Ritorna:
- void
*/
void inserisciRichiesta(Richiesta** testa, Richiesta* nuova);

/*
Funzione: aggiornaStato
Modifica lo stato di una richiesta solo se la transizione e' valida.

Parametri:
- r:          puntatore alla richiesta da aggiornare (non NULL)
- nuovoStato: il nuovo stato da assegnare alla richiesta

Pre-condizioni:
- r != NULL

Post-condizioni:
- restituisce 1 se lo stato e' stato aggiornato con successo
- restituisce 0 se la transizione non e' permessa

Ritorna:
- 1 se aggiornato, 0 se transizione non permessa
*/
int aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato);

/*
Funzione: aggiornaStatoDaCodice
Cerca la richiesta per codice nella lista e aggiorna il suo stato.

Parametri:
- testa:      puntatore alla testa della lista (puo' essere NULL)
- codice:     codice univoco della richiesta da cercare
- nuovoStato: il nuovo stato da assegnare alla richiesta trovata

Pre-condizioni:
- la lista puo' essere NULL (restituisce 0)

Post-condizioni:
- restituisce 1 se la richiesta e' stata trovata e lo stato aggiornato
- restituisce 0 altrimenti

Ritorna:
- 1 se trovata e aggiornata, 0 altrimenti
*/
int aggiornaStatoDaCodice(Richiesta* testa, int codice, StatoRichiesta nuovoStato);

/*
Funzione: menuAggiornaStato
Interfaccia da terminale per aggiornare lo stato di una richiesta tramite input utente.

Parametri:
- testa: puntatore alla testa della lista (puo' essere NULL)

Pre-condizioni:
- nessuna (gestisce lista vuota)

Post-condizioni:
- lo stato della richiesta trovata viene aggiornato se la transizione e' valida

Ritorna:
- void
*/
void menuAggiornaStato(Richiesta* testa);

/*
Funzione: cercaPerCodice
Ricerca ricorsiva nella lista per codice univoco.

Parametri:
- testa:  puntatore alla testa della lista (puo' essere NULL)
- codice: codice univoco della richiesta da cercare

Pre-condizioni:
- nessuna (gestisce lista vuota)

Post-condizioni:
- restituisce il puntatore al nodo trovato, oppure NULL se non presente

Ritorna:
- puntatore alla Richiesta trovata, oppure NULL
*/
Richiesta* cercaPerCodice(Richiesta* testa, int codice);

/*
Funzione: cercaEStampaPerCodice
Cerca una richiesta per codice e ne stampa il dettaglio, oppure un messaggio di errore.

Parametri:
- testa:  puntatore alla testa della lista (puo' essere NULL)
- codice: codice univoco della richiesta da cercare

Pre-condizioni:
- nessuna

Post-condizioni:
- output su stdout con il dettaglio della richiesta trovata, o messaggio di errore

Ritorna:
- void
*/
void cercaEStampaPerCodice(Richiesta* testa, int codice);

/*
Funzione: cercaPerTipologia
Ricerca ricorsiva nella lista: stampa tutti i nodi con la tipologia indicata.

Parametri:
- testa:     puntatore alla testa della lista (puo' essere NULL)
- tipologia: specializzazione da ricercare

Pre-condizioni:
- nessuna (gestisce lista vuota)

Post-condizioni:
- output su stdout con tutte le richieste corrispondenti alla tipologia

Ritorna:
- void
*/
void cercaPerTipologia(Richiesta* testa, Specializzazione tipologia);

/*
Funzione: stampaDettaglioRichiesta
Stampa tutti i campi di una singola richiesta in formato leggibile.

Parametri:
- r: puntatore alla richiesta da stampare (puo' essere NULL)

Pre-condizioni:
- nessuna (gestisce r == NULL)

Post-condizioni:
- output formattato su stdout con tutti i campi della richiesta

Ritorna:
- void
*/
void stampaDettaglioRichiesta(Richiesta* r);

/*
Funzione: stampaRichiesteFiltrate
Stampa le richieste che corrispondono al filtro numerico specificato.

Parametri:
- testa:      puntatore alla testa della lista (puo' essere NULL)
- valore:     valore numerico da confrontare (valore enum dello stato, oppure livello di urgenza)
- tipoFiltro: FILTRO_STATO oppure FILTRO_URGENZA

Pre-condizioni:
- nessuna (gestisce lista vuota)

Post-condizioni:
- output su stdout con le richieste corrispondenti al filtro
- nessuna modifica alla lista

Ritorna:
- void
*/
void stampaRichiesteFiltrate(Richiesta* testa, int valore, TipoFiltro tipoFiltro);

/*
Funzione: stampaRichiestePerStringa
Stampa le richieste che corrispondono al filtro testuale specificato.

Parametri:
- testa:      puntatore alla testa della lista (puo' essere NULL)
- valore:     stringa da confrontare (non NULL)
- tipoFiltro: FILTRO_AREA oppure FILTRO_TECNICO

Pre-condizioni:
- valore != NULL

Post-condizioni:
- output su stdout con le richieste corrispondenti al filtro
- nessuna modifica alla lista

Ritorna:
- void
*/
void stampaRichiestePerStringa(Richiesta* testa, const char* valore, TipoFiltro tipoFiltro);

/*
Funzione: stampaRichiestePerTipologia
Stampa tutte le richieste con la tipologia specificata.

Parametri:
- testa:     puntatore alla testa della lista (puo' essere NULL)
- tipologia: specializzazione da ricercare

Pre-condizioni:
- nessuna

Post-condizioni:
- output su stdout con tutte le richieste corrispondenti alla tipologia

Ritorna:
- void
*/
void stampaRichiestePerTipologia(Richiesta* testa, Specializzazione tipologia);

/*
Funzione: stampaRichiestePerTecnico
Stampa tutte le richieste assegnate al tecnico indicato.

Parametri:
- testa:       puntatore alla testa della lista (puo' essere NULL)
- nomeTecnico: nome del tecnico da ricercare (non NULL)

Pre-condizioni:
- nomeTecnico != NULL

Post-condizioni:
- output su stdout con tutte le richieste assegnate al tecnico

Ritorna:
- void
*/
void stampaRichiestePerTecnico(Richiesta* testa, const char* nomeTecnico);

/*
Funzione: areaPiuProblematica
Trova e stampa l'area con il maggior numero di richieste presenti in lista.

Parametri:
- testa: puntatore alla testa della lista (puo' essere NULL)

Pre-condizioni:
- nessuna (gestisce lista vuota)

Post-condizioni:
- output su stdout con il nome dell'area piu' problematica e il relativo conteggio
- in caso di parita', viene stampata la prima area incontrata con il conteggio massimo

Ritorna:
- void
*/
void areaPiuProblematica(Richiesta* testa);

/*
Funzione: liberaListaRichieste
Libera tutta la memoria allocata dalla lista con approccio ricorsivo.

Parametri:
- testa: puntatore alla testa della lista (puo' essere NULL)

Pre-condizioni:
- nessuna (gestisce lista vuota)

Post-condizioni:
- tutti i nodi della lista sono deallocati

Ritorna:
- void
*/
void liberaListaRichieste(Richiesta* testa);
 

/*
Funzione: initPQ
Inizializza la coda a priorita', producendo un heap vuoto.

Parametri:
- pq: puntatore alla struttura PriorityQueue da inizializzare (non NULL)

Pre-condizioni:
- pq != NULL

Post-condizioni:
- pq->size == 0

Ritorna:
- void
*/
void initPQ(PriorityQueue* pq);

/*
Funzione: emptyPQ
Verifica se la coda a priorita' e' vuota.

Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)

Pre-condizioni:
- pq != NULL

Post-condizioni:
- la struttura non viene modificata
- restituisce 1 se la coda e' vuota, 0 altrimenti

Ritorna:
- 1 se vuota, 0 altrimenti
*/
int emptyPQ(const PriorityQueue* pq);

/*
Funzione: fullPQ
Verifica se la coda a priorita' ha raggiunto la capacita' massima (MAX_HEAP).

Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)

Pre-condizioni:
- pq != NULL

Post-condizioni:
- la struttura non viene modificata
- restituisce 1 se la coda e' piena, 0 altrimenti

Ritorna:
- 1 se piena, 0 altrimenti
*/
int fullPQ(const PriorityQueue* pq);

/*
Funzione: insertPQ
Inserisce una richiesta nella coda a priorita'.
L'elemento viene aggiunto come foglia e fatto risalire (upheap) finche' l'invariante Max-Heap e' ripristinato.

Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
- r:  puntatore alla richiesta da inserire (non NULL)

Pre-condizioni:
- pq != NULL
- r != NULL
- !fullPQ(pq)

Post-condizioni:
- r e' inserita nell'heap e l'invariante Max-Heap e' mantenuto
- pq->size incrementa di 1
- restituisce 1 se l'inserimento ha avuto successo, 0 se la coda e' piena

Ritorna:
- 1 se ok, 0 se la coda e' piena
*/
int insertPQ(PriorityQueue* pq, Richiesta* r);

/*
Funzione: deleteMax
Estrae e restituisce la richiesta con urgenza massima (radice heap[1]).
La foglia piu' a destra viene spostata in radice e fatta scendere (downheap) finche' l'invariante Max-Heap e' ripristinato.

Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)

Pre-condizioni:
- pq != NULL
- !emptyPQ(pq)

Post-condizioni:
- la richiesta con urgenza massima e' rimossa dall'heap
- pq->size decrementa di 1
- l'invariante Max-Heap e' ripristinato
- restituisce NULL se la coda e' vuota

Ritorna:
- puntatore alla richiesta estratta, oppure NULL se la coda e' vuota
*/
Richiesta* deleteMax(PriorityQueue* pq);

/*
Funzione: peekMax
Restituisce (senza rimuovere) la richiesta con urgenza massima.

Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)

Pre-condizioni:
- pq != NULL

Post-condizioni:
- l'heap non viene modificato
- restituisce NULL se la coda e' vuota

Ritorna:
- puntatore alla richiesta con urgenza massima, oppure NULL se la coda e' vuota
*/
Richiesta* peekMax(const PriorityQueue* pq);

/*
Funzione: stampaHeap
Stampa tutte le richieste presenti nell'heap in ordine decrescente di urgenza,
operando su una copia temporanea per non alterare la struttura originale.

Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)

Pre-condizioni:
- pq != NULL

Post-condizioni:
- output su stdout con le richieste ordinate per urgenza decrescente
- l'heap originale non viene modificato

Ritorna:
- void
*/
void stampaHeap(const PriorityQueue* pq);
 
#endif  