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

const char* spec_to_string(Specializzazione spec); // Funzione per convertire l'enum Specializzazione in una stringa

int is_digits_only(const char *s, size_t n); //Funzione per controllare che la stringa ID sia solo cifre


ListaTecnici aggiungiTecnico(ListaTecnici testa); //Funzione che inserisce un nuovo Tecnico in un nuovo nodo della lista e aggiorna la testa

ListaTecnici nuovaLista(); //Funzione per creare una nuova lista di tecnici

Tecnico* creaTecnico(); // Funzione per creare un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.

void liberaLista(struct nodo_tec* testa); // Funzione per liberare la memoria allocata per la lista dei tecnici

ListaTecnici assegnaRichiesta(ListaTecnici testa, Specializzazione specializzazione, Richiesta* r); // Assegna una richiesta al tecnico compatibile con meno carico

void monitoraCarico(ListaTecnici testa) ; // Funzione per monitorare il carico di lavoro dei tecnici (stampa richieste assegnate)

// aggiunta lucia last minute perché serve per cercare i tecnici per il loro nome
Tecnico* trovaTecnicoPerNome(ListaTecnici testa, const char* nome);

#endif