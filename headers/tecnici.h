#ifndef TECNICI_H
#define TECNICI_H //CONTROLLA POI SE SONO LINKATE CORRETTAMENTE CON LE FUNZIONI
#include "richieste.h"

//Struttura per rappresentare un tecnico con i suoi dati
typedef struct {    
    char codice_ID[10];
    char* nome;
    const char* specializzazione;
    bool disponibile;
} Tecnico;

//struttura del nodo per la lista dei tecnici
typedef struct nodo_tec *ListaTecnici;

const char* spec_to_string(Specializzazione spec); // Funzione per convertire l'enum Specializzazione in una stringa
ListaTecnici aggiungiTecnico(ListaTecnici testa, Tecnico* nuovoTecnico); //Funzione che inserisce un nuovo Tecnico in un nuovo nodo della lista e aggiorna la testa
ListaTecnici nuovaLista(); //Funzione per creare una nuova lista di tecnici
Tecnico* creaTecnico(); // Funzione per creare un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.
void liberaLista(struct nodo_tec* testa); // Funzione per liberare la memoria allocata per la lista dei tecnici

ListaTecnici assegnaRichiesta(ListaTecnici testa, const char* specializzazione, Richiesta* r); // Assegna una richiesta al tecnico compatibile con meno carico

void monitoraCarico(ListaTecnici testa) ; // Funzione per monitorare il carico di lavoro dei tecnici (stampa richieste assegnate)

#endif