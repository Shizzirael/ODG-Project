/* 
 File:        schedule.h (Modulo di Pianificazione)
 Autore:      Lucia Schettino
 Matricola:   NF12100025
 Descrizione: Interfaccia per la gestione degli interventi
              utilizzando un Albero Binario di Ricerca (BST).
              L'albero e' ordinato per data e fascia oraria:
              ogni nodo rappresenta un intervento pianificato
              con il tecnico assegnato e la richiesta associata.
*/

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "richieste.h"
#include "tecnici.h"

/*
 Struttura Data: rappresenta la data e la fascia oraria di un intervento.
 ora_inizio e ora_fine definiscono la fascia (es. 9 e 11 = dalle 9:00 alle 11:00).
 */
typedef struct {
    int anno;
    int mese;
    int giorno;
    int ora_inizio;
    int ora_fine;
} Data;

/* Tipo opaco: puntatore al nodo interno dell'albero. */
typedef struct node_schedule *Schedule;


/*____________________________________________________________________________________________________
Funzione: creaAlbero
Crea e restituisce un albero BST vuoto, pronto per ricevere interventi pianificati.

Parametri:
- Nessuno.

Pre-condizioni:
- Nessuna.

Post-condizioni:
- Restituisce NULL, che rappresenta convenzionalmente l'albero vuoto.

Ritorna:
- Schedule (NULL) che rappresenta un albero vuoto pronto per l'uso.
*/
Schedule creaAlbero(void);


/*____________________________________________________________________________________________________
Funzione: verificaConflitti
Verifica se nella fascia oraria indicata da target esiste gia' un intervento pianificato nell'albero.

Parametri:
- root:   radice dell'albero BST degli interventi pianificati.
- target: data e fascia oraria da verificare.

Pre-condizioni:
- target deve contenere valori coerenti (ora_fine > ora_inizio).
- root puo' essere NULL (albero vuoto, nessun conflitto possibile).

Post-condizioni:
- L'albero non viene modificato.

Ritorna:
- 1 se esiste un conflitto con un intervento gia' presente, 0 altrimenti.
*/
int verificaConflitti(Schedule root, Data target);


/*____________________________________________________________________________________________________
Funzione: planIntervento
Inserisce un nuovo intervento nell'albero BST ordinato per data e fascia oraria.
Lo stato della richiesta viene impostato a PIANIFICATA al momento dell'inserimento.
Il chiamante e' responsabile di invocare verificaConflitti prima di questa funzione.

Parametri:
- root: radice corrente dell'albero (NULL se l'albero e' vuoto).
- req:  copia della richiesta da associare all'intervento.
- tec:  puntatore al tecnico assegnato (puo' essere NULL se non ancora assegnato).
- data: data e fascia oraria dell'intervento.

Pre-condizioni:
- data deve contenere valori coerenti (ora_fine > ora_inizio).
- Non devono esistere conflitti per la fascia oraria indicata.

Post-condizioni:
- Un nuovo nodo viene inserito nell'albero nella posizione corretta.
- req.stato e' impostato a PIANIFICATA nel nodo inserito.

Ritorna:
- Schedule — nuova radice dell'albero aggiornata.
*/
Schedule planIntervento(Schedule root, Richiesta req, Tecnico* tec, Data data);


/*____________________________________________________________________________________________________
Funzione: stampaStorico
Stampa in ordine cronologico tutti gli interventi il cui stato e' CONCLUSA,
mostrando data, tecnico assegnato e codice della richiesta.

Parametri:
- root: radice dell'albero BST degli interventi.

Pre-condizioni:
- root puo' essere NULL (nessun output prodotto).
- I nodi con stato CONCLUSA devono avere tec != NULL per evitare errori.

Post-condizioni:
- L'albero non viene modificato. Output su stdout.

Ritorna:
- Nessun valore restituito (void), ma viene prodotto un output su stdout.
*/
void stampaStorico(Schedule root);


/*____________________________________________________________________________________________________
Funzione: cercaIntervento
Cerca nell'intero albero il nodo con il codice richiesta corrispondente a codiceTarget.
La ricerca e' completa (non BST-guidata) perche' l'albero e' ordinato per data, non per codice.
Se trovato, stampa la data dell'intervento.

Parametri:
- root:         radice dell'albero BST.
- codiceTarget: codice univoco della richiesta da cercare.

Pre-condizioni:
- root puo' essere NULL (restituisce 0).
- codiceTarget deve essere un intero positivo.

Post-condizioni:
- L'albero non viene modificato. Se trovato, stampa su stdout la data dell'intervento.

Ritorna:
- 1 se l'intervento e' stato trovato, 0 altrimenti.
*/
int cercaIntervento(Schedule root, int codiceTarget);


/*____________________________________________________________________________________________________
Funzione: generaReport
Visita l'intero albero e conta gli interventi suddividendoli in aperti e conclusi.
Gli interventi in stato CONCLUSA incrementano *conclusi, tutti gli altri incrementano *aperti.

Parametri:
- root:     radice dell'albero BST.
- aperti:   puntatore al contatore degli interventi non conclusi.
- conclusi: puntatore al contatore degli interventi conclusi.

Pre-condizioni:
- aperti != NULL e conclusi != NULL.
- I contatori devono essere inizializzati a 0 dal chiamante prima della chiamata.
- root puo' essere NULL (i contatori rimangono invariati).

Post-condizioni:
- *aperti e *conclusi vengono aggiornati. L'albero non viene modificato.

Ritorna:
- Nessun valore restituito (void), ma *aperti e *conclusi vengono aggiornati.
*/
void generaReport(Schedule root, int* aperti, int* conclusi);


/*____________________________________________________________________________________________________
Funzione: tempoMedioCompletamento
Per ogni intervento concluso, calcola la differenza in giorni tra data di apertura
e data di chiusura della richiesta e la accumula in *somma. Incrementa *count per
ogni intervento valido. Il chiamante calcola la media dividendo *somma per *count.

Parametri:
- root:  radice dell'albero BST.
- somma: puntatore alla somma totale dei giorni di completamento.
- count: puntatore al numero di interventi conclusi considerati validi.

Pre-condizioni:
- somma != NULL e count != NULL.
- I valori puntati devono essere inizializzati a 0 dal chiamante prima della chiamata.
- root puo' essere NULL (i valori rimangono invariati).

Post-condizioni:
- *somma e *count vengono aggiornati. L'albero non viene modificato.
- Le richieste con differenza di giorni negativa (dati incoerenti) vengono ignorate.

Ritorna:
- Nessun valore restituito (void), ma *somma e *count vengono aggiornati.
*/
void tempoMedioCompletamento(Schedule root, int* somma, int* count);


/*____________________________________________________________________________________________________
Funzione: trovaTecnicoPiuAttivo
Stampa i nomi di tutti i tecnici associati a interventi in stato CONCLUSA, in ordine cronologico.
Il tecnico che compare piu' volte nell'output e' il piu' attivo.

Parametri:
- root: radice dell'albero BST.

Pre-condizioni:
- root puo' essere NULL (stampa un messaggio di avviso).
- I nodi con stato CONCLUSA devono avere tec != NULL per evitare errori.

Post-condizioni:
- L'albero non viene modificato. Output su stdout.

Ritorna:
- Nessun valore restituito (void), ma viene prodotto un output su stdout.
*/
void trovaTecnicoPiuAttivo(Schedule root);


/*____________________________________________________________________________________________________
Funzione: interventiPerTipologia
Conta e stampa il numero di interventi presenti nell'albero per ciascuna tipologia
di specializzazione (IDRAULICO, ELETTRICISTA, MURATORE, ASCENSORISTA, GENERICO),
indipendentemente dallo stato dell'intervento.

Parametri:
- root: radice dell'albero BST.

Pre-condizioni:
- root puo' essere NULL (nessun output prodotto).

Post-condizioni:
- L'albero non viene modificato. Output su stdout.

Ritorna:
- Nessun valore restituito (void), ma viene prodotto un output su stdout.
*/
void interventiPerTipologia(Schedule root);


/*____________________________________________________________________________________________________
Funzione: liberaAlbero
Dealloca ricorsivamente tutti i nodi dell'albero BST con visita post-order
(prima i sottoalberi sinistro e destro, poi il nodo corrente).
Non dealloca le strutture Tecnico* puntate dai nodi, poiche' la loro
ownership appartiene alla lista dei tecnici (modulo tecnici.c).

Parametri:
- root: radice dell'albero da deallocare.

Pre-condizioni:
- root puo' essere NULL (nessuna operazione eseguita).

Post-condizioni:
- Tutti i nodi dell'albero sono deallocati.
- I puntatori Tecnico* contenuti nei nodi non vengono toccati.

Ritorna:
- Nessun valore restituito (void).
*/
void liberaAlbero(Schedule root);

//TODO aggiungi specifiche 
int aggiornaStatoNelBST(Schedule root, int codice, StatoRichiesta nuovoStato, const char* dataChiusura);

#endif