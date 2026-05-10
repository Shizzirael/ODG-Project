#ifndef TECNICI_H
#define TECNICI_H //CONTROLLA POI SE SONO LINKATE CORRETTAMENTE CON LE FUNZIONI

//struttura del nodo per la lista dei tecnici
typedef struct nodo_tec *ListaTecnici;

//Struttura per rappresentare un tecnico con i suoi dati
typedef struct {    
    char codice_ID[10];
    char* nome;
    char* specializzazione;
    bool disponibile;
} Tecnico;

typedef enum {
    IDRAULICO     = 0,
    ELETTRICISTA  = 1,
    MURATORE      = 2,
    ASCENSORISTA  = 3,
    GENERICO      = 4
} Specializzazione;

const char* spec_to_string(Specializzazione spec); // Funzione per convertire l'enum Specializzazione in una stringa
ListaTecnici aggiungiTecnico(ListaTecnici testa, Tecnico* nuovoTecnico); //Funzione che inserisce un nuovo Tecnico in un nuovo nodo della lista e aggiorna la testa
ListaTecnici nuovaLista(); //Funzione per creare una nuova lista di tecnici
Tecnico* creaTecnico(); // Funzione per creare un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.
void liberaLista(struct nodo_tec* testa); // Funzione per liberare la memoria allocata per la lista dei tecnici

#endif