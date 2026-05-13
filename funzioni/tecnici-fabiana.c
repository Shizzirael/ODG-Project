#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../headers/tecnici.h" 

// Definizione della struttura del nodo per la lista dei tecnici
struct nodo_tec {
    Tecnico* tecnico;
    int n_richieste;
    Richiesta* richieste_assegnate;  // lista già implementata in richiesta.c
    struct nodo_tec* prossimo;
};

//_____________________________________________________________________________________________________
//Funzione per creare una nuova lista di tecnici
ListaTecnici nuovaLista() {
    return NULL; 
}

//_____________________________________________________________________________________________________
/*
Controlla che la stringa sia composta solo da cifre e abbia una lunghezza specifica.
Utilizzata da creaTecnico per validare l'input dell'ID del tecnico.
Parametri:
- s: la stringa da controllare
- n: la lunghezza esatta che la stringa deve avere
Ritorna:
- true solo se la stringa è composta solo da cifre e ha la lunghezza n, false altrimenti.
*/
bool solo_cifre(const char *s, size_t n) {
    if (strlen(s) != n) return false;
    for (size_t i = 0; i < n; i++) {
        if (!isdigit((unsigned char)s[i])) return false; //isdigit da ctype.h, controlla se il carattere è una cifra
    }
    return true;
}

//_____________________________________________________________________________________________________
/*
Crea un nuovo tecnico. I dati del tecnico vengono inseriti dall'utente.
Utilizzata da aggiungiTecnico per creare un tecnico da inserire nella lista.
Parametri:
- Nessuno, ma la funzione richiede input da terminale per i singoli dati del tecnico.
Ritorna:
- Un puntatore a Tecnico con i dati inseriti dall'utente, o termina il programma in caso di errore di allocazione.
*/
Tecnico* creaTecnico()
{
    char buffer_id[ID_LEN + 2]; 
    char buffer_nome[1000];

    printf("Aggiungi un tecnico:\n");
    Tecnico* tec = malloc(sizeof(Tecnico));
    if(tec == NULL) {                     
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }

    //Inserimento del CODICE ID, con rispettivo controllo di validità
    while (1) {
        printf("Codice ID (%d cifre): ", ID_LEN);

        if (scanf("%10s", buffer_id) != 1) {
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n'); // flush sempre, in ogni caso

        if(solo_cifre(buffer_id, ID_LEN)) break;

        printf("Errore: l'ID deve essere di esattamente %d cifre numeriche.\n", ID_LEN);
    }
    strncpy(tec->codice_ID, buffer_id, ID_LEN);
    tec->codice_ID[ID_LEN] = '\0';

    //inserimento del NOME del tecnico, con rimozione del newline finale
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

    //inserimento della SPECIALIZZAZIONE, con controllo di validità
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
tec->specializzazione = (Specializzazione)scelta; //Specializzazione è un enum in tipi.h
    
    //inserimento della DISPONIBILITÀ
    int disp_temp;
  while (1) {
    printf("Il tecnico è disponibile? (1 per sì, 0 per no): ");
    scanf("%d", &disp_temp);
     while (getchar() != '\n'); // flush del buffer

    if (disp_temp==1 || disp_temp==0) break;
    
    printf("Scelta non valida, inserire 0 o 1\n"); //Se la scelta non è valida, viene impostata la disponibilità a false
  }
  tec->disponibile = (disp_temp == 1);

  // Restituzione finale del tecnico creato con tutti i campi valorizzati correttamente
return tec;
}


//_____________________________________________________________________________________________________
/*  
Aggiunge un nuovo tecnico alla lista dei tecnici.
Il nuovo tecnico viene creato tramite la funzione creaTecnico
Parametri:
- testa: il nodo iniziale della lista dei tecnici, può essere NULL se la lista è vuota
Ritorna:
- Il nuovo nodo creato, che diventa la nuova testa della lista dei tecnici
*/
ListaTecnici aggiungiTecnico(ListaTecnici testa)
{
    Tecnico* nuovoTecnico = creaTecnico();

    struct nodo_tec* nuovo = malloc(sizeof(struct nodo_tec));
    if(nuovo == NULL) {  
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }                    
        nuovo->tecnico = nuovoTecnico;
        nuovo->prossimo = testa;
        nuovo->n_richieste = 0; // Inizialmente, il nuovo tecnico non ha richieste assegnate
        nuovo->richieste_assegnate = NULL;
    
     return nuovo;
}


//_____________________________________________________________________________________________________
/*  
Libera la memoria allocata per la lista dei tecnici, inclusi i dati dei tecnici stessi.
Parametri:
- testa: il nodo iniziale della lista dei tecnici da liberare, può essere NULL se la lista è vuota
Ritorna:
nessun valore, ma la memoria per la lista dei tecnici viene liberata correttamente
*/
void liberaLista(struct nodo_tec* testa) {
    struct nodo_tec* temp;
    while (testa != NULL) {
        temp = testa;
        testa = testa->prossimo;

        free(temp->tecnico->nome); 
        free(temp->tecnico);
        free(temp);
    }
}

//-------------------------------------------------------------------------------------------------------
//FUNZIONI PER ASSEGNARE RICHIESTE AI TECNICI e ORDINARE PER NUMERO RICHIESTE ASSEGNATE

/*  
Rimuove un nodo dalla lista senza liberarlo
serve per reinserirlo ordinato dopo aver aggiornato il numero di richieste assegnate
Parametri:
- testa: il nodo iniziale della lista dei tecnici, può essere NULL se la lista è vuota
- nodo: il nodo da rimuovere, deve essere un nodo valido presente nella lista
Ritorna:
- La testa della lista aggiornata dopo la rimozione del nodo, o la stessa testa se il nodo non è trovato
*/
static struct nodo_tec* rimuoviNodo(ListaTecnici testa, struct nodo_tec* nodo) {
    if (testa == NULL ) return testa;

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
    return testa; //caso: nodo non trovato, restituisce la testa originale senza modifiche
}


//_____________________________________________________________________________________________________
/*  
Inserisce un nodo già esistente nella lista dei tecnici 
in modo ordinato in base al numero di richieste assegnate.
Parametri:
- testa: il nodo iniziale della lista dei tecnici, può essere NULL se la lista è vuota
- nodo: il nodo da inserire, deve essere un nodo valido con n_richieste aggiornato
Ritorna:
- La testa della lista aggiornata dopo l'inserimento del nodo nella posizione corretta
*/
static ListaTecnici inserisciOrdinato(ListaTecnici testa, struct nodo_tec* nodo) {
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


//_____________________________________________________________________________________________________
// Cerca un tecnico disponibile con la specializzazione richiesta che abbia meno lavoro
/*  
Cerca un tecnico disponibile con la specializzazione richiesta che abbia meno richieste assegnate.
Parametri:
- testa: il nodo iniziale della lista dei tecnici, può essere NULL se la lista è vuota
- specializzazione: la specializzazione richiesta per l'intervento, deve essere un valore valido dell'enum Specializzazione
Ritorna:
- Un puntatore al nodo del tecnico trovato che soddisfa i criteri, o NULL se nessun tecnico disponibile con la specializzazione richiesta è trovato
*/
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
    return migliore;
}

//_____________________________________________________________________________________________________
/*  
Converte una specializzazione in una stringa descrittiva.
Parametri:
- spec: un valore dell'enum Specializzazione da convertire, deve essere un valore valido dell'enum Specializzazione
Ritorna:
- Un puntatore a una stringa costante che rappresenta la specializzazione, o "Generico" se il valore di spec non è riconosciuto
*/
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


//______________________________LUCIA(DA CONTROLLARE_______________________________________________________________________
// trova il tecnico cercandolo per il suo nome (Aggiunta lucia)
Tecnico* trovaTecnicoPerNome(ListaTecnici testa, const char* nome) {
    struct nodo_tec* curr = testa;
    while (curr != NULL) {
        if (strcmp(curr->tecnico->nome, nome) == 0)
            return curr->tecnico;
        curr = curr->prossimo;
    }
    return NULL;
}


//_____________________________________________________________________________________________________
// Assegna una richiesta al tecnico compatibile con meno carico
/*  
Assegna una richiesta al tecnico compatibile con meno carico, aggiornando la lista dei tecnici di conseguenza.
Capisci bene che c'è scritto perchè mi sto confondendo...
*/
ListaTecnici assegnaRichiesta(ListaTecnici testa, Specializzazione specializzazione, Richiesta* r) {
    if (r == NULL) return testa;

    struct nodo_tec* tecnico = trovaTecnico(testa, specializzazione);
    if (tecnico == NULL) {
        printf("Nessun tecnico disponibile per: %s\n", specializzazioneToString(specializzazione));
        return testa;
    }

    strncpy(r->tecnico, tecnico->tecnico->nome, MAX_STR - 1);
    r->tecnico[MAX_STR - 1] = '\0';

    // Crea una copia del nodo per la lista del tecnico
    inserisciRichiesta(&tecnico->richieste_assegnate, r);
    tecnico->n_richieste++;
    
    if (copia == NULL) {
    printf("Errore: allocazione copia richiesta fallita.\n");
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

/*
Monitora il carico di lavoro dei tecnici stampando le richieste assegnate a ciascun tecnico.
Parametri:
- testa: il nodo iniziale della lista dei tecnici da monitorare, può essere NULL se la lista è vuota
Ritorna:
- Nessun valore restituito (void), ma viene prodotto un output a schermo che mostra il carico di lavoro dei tecnici  
*/
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
