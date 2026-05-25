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
Tecnico* creaTecnico(FILE* f)
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

        if (fscanf(f, "%10s", buffer_id) != 1) {
            while (fgetc(f) != '\n' && !feof(f));
            continue;
        }

        while (fgetc(f) != '\n' && !feof(f)); // flush sempre, in ogni caso

        if(solo_cifre(buffer_id, ID_LEN)) break;

        printf("Errore: l'ID deve essere di esattamente %d cifre numeriche.\n", ID_LEN);
    }
    strncpy(tec->codice_ID, buffer_id, ID_LEN);
    tec->codice_ID[ID_LEN] = '\0';

    //inserimento del NOME del tecnico, con rimozione del newline finale
    printf("Nome: ");
    fgets(buffer_nome, sizeof(buffer_nome), f);
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

    if (fscanf(f, "%d", &scelta) != 1) {
        while (fgetc(f) != '\n' && !feof(f));
        continue;
    }
    while (fgetc(f) != '\n' && !feof(f));

    if (scelta >= 0 && scelta <= 4) break;

    printf("Errore: inserire un valore tra 0 e 4.\n");
}
tec->specializzazione = (Specializzazione)scelta; //Specializzazione è un enum in tipi.h
    
    //inserimento della DISPONIBILITÀ
    int disp_temp;
  while (1) {
    printf("Il tecnico è disponibile? (1 per sì, 0 per no): ");
    fscanf(f, "%d", &disp_temp);
     while (fgetc(f) != '\n' && !feof(f)); // flush del buffer

    if (disp_temp==1 || disp_temp==0) break;
    
    printf("Scelta non valida, inserire 0 o 1\n"); //Se la scelta non è valida, viene impostata la disponibilità a false
  }
  tec->disponibile = (disp_temp == 1);

  // Restituzione finale del tecnico creato con tutti i campi valorizzati correttamente
return tec;
}


//______________________________________________________________________________________________________
/*
 Assegna automaticamente le richieste orfane ai tecnici disponibili.
 Parametri:
 - tecnici: puntatore alla lista dei tecnici
 - richieste: puntatore alla lista delle richieste
 Ritorna:
- Nessun valore restituito (void), ma le richieste orfane vengono assegnate ai tecnici compatibili
 */
static void assegnaRichiesteOrfane(ListaTecnici* tecnici, Richiesta* richieste) {
    Richiesta* r = richieste;
    int assegnate = 0;

    while (r != NULL) {
        if (r->stato == APERTA && r->tecnico[0] == '\0') {
            *tecnici = assegnaRichiesta(*tecnici, r->tipologia, r);
            if (r->tecnico[0] != '\0') {
                assegnate++;
            }
        }
        r = r->next;
    }
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
ListaTecnici aggiungiTecnico(ListaTecnici testa, FILE* f)
{
    Tecnico* nuovoTecnico = creaTecnico(f);

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
void liberaLista(ListaTecnici testa) {
    struct nodo_tec* temp;
    while (testa != NULL) {
        temp = testa;
        testa = testa->prossimo;
        liberaListaRichieste(temp->richieste_assegnate);
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
/*  
Cerca un tecnico disponibile con la specializzazione richiesta che abbia meno richieste assegnate.
Parametri:
- testa: il nodo iniziale della lista dei tecnici, può essere NULL se la lista è vuota
- specializzazione: la specializzazione richiesta per l'intervento, deve essere un valore valido dell'enum Specializzazione
Ritorna:
- Un puntatore al nodo del tecnico trovato che soddisfa i criteri, o NULL se nessun tecnico disponibile con la specializzazione richiesta è trovato
*/
ListaTecnici trovaTecnico(ListaTecnici testa, Specializzazione specializzazione) {
    ListaTecnici curr = testa;
    ListaTecnici migliore = NULL;

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


//_____________________________________________________________________________________________________________
/*  
Cerca un tecnico nella lista in base al suo nome.
Parametri:
- testa: il nodo iniziale della lista dei tecnici, può essere NULL se la lista è vuota
- nome: il nome del tecnico da cercare, deve essere una stringa valida
Ritorna:
- Un puntatore al nodo del tecnico trovato, o NULL se il tecnico non è trovato
*/
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
Assegna una richiesta al tecnico compatibile per specializzazione con meno carico.
Parametri:
- testa: il nodo iniziale della lista dei tecnici, può essere NULL se la lista è vuota
- specializzazione: la specializzazione richiesta per l'intervento, deve essere un valore valido dell'enum Specializzazione in tipi.h
- r: un puntatore alla richiesta da assegnare, deve essere un puntatore valido a una richiesta che si desidera assegnare
Ritorna:
- La lista dei tecnici aggiornata con la richiesta assegnata al tecnico appropriato, o NULL se non è stato possibile assegnare la richiesta a nessun tecnico disponibile
*/
ListaTecnici assegnaRichiesta(ListaTecnici testa, Specializzazione specializzazione, Richiesta* r) {
    if (r == NULL) return testa;

    struct nodo_tec* tecnico = trovaTecnico(testa, specializzazione);
// se non trova un tecnico con la specializzazione richiesta, prova ad assegnare a un generico
    if (tecnico == NULL && specializzazione != GENERICO) { 
        tecnico = trovaTecnico(testa, GENERICO);
    }

    if (tecnico == NULL) {
        printf("Nessun tecnico disponibile per: %s (nemmeno generico)\n", spec_to_string(specializzazione));
        return testa;
    }

    strncpy(r->tecnico, tecnico->tecnico->nome, MAX_STR - 1);
    r->tecnico[MAX_STR - 1] = '\0';

    // Crea una copia del nodo per la lista del tecnico
    inserisciRichiesta(&tecnico->richieste_assegnate, r);
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

//-------------------------------------------------------------------------------------------------------
/*FUNZIONE DI SUPPORTO PER IL TESTING: AGGIUNGE UN TECNICO GIA' CREATO ALLA LISTA

Aggiunge un tecnico già creato alla lista dei tecnici senza richiedere input da terminale, utile per il testing.
Parametri:
- testa: il nodo iniziale della lista dei tecnici, può essere NULL se la lista è vuota
- t: un puntatore a un tecnico già creato, deve essere un puntatore valido a una struttura Tecnico con i campi valorizzati
Ritorna:
- Il nuovo nodo creato con il tecnico t, che diventa la nuova testa della lista dei tecnici
*/
ListaTecnici aggiungiTecnicoDiretto(ListaTecnici testa, Tecnico* t) {
    struct nodo_tec* nuovo = malloc(sizeof(struct nodo_tec));
    if (nuovo == NULL) {
        printf("Errore di allocazione memoria\n");
        exit(EXIT_FAILURE);
    }
    nuovo->tecnico = t;
    nuovo->n_richieste = 0;
    nuovo->richieste_assegnate = NULL;
    nuovo->prossimo = testa;

return nuovo;
}
