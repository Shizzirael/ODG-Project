#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "headers/tecnici.h" 

// Definizione della struttura del nodo per la lista dei tecnici
struct nodo_tec {
    Tecnico* tecnico;
    int n_richieste;
    Richiesta* richieste_assegnate;  // lista già implementata in richiesta.c
    struct nodo_tec* prossimo;
};


//Funzione per creare una nuova lista di tecnici
ListaTecnici nuovaLista() {
    return NULL; 
}

//Funzione per controllare che la stringa ID sia solo cifre
int is_digits_only(const char *s, size_t n) {
    if (strlen(s) != n) return 0;
    for (size_t i = 0; i < n; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

// Funzione per creare un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.
Tecnico* creaTecnico()
{
    //Buffer temporanei per leggere input id e nome, con spazio per il terminatore null
    char buffer_id[ID_LEN + 2]; 
    char buffer_nome[1000];

    printf("Aggiungi un tecnico:\n");
    Tecnico* tec = malloc(sizeof(Tecnico));
    if(tec == NULL) {                     
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }

    //CODICE ID
    while (1) {
        printf("Codice ID (%d cifre): ", ID_LEN);

        if (scanf("%10s", buffer_id) != 1) {
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n'); // flush sempre, in ogni caso

        if(is_digits_only(buffer_id, ID_LEN)) break;

        printf("Errore: l'ID deve essere di esattamente %d cifre numeriche.\n", ID_LEN);
    }
    strncpy(tec->codice_ID, buffer_id, ID_LEN);
    tec->codice_ID[ID_LEN] = '\0';

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
  int scelta;

while (1) {
    printf("Specializzazione:\n");
    printf("0) Idraulico, 1) Elettricista, 2) Muratore, 3) Ascensorista, 4) Generico\n");
    printf("Scelta: ");

    if (scanf("%d", &scelta) != 1) {
        while (getchar() != '\n');
        continue;
    }
    while (getchar() != '\n');

    if (scelta >= 0 && scelta <= 4) break;

    printf("Errore: inserire un valore tra 0 e 4.\n");
}
tec->specializzazione = (Specializzazione)scelta;
    
    //DISPONIBILITÀ
    int disp_temp;
  while (1) {
    printf("Il tecnico è disponibile? (1 per sì, 0 per no): "); //Chiede all'utente un intero che verrà interpretato come booleano
    scanf("%d", &disp_temp);
     while (getchar() != '\n'); // flush del buffer

    if (disp_temp==1 || disp_temp==0) break;
    
    printf("Scelta non valida, inserire 0 o 1\n"); //Se la scelta non è valida, viene impostata la disponibilità a false
  }
  tec->disponibile = (disp_temp == 1);

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
        nuovo->n_richieste = 0; // Inizialmente, il nuovo tecnico non ha richieste assegnate
        nuovo->richieste_assegnate = NULL; // Inizializza la lista delle richieste assegnate a NULL
    
     return nuovo; // Restituisce il nuovo nodo, che diventa la nuova testa della lista
}


// Funzione per liberare la memoria allocata per la lista dei tecnici
void liberaLista(struct nodo_tec* testa) {
    struct nodo_tec* temp;
    while (testa != NULL) {
        temp = testa;
        testa = testa->prossimo;

        free(temp->tecnico->nome);

        free(temp->tecnico);

        free(temp); // Libera il nodo della lista
    }
}
/*CLAUDE SAYS: liberalista non libera richieste assegnate,
    se inserisci richiesta alloca memoria, andrebbe liberata prima di free(tempo-tecnico)
    dipende da come è implementata richiesta.c
*/

//-------------------------------------------------------------------------------------------------------
//FUNZIONI PER ASSEGNARE RICHIESTE AI TECNICI e ORDINARE PER NUMERO RICHIESTE ASSEGNATE

// Rimuove un nodo dalla lista senza liberarlo (serve per reinserirlo ordinato)
struct nodo_tec* rimuoviNodo(ListaTecnici testa, struct nodo_tec* nodo) {
    if (testa == NULL ) return testa; //nessuna modifica alla testa se la lista è vuota

    // caso: il nodo da rimuovere è la testa
    if (testa == nodo) {
        return testa->prossimo;
    }
    //caso: il nodo da rimuovere è in mezzo o alla fine
    struct nodo_tec* curr = testa;
    while (curr->prossimo != NULL) {
        if (curr->prossimo == nodo) {
            curr->prossimo = nodo->prossimo;
            nodo->prossimo = NULL;
            return testa;
        }
        curr = curr->prossimo;
    }
    return testa; // nodo non trovato, lista invariata
}


// Inserisce un nodo già esistente nella posizione corretta (ordinata per n_richieste)
ListaTecnici inserisciOrdinato(ListaTecnici testa, struct nodo_tec* nodo) {
    if (nodo == NULL) return testa;

    // caso: lista vuota o nodo va in testa
    if (testa == NULL || nodo->n_richieste <= testa->n_richieste) {
        nodo->prossimo = testa;
        return nodo;
    }
    // caso: nodo va in mezzo o alla fine
    struct nodo_tec* curr = testa;
    while (curr->prossimo != NULL &&
           curr->prossimo->n_richieste <= nodo->n_richieste) {
        curr = curr->prossimo;
    }
    nodo->prossimo = curr->prossimo;
    curr->prossimo = nodo;
    return testa;
}


// Cerca un tecnico disponibile con la specializzazione richiesta che abbia meno lavoro
struct nodo_tec* trovaTecnico(ListaTecnici testa, Specializzazione specializzazione) {
    struct nodo_tec* curr = testa;
    struct nodo_tec* migliore = NULL;

    while (curr != NULL) {
        if (curr->tecnico->disponibile && curr->tecnico->specializzazione == specializzazione) 
        {
            if (migliore == NULL || curr->n_richieste < migliore->n_richieste) //controllo se se ha meno richieste assegnate del migliore attuale
            {
                migliore = curr;
            }
        }
        curr = curr->prossimo;
    }
    return migliore; // Restituisce il tecnico migliore trovato, o NULL se nessuno è disponibile
}

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

// Assegna una richiesta al tecnico compatibile con meno carico
ListaTecnici assegnaRichiesta(ListaTecnici testa, Specializzazione specializzazione, Richiesta* r) {
    if (r == NULL) return testa;

    struct nodo_tec* tecnico = trovaTecnico(testa, specializzazione);
    if (tecnico == NULL) {
        printf("Nessun tecnico disponibile per: %s\n", spec_to_string(specializzazione));
        return testa;
    }

    inserisciRichiesta(&tecnico->richieste_assegnate, r); //implementata in richiesta.c da assia
    tecnico->n_richieste++;

    // rimuovi e reinserisci per mantenere la lista ordinata per carico
    testa = rimuoviNodo(testa, tecnico);
    testa = inserisciOrdinato(testa, tecnico);

    return testa;
}

//-------------------------------------------------------------------------------------------------------
//FUNZIONI PER MONITORARE IL CARICO DI LAVORO DEI TECNICI (STAMPA RICHIESTE ASSEGNATE)
void monitoraCarico(ListaTecnici testa) {
    if (testa == NULL) {
        printf("Nessun tecnico registrato.\n");
        return;
    }

    struct nodo_tec* curr = testa;
    while (curr != NULL) {
        if (curr->tecnico->disponibile) 
        {
            printf("%s  %s  %s  richieste: %d\n",
                curr->tecnico->codice_ID,
                curr->tecnico->nome,
                spec_to_string(curr->tecnico->specializzazione),
                curr->n_richieste);
            curr = curr->prossimo;
        }
        else {
            printf("%s  %s  %s  (non disponibile)\n",
                curr->tecnico->codice_ID,
                curr->tecnico->nome,
                spec_to_string(curr->tecnico->specializzazione));
            curr = curr->prossimo;
        }
    }
}