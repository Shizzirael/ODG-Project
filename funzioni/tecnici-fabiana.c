#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "headers/tecnici.h" 

// Definizione della struttura del nodo per la lista dei tecnici
struct nodo_tec {
    Tecnico* tecnico;
    struct nodo_tec* prossimo; 
};


//Funzione per creare una nuova lista di tecnici
ListaTecnici nuovaLista() {
    return NULL; 
}


// Funzione per convertire l'enum Specializzazione in una stringa
const char* spec_to_string(Specializzazione spec) 
{
     switch(spec) {
        case IDRAULICO:    return "Idraulico";
        case ELETTRICISTA: return "Elettricista";
        case MURATORE:     return "Muratore";
        case ASCENSORISTA: return "Ascensorista";
        case GENERICO:     return "Generico";
        default:           return "Generico";
    }
}


// Funzione per creare un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.
Tecnico* creaTecnico()
{
    //Buffer temporanei per leggere input id e nome, con spazio per il terminatore null
    char buffer_id[11]; 
    char buffer_nome[1000];

    printf("Aggiungi un tecnico:\n");
    Tecnico* tec = malloc(sizeof(Tecnico));
    if(tec == NULL) {                     
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }

    //CODICE ID
    while (1) {
        printf("Codice ID (9 cifre): ");

        if (scanf("%10s", buffer_id) != 1) {  // se scanf ritorna 0 o EOF, puliamo e riproviamo.
            while (getchar() != '\n');
            continue;
        }
        if (strlen(buffer_id) == 9) {
            while (getchar() != '\n');
            break;
        }
        printf("Errore: l'ID deve essere di esattamente 9 caratteri.\n");
        while (getchar() != '\n');
    }
    tec->codice_ID = malloc(strlen(buffer_id) + 1);  
    if (tec->codice_ID != NULL) {
        strcpy(tec->codice_ID, buffer_id);                  
    } else {
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }

    //NOME
    printf("Nome: ");
    fgets(buffer_nome, sizeof(buffer_nome), stdin);
    if (buffer_nome[strlen(buffer_nome) - 1] == '\n') {
        buffer_nome[strlen(buffer_nome) - 1] = '\0'; 
    }
    tec->nome = malloc(strlen(buffer_nome) + 1); 
    if (tec->nome == NULL) {
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }
    strcpy(tec->nome, buffer_nome);

    //SPECIALIZZAZIONE
    Specializzazione scelta;
    int input;
    scanf("%d", &input);

    if (input < IDRAULICO || input > GENERICO) {
        printf("Scelta non valida, impostazione a 'Generico'\n");
        scelta = GENERICO;
    } else {
        scelta = (Specializzazione)input;
    }   
    const char* str_spec = spec_to_string(scelta);
    tec->specializzazione = malloc(strlen(str_spec) + 1);
    strcpy(tec->specializzazione, str_spec);
    
    //DISPONIBILITÀ
   printf("Il tecnico è disponibile? (1 per sì, 0 per no): "); //Chiede all'utente un intero che verrà interpretato come booleano
    scanf("%d", &tec->disponibile);

    return tec; // Restituisce il tecnico creato
}


//Funzione che inserisce un nuovo Tecnico in un nuovo nodo della lista e aggiorna la testa
ListaTecnici aggiungiTecnico(ListaTecnici testa)
{
    Tecnico* nuovoTecnico = creaTecnico(); //chiama la funzione che alloca e riempie il tecnico

    struct nodo_tec* nuovo = malloc(sizeof(struct nodo_tec));  // Allocazione dinamica per un nuovo nodo della lista
    if(nuovo == NULL) {          //Se il puntatore è NULL, l'allocazione è fallita
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }                    
        nuovo->tecnico = nuovoTecnico; //Se l'allocazione è riuscita, inizializza il nuovo nodo
        nuovo->prossimo = testa; // Il nuovo nodo punta alla vecchia testa della lista
    return nuovo; // Restituisce il nuovo nodo, che diventa la nuova testa della lista
}


// Funzione per liberare la memoria allocata per la lista dei tecnici
void liberaLista(struct nodo_tec* testa) {
    struct nodo_tec* temp;
    while (testa != NULL) {
        temp = testa;
        testa = testa->prossimo;

        //Libera la memoria allocata per i campi della struttura tecnico
        free(temp->tecnico->codice_ID);
        free(temp->tecnico->nome);
        free(temp->tecnico->specializzazione);

        // Libera la memoria allocata per il tecnico stesso
        free(temp->tecnico);

        // Libera il nodo della lista
        free(temp);
    }
}