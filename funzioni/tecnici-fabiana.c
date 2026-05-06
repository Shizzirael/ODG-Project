#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "headers/tecnici.h"  //NON HO IDEA SE LINKA CORRETTAMENTE, CONTROLLA POI

// Definizione della struttura del nodo per la lista dei tecnici
struct nodo_tec {
    Tecnico tecnico; // Dati del tecnico
    struct nodo_tec* prossimo; // Puntatore al prossimo nodo nella lista
};

//Funzione per creare una nuova lista di tecnici
ListaTecnici nuovaLista() {
    return NULL;  // La lista è inizialmente vuota
}

//Funzione che inserisce un nuovo Tecnico in un nuovo nodo della lista e aggiorna la testa
ListaTecnici aggiungiTecnico(ListaTecnici testa)
{
    Tecnico* nuovoTecnico = malloc(sizeof(Tecnico));
    if(nuovoTecnico == NULL) {          //Se il puntatore è NULL, l'allocazione è fallita
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }
    else {      //Se l'allocazione è riuscita, crea un nuovo tecnico tramite una seconda funzione e assegna i dati
        *nuovoTecnico = creaTecnico();
    }

    struct nodo_tec* nuovo;
    nuovo = malloc(sizeof(struct nodo_tec));  // Allocazione dinamica per un nuovo nodo
    if(nuovo == NULL) {          //Se il puntatore è NULL, l'allocazione è fallita
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }
    else {                        //Se l'allocazione è riuscita, inizializza il nuovo nodo
        nuovo->tecnico = *nuovoTecnico;
        nuovo->prossimo = testa; // Il nuovo nodo punta alla vecchia testa della lista
        testa = nuovo;
    }
    return testa;
}

// Funzione per creare un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.
//DA RIVEDERE: La funzione è lunga, non so che cazzo ho combinato ma dovrebbe andare bene???
Tecnico* creaTecnico()
{
    //Buffer temporanei per leggere input id e nome, con spazio per il terminatore null
    char buffer_id[11]; 
    char buffer_nome[1000];

    printf("Aggiungi un tecnico:\n");
    Tecnico* tec = malloc(sizeof(Tecnico)); // Allocazione dinamica per un nuovo tecnico
    if(tec == NULL) {                     //controllo se l'allocazione è fallita
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }

    //CODICE ID
    printf("Codice ID (9 cifre): ");
    scanf("%s", buffer_id);
    tec->codice_ID = (char*)malloc(strlen(buffer_id) + 1);  // Allocazione dinamica per il codice ID
    if (tec->codice_ID != NULL) {
        strcpy(tec->codice_ID, buffer_id);                  //copia il contenuto dal buffer_id
    }

    //NOME
    printf("Nome: "); //dovrebbe essere possibile inserire un nome con spazi, quindi si potrebbe considerare l'uso di fgets invece di scanf per leggere l'intera riga. Tuttavia, per semplicità, si utilizza scanf in questo esempio.
    scanf(" %s", buffer_nome);
    tec->nome = (char*)malloc(strlen(buffer_nome) + 1);  // Allocazione dinamica per il nome del tecnico
    if (tec->nome != NULL) {
        strcpy(tec->nome, buffer_nome);                  // Copia il contenuto dal buffer-nome
    }

    //SPECIALIZZAZIONE
    int scelta;
    printf("Specializzazione:\n");
    printf("0) Idraulico, 1) Elettricista, 2) Muratore, 3) Ascensorista, 4) Generico\n");
    printf("Scelta: ");
    scanf("%d", &scelta);
    //A seconda della scelta dell'utente, viene allocata dinamicamente la stringa corrispondente alla specializzazione del tecnico e viene copiata in specializzazione
    switch(scelta)  
     {
        case 0: tec->specializzazione = malloc(strlen("Idraulico") + 1);
                strcpy(tec->specializzazione, "Idraulico");
                break;
        case 1: tec->specializzazione = malloc(strlen("Elettricista") + 1);
                strcpy(tec->specializzazione, "Elettricista");
                break;
        case 2: tec->specializzazione = malloc(strlen("Muratore") + 1);
                strcpy(tec->specializzazione, "Muratore");
                break;
        case 3: tec->specializzazione = malloc(strlen("Ascensorista") + 1);
                strcpy(tec->specializzazione, "Ascensorista");
                break;
        case 4: tec->specializzazione = malloc(strlen("Generico") + 1);
                strcpy(tec->specializzazione, "Generico");
                break;
        default:           printf("Scelta non valida, impostazione a 'Generico'\n"); //Se la scelta non è valida, viene impostata la specializzazione a "Generico"
                tec->specializzazione = malloc(strlen("Generico") + 1);
                strcpy(tec->specializzazione, "Generico");
                break;
    }

    //DISPONIBILITÀ
   printf("Il tecnico è disponibile? (1 per sì, 0 per no): "); //Chiede all'utente un intero che verrà interpretato come booleano
    scanf("%d", &tec->disponibilità);

    return tec; // Restituisce il tecnico creato
}