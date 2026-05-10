#ifndef RICHIESTA_H
#define RICHIESTA_H
 
#include <stdbool.h>
 
#define MAX_STR 100
 
//Costanti per i tipoFiltro delle funzioni di stampa //
#define FILTRO_STATO      1
#define FILTRO_URGENZA    2
#define FILTRO_TIPOLOGIA  1
#define FILTRO_AREA       2
 
/* 
 * Enumerazione per la specializzazione del tecnico
 * (usata anche come tipologia di problema nella richiesta)
 */
typedef enum {
    IDRAULICO,
    ELETTRICISTA,
    MURATORE,
    ASCENSORISTA,
    GENERICO
} Specializzazione;
 
/*  
 * Enumerazione per lo stato della richiesta
 * */
typedef enum {
    APERTA,
    PIANIFICATA,
    IN_LAVORAZIONE,
    CONCLUSA,
    ANNULLATA
} StatoRichiesta;
 
/* =========================================================
 * Definizione dell'ADT Richiesta (nodo di lista collegata)
 * ========================================================= */
typedef struct NodoRichiesta {
    int              codice;
    char             area[MAX_STR];
    Specializzazione tipologia;        /* ora e' un enum, non piu' una stringa */
    char             descrizione[MAX_STR];
    char             data[11];         /* Formato GG/MM/AAAA + '\0' */
    int              urgenza;          /* Da 1 (bassa) a 5 (alta) */
    StatoRichiesta   stato;
    struct NodoRichiesta* next;
} Richiesta;
 
/* =========================================================
 * Prototipi delle funzioni
 * ========================================================= */
 
/*
 * Specifica Sintattica:  Richiesta* creaRichiesta(int, const char*, Specializzazione, const char*, const char*, int);
 * Specifica Semantica:   Alloca in memoria una nuova richiesta con i dati forniti.
 * Pre-condizioni:        I puntatori a stringa non devono essere NULL.
 *                        urgenza compreso tra 1 e 5.
 * Post-condizioni:       Restituisce puntatore a Richiesta con stato=APERTA e next=NULL,
 *                        oppure NULL in caso di fallimento malloc.
 */
Richiesta* creaRichiesta(int codice, const char* area, Specializzazione tipologia,
                         const char* descrizione, const char* data, int urgenza);
 
/*
 * Specifica Sintattica:  void inserisciRichiesta(Richiesta**, Richiesta*);
 * Specifica Semantica:   Aggiunge una richiesta in coda alla lista.
 * Pre-condizioni:        testa != NULL; nuova != NULL.
 * Post-condizioni:       La richiesta e' inserita come ultimo nodo della lista.
 */
void inserisciRichiesta(Richiesta** testa, Richiesta* nuova);
 
/*
 * Specifica Sintattica:  int aggiornaStato(Richiesta*, StatoRichiesta);
 * Specifica Semantica:   Modifica lo stato di una richiesta solo se la transizione e' valida.
 * Pre-condizioni:        r != NULL.
 * Post-condizioni:       Restituisce 1 se aggiornato, 0 se transizione non permessa.
 */
int aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato);
 
/*
 * Specifica Sintattica:  int aggiornaStatoDaCodice(Richiesta*, int, StatoRichiesta);
 * Specifica Semantica:   Cerca la richiesta per codice e aggiorna lo stato.
 * Pre-condizioni:        La lista puo' essere NULL (restituisce 0).
 * Post-condizioni:       Restituisce 1 se trovata e aggiornata, 0 altrimenti.
 */
int aggiornaStatoDaCodice(Richiesta* testa, int codice, StatoRichiesta nuovoStato);
 
/*
 * Specifica Sintattica:  void menuAggiornaStato(Richiesta*);
 * Specifica Semantica:   Interfaccia da terminale per aggiornare lo stato via input utente.
 * Pre-condizioni:        Nessuna (gestisce lista vuota).
 * Post-condizioni:       Lo stato della richiesta trovata viene aggiornato se valido.
 */
void menuAggiornaStato(Richiesta* testa);
 
/*
 * Specifica Sintattica:  Richiesta* cercaPerCodice(Richiesta*, int);
 * Specifica Semantica:   Ricerca lineare nella lista per codice.
 * Pre-condizioni:        Nessuna (gestisce lista vuota).
 * Post-condizioni:       Restituisce puntatore al nodo trovato, oppure NULL.
 */
Richiesta* cercaPerCodice(Richiesta* testa, int codice);
 
/*
 * Specifica Sintattica:  void stampaDettaglioRichiesta(Richiesta*);
 * Specifica Semantica:   Stampa tutti i campi di una singola richiesta.
 * Pre-condizioni:        Nessuna (gestisce r == NULL).
 * Post-condizioni:       Output formattato su stdout.
 */
void stampaDettaglioRichiesta(Richiesta* r);
 
/*
 * Specifica Sintattica:  void stampaRichiesteFiltrate(Richiesta*, int, int);
 * Specifica Semantica:   Stampa le richieste che corrispondono al filtro numerico.
 *                        tipoFiltro = FILTRO_STATO o FILTRO_URGENZA.
 * Pre-condizioni:        Nessuna (gestisce lista vuota).
 * Post-condizioni:       Output su stdout. Nessuna modifica alla lista.
 */
void stampaRichiesteFiltrate(Richiesta* testa, int valore, int tipoFiltro);
 
/*
 * Specifica Sintattica:  void stampaRichiestePerStringa(Richiesta*, const char*, int);
 * Specifica Semantica:   Stampa le richieste che corrispondono al filtro testuale.
 *                        tipoFiltro = FILTRO_AREA.
 * Pre-condizioni:        valore != NULL.
 * Post-condizioni:       Output su stdout. Nessuna modifica alla lista.
 */
void stampaRichiestePerStringa(Richiesta* testa, const char* valore, int tipoFiltro);
 
/*
 * Specifica Sintattica:  void liberaListaRichieste(Richiesta*);
 * Specifica Semantica:   Libera tutta la memoria allocata dalla lista.
 * Pre-condizioni:        Nessuna (gestisce lista vuota).
 * Post-condizioni:       Tutti i nodi sono deallocati.
 */
void liberaListaRichieste(Richiesta* testa);
 
#endif /* RICHIESTA_H */