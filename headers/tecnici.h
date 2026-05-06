#ifndef TECNICI_H
#define TECNICI_H //CONTROLLA POI SE SONO LINKATE CORRETTAMENTE CON LE FUNZIONI

//struttura del nodo per la lista dei tecnici
typedef struct nodo_tec *ListaTecnici;

//Struttura per rappresentare un tecnico con i suoi dati
typedef struct {    
    char codice_ID[10];
    char* nome;
    char* specializzazione;
    bool disponibilità;
} Tecnico;

ListaTecnici nuovaLista(); //Funzione per creare una nuova lista di tecnici
ListaTecnici aggiungiTecnico(ListaTecnici testa, Tecnico* nuovoTecnico); //Funzione che inserisce un nuovo Tecnico in un nuovo nodo della lista e aggiorna la testa
Tecnico* creaTecnico(); // Funzione per creare un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.

#endif