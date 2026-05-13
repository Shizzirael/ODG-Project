/* 
File: tecnici.h (Modulo di gestione dei tecnici)
Autore: Fabiana Sasso
Matricola: NF12100245
Descrizione: Interfaccia per la gestione dei tecnici,
             utilizzando una lista concatenata.
*/
#ifndef TECNICI_H
#define TECNICI_H //CONTROLLA POI SE SONO LINKATE CORRETTAMENTE CON LE FUNZIONI
#include "richieste.h"
#include "tipi.h"

#define ID_LEN 9

//Struttura per rappresentare un tecnico con i suoi dati
typedef struct {    
    char codice_ID[ID_LEN + 1];
    char* nome;
    Specializzazione specializzazione;
    bool disponibile;
} Tecnico;

typedef struct nodo_tec *ListaTecnici; //struttura del nodo per la lista dei tecnici


/*____________________________________________________________________________________________________
Funzione: nuovaLista
Crea una nuova lista di tecnici, inizialmente vuota.

Parametri:
nessuno

Pre-condizioni:
nessuna

Post-condizioni:
- viene restituita una nuova lista di tecnici, rappresentata come un puntatore a nodo_tec, inizialmente impostato a NULL (lista vuota).

Ritorna:
- NULL, indicando che la lista è stata creata ma è attualmente vuota.
*/
ListaTecnici nuovaLista(); 


/*____________________________________________________________________________________________________
Funzione: solo_cifre 
Controlla che la stringa sia composta solo da cifre e abbia una lunghezza specifica.

Parametri:
- s: la stringa da controllare
- n: la lunghezza esatta che la stringa deve avere

Pre-condizioni:
- s non deve essere NULL
- n deve essere un valore positivo

Post-condizioni:
Ritorna un booleano che indica se la stringa è composta solo da cifre e ha la lunghezza n.

Ritorna:
- true se la stringa è composta solo da cifre e ha la lunghezza n, false altrimenti.
*/
bool solo_cifre(const char *s, size_t n); 


/*____________________________________________________________________________________________________
Funzione: creaTecnico
Crea un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.

Parametri:
nessuno

Pre-condizioni:
-L'utente deve inserire correttamente i campi richiesti (codice ID, nome, specializzazione e disponibilità) seguendo le indicazioni fornite durante l'inserimento.
- Il codice ID deve essere composto da esattamente 9 cifre numeriche.

Post-condizioni:
- Viene creato un nuovo Tecnico con i dati inseriti dall'utente e viene restituito un puntatore a questo tecnico.

Ritorna:
- Un puntatore a un Tecnico con i dati inseriti dall'utente, oppure termina il programma in caso di errore di allocazione della memoria.
*/
Tecnico* creaTecnico(); 


/*___________________________________________________________________________________________________
Funzione: aggiungiTecnico
Inserisce un nuovo Tecnico in un nuovo nodo della lista e aggiorna la testa.

Parametri:
- testa: il nodo iniziale della lista dei tecnici

Pre-condizioni:
- testa deve essere un puntatore valido a un nodo della lista dei tecnici e può essere NULL (lista vuota),
in quel caso verrà creato il primo nodo della lista.

Post-condizioni:
- Viene creato un nuovo nodo contenente un Tecnico con i dati inseriti dall'utente.
(il tecnico viene creato tramite la funzione creaTecnico chiamata all'interno della funzione).

Ritorna:
- Il nuovo nodo creato, che diventa la nuova testa della lista dei tecnici.
*/
ListaTecnici aggiungiTecnico(ListaTecnici testa); 


/*____________________________________________________________________________________________________
Funzione: liberaLista
Libera la memoria allocata per la lista dei tecnici.

Parametri:
- testa: il nodo iniziale della lista dei tecnici da liberare

Pre-condizioni:
- testa deve essere un puntatore valido a un nodo della lista dei tecnici, o NULL se la lista è già vuota.
- Tutti i nodi della lista devono essere stati allocati dinamicamente e devono essere validi.

Post-condizioni:
- Tutti i nodi della lista dei tecnici, inclusi i dati dei tecnici stessi, vengono deallocati correttamente, liberando tutta la memoria associata alla lista.

Ritorna:
- Nessun valore restituito (void), ma la memoria allocata per la lista dei tecnici viene liberata.
*/
void liberaLista(struct nodo_tec* testa); 


/* _____________________________________________________________________________________________________
Funzione: spec_to_string
Converte un valore dell'enum Specializzazione in una stringa rappresentativa.

Parametri:
- spec: un valore dell'enum Specializzazione da convertire in stringa.

Pre-condizioni:
- spec deve essere un valore valido dell'enum Specializzazione definito in tipi.h.

Post-condizioni:
- viene restituito un puntatore a una stringa costante che rappresenta la specializzazione corrispondente al valore di spec.

Ritorna:
- Un puntatore a una stringa costante che rappresenta la specializzazione, o "Unknown" se il valore di spec non è riconosciuto.
*/
const char* spec_to_string(Specializzazione spec);


/*____________________________________________________________________________________________________
Funzione: assegnaRichiesta
Assegna una richiesta al tecnico compatibile per specializzazione con meno carico.

Parametri:
- testa: il nodo iniziale della lista dei tecnici
- specializzazione: la specializzazione richiesta per l'intervento
- r: un puntatore alla richiesta da assegnare

Pre-condizioni:
- testa deve essere un puntatore valido a un nodo della lista dei tecnici, o NULL se la lista è vuota.
- specializzazione deve essere un valore valido dell'enum Specializzazione di tipi.h.
- r deve essere un puntatore valido a una richiesta che si desidera assegnare, e la richiesta deve essere compatibile con la specializzazione richiesta.

Post-condizioni:
- La richiesta viene assegnata al tecnico compatibile con meno carico.

Ritorna:
- La lista dei tecnici aggiornata con la richiesta assegnata al tecnico appropriato, o NULL se non è stato possibile assegnare la richiesta a nessun tecnico disponibile.
*/
ListaTecnici assegnaRichiesta(ListaTecnici testa, Specializzazione specializzazione, Richiesta* r); 


/*____________________________________________________________________________________________________
Funzione: monitoraCarico
Monitora il carico di lavoro dei tecnici stampando le richieste assegnate a ciascun tecnico.

Parametri:
- testa: il nodo iniziale della lista dei tecnici da monitorare

Pre-condizioni:
- testa deve essere un puntatore valido a un nodo della lista dei tecnici, o NULL se la lista è vuota.
- Ogni nodo della lista deve essere valido e contenere un tecnico con eventuali richieste assegnate.

Post-condizioni:
- Viene stampato a schermo il carico di lavoro di ciascun tecnico, elencando le richieste assegnate a ciascuno di essi.

Ritorna:
- Nessun valore restituito (void), ma viene prodotto un output a schermo che mostra il carico di lavoro dei tecnici.
*/
void monitoraCarico(ListaTecnici testa) ; 


//_________________________________________________________________________________
// aggiunta lucia last minute perché serve per cercare i tecnici per il loro nome
Tecnico* trovaTecnicoPerNome(ListaTecnici testa, const char* nome);

#endif