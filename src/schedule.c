#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/schedule.h"


// NON VISIBILI !!!

// struttura nodo
struct node_schedule {
    Data dataAppuntamento;
    Richiesta req;
    Tecnico* tec;
    struct node_schedule *left;
    struct node_schedule *right;
};

/*_____________________________________________________________________________________________________  
Libera la memoria allocata per l'intero albero binario.
Scorre l'albero in post-order per deallocare prima i figli e poi il nodo padre, pk se fai il contrario perdi i riferimenti per scendere.
Parametri:
- root: la radice dell'albero (o sotto-albero) da liberare
Ritorna:
- Nessun valore (void)
*/
void liberaAlbero(Schedule root) {
    if (root == NULL) return;
    liberaAlbero(root->left);
    liberaAlbero(root->right);
    free(root);
}

/*_____________________________________________________________________________________________________  
Confronta due date per capire quale viene prima cronologicamente o se c'è una sovrapposizione.
Controlla prima l'anno, poi mese, giorno e infine le ore. 
Parametri:
- d1: la prima data da confrontare
- d2: la seconda data da confrontare
Ritorna:
- -1 se d1 è precedente a d2 (o finisce prima/uguale a quando d2 inizia)
- 1 se d1 è successiva a d2 (o inizia dopo/uguale a quando d2 finisce)
- 0 se le date si accavallano (cioè tutti i controlli falliscono)
*/
static int confrontaDate(Data d1, Data d2){
    if(d1.anno != d2.anno){
        return (d1.anno < d2.anno) ? -1 : 1;
    }
    if(d1.mese != d2.mese){
        return (d1.mese < d2.mese) ? -1 : 1;
    }
    if(d1.giorno != d2.giorno){
        return (d1.giorno < d2.giorno) ? -1 : 1;
    }
    if(d1.ora_fine <= d2.ora_inizio){
        return -1;
    }
    if(d1.ora_inizio >= d2.ora_fine){
        return 1;
    }
    return 0; //se le date sono uguali ritorna zero pk tutti confronti sono falliti
}

/*_____________________________________________________________________________________________________  
Confronta due date per capire quale viene prima cronologicamente o se c'è una sovrapposizione.
Controlla prima l'anno, poi mese, giorno e infine le ore. 
Parametri:
- d1: la prima data da confrontare
- d2: la seconda data da confrontare
Ritorna:
- -1 se d1 è precedente a d2 (o finisce prima/uguale a quando d2 inizia)
- 1 se d1 è successiva a d2 (o inizia dopo/uguale a quando d2 finisce)
- 0 se le date si accavallano (cioè tutti i controlli falliscono)
*/
static Schedule creaNodo(Richiesta req, Tecnico* tec, Data data){
    Schedule nuovo = malloc(sizeof(struct node_schedule));
    if (nuovo != NULL){
        nuovo->dataAppuntamento = data;
        nuovo->req = req;
        nuovo->tec = tec;
        nuovo->left = NULL;
        nuovo->right = NULL;
    }
    return nuovo;
}

/*_____________________________________________________________________________________________________  
Stampa i nomi dei tecnici per tutti gli interventi che hanno come stato CONCLUSA.
Fa una visita in-order dell'albero così li vedi in ordine cronologico.
Parametri:
- root: la radice dell'albero da visitare
Ritorna:
- Nessun valore (void)
*/
static void stampaTecniciConclusi(Schedule root) {
    if (root == NULL) return;

    stampaTecniciConclusi(root->left);

    if (root->req.stato == CONCLUSA) {
        printf("- %s\n", root->tec->nome);
    }

    stampaTecniciConclusi(root->right);
}

/*_____________________________________________________________________________________________________  
Scorre l'albero e aggiorna un array di contatori per tenere traccia di quanti interventi ci sono per ogni tipologia.
Parametri:
- root: la radice dell'albero da analizzare
- conteggi: array di interi in cui vengono salvati i totali (l'indice corrisponde all'enum della tipologia)
Ritorna:
- Nessun valore (void)
*/
static void contaPerTipologia(Schedule root, int* conteggi) {
    if (root == NULL) return;
    contaPerTipologia(root->left, conteggi);
    conteggi[(int)root->req.tipologia]++;
    contaPerTipologia(root->right, conteggi);
}

// FUNZIONI PUBBLICHE!!!

// crea l'albero vuoto
Schedule creaAlbero(){
    return NULL;
}

/*_____________________________________________________________________________________________________  
Cerca all'interno dell'albero se la data target si accavalla con un appuntamento già esistente.
Usa confrontaDate per capire in che direzione scendere nel BST.
Parametri:
- root: la radice dell'albero degli appuntamenti
- target: la data di cui verificare la disponibilità
Ritorna:
- 1 se c'è un conflitto (le date si sovrappongono)
- 0 se non ci sono conflitti (slot libero)
*/
int verificaConflitti(Schedule root, Data target){
    if (root == NULL){
        return 0;
    }
    int risultato = confrontaDate(target, root->dataAppuntamento);

    if (risultato == 0){
        return 1;
    }
    if (risultato < 0){
        return verificaConflitti(root->left, target);
    }
    else{
        return verificaConflitti(root->right, target);
    }
}

/*_____________________________________________________________________________________________________  
Inserisce un nuovo intervento nell'albero in base alla data, mantenendo l'ordinamento del BST.
Imposta anche in automatico lo stato della richiesta a PIANIFICATA.
Parametri:
- root: la radice dell'albero
- req: i dettagli della richiesta da pianificare
- tec: puntatore al tecnico assegnato
- data: la data concordata per l'intervento
Ritorna:
- La radice dell'albero aggiornata con il nuovo nodo
*/
Schedule planIntervento(Schedule root, Richiesta req, Tecnico* tec, Data data) {
    req.stato = PIANIFICATA;
    if (root == NULL) return creaNodo(req, tec, data);

    int risultato = confrontaDate(data, root->dataAppuntamento);

    if (risultato < 0)
        root->left  = planIntervento(root->left,  req, tec, data);

    else if (risultato > 0)
        root->right = planIntervento(root->right, req, tec, data);

    return root;
} 


/*_____________________________________________________________________________________________________  
Stampa tutti gli interventi conclusi in ordine cronologico.
Può stampare a video o scrivere su file a seconda del puntatore FILE che gli passi.
Parametri:
- root: la radice dell'albero
- out: puntatore al file di output (può anche essere stdout per il terminale)
Ritorna:
- Nessun valore (void)
*/
void stampaStorico(Schedule root, FILE* out) {
    if (root == NULL) return;
    stampaStorico(root->left, out);

    if (root->req.stato == CONCLUSA) {
        fprintf(out, "%02d/%02d/%d %02d:00 | Tecnico: %s | ID: %d\n",
            root->dataAppuntamento.giorno,
            root->dataAppuntamento.mese,
            root->dataAppuntamento.anno,
            root->dataAppuntamento.ora_inizio,
            root->tec->nome,
            root->req.codice);
    } 

    stampaStorico(root->right, out);
}


/*_____________________________________________________________________________________________________  
Cerca un intervento specifico tramite il suo ID (codice).
Deve scorrersi per forza tutto l'albero pk i nodi sono ordinati per date, non per codici.
Parametri:
- root: la radice dell'albero in cui cercare
- codiceTarget: l'ID dell'intervento che vuoi trovare
Ritorna:
- 1 se l'intervento viene trovato (e lo stampa)
- 0 se l'intervento non esiste nell'albero
*/
int cercaIntervento(Schedule root, int codiceTarget) {
    if (root == NULL){
        return 0;
    }

    if (root->req.codice == codiceTarget) {
        printf("Trovato intervento: %d/%d/%d\n",
            root->dataAppuntamento.giorno,
            root->dataAppuntamento.mese,
            root->dataAppuntamento.anno);
        return 1;
    }
    if (cercaIntervento(root->left,  codiceTarget)){
        return 1;
    }
    return cercaIntervento(root->right, codiceTarget);
}


/*_____________________________________________________________________________________________________  
Conta quanti interventi sono aperti (non conclusi) e quanti invece sono già stati chiusi.
Usa i puntatori per aggiornare i valori all'esterno della funzione.
Parametri:
- root: la radice dell'albero
- aperti: puntatore alla variabile che conta gli interventi aperti/pianificati
- conclusi: puntatore alla variabile che conta gli interventi completati
Ritorna:
- Nessun valore (void)
*/
void generaReport(Schedule root, int* aperti, int* conclusi) {
    if (root == NULL) return;

    if (root->req.stato == CONCLUSA){
        (*conclusi)++;
    }
    else{
        (*aperti)++;
    }

    generaReport(root->left,  aperti, conclusi);
    generaReport(root->right, aperti, conclusi);
}

/*_____________________________________________________________________________________________________  
Calcola la differenza in giorni tra due date (passate come stringhe).
Usa un'approssimazione (mesi da 30 e anni da 365) pk fare un calcolo precisissimo coi bisestili in questo contesto non serve.
Parametri:
- d1: la prima data in formato stringa "gg/mm/aaaa"
- d2: la seconda data in formato stringa "gg/mm/aaaa"
Ritorna:
- La differenza in giorni (d2 - d1) come numero intero
*/
static int giorniTra(const char* d1, const char* d2) {
    int g1, m1, a1, g2, m2, a2;
    sscanf(d1, "%d/%d/%d", &g1, &m1, &a1);
    sscanf(d2, "%d/%d/%d", &g2, &m2, &a2);
    // approssimazione
    int tot1 = a1*365 + m1*30 + g1;
    int tot2 = a2*365 + m2*30 + g2;
    return tot2 - tot1;
}

/*_____________________________________________________________________________________________________  
Somma i giorni trascorsi tra l'apertura e la chiusura per tutti gli interventi conclusi e li conta.
Così dopo ti basta fare somma / count per ottenere il tempo medio.
Parametri:
- root: la radice dell'albero
- somma: puntatore alla variabile dove accumulare il totale dei giorni
- count: puntatore alla variabile dove contare gli interventi validi per la media
Ritorna:
- Nessun valore (void)
*/
void tempoMedioCompletamento(Schedule root, int* somma, int* count) {
    if (root == NULL) return;

    tempoMedioCompletamento(root->left, somma, count);

    if (root->req.stato == CONCLUSA) {
        int giorni = giorniTra(root->req.data, root->req.data_chiusura); 
        if (giorni >= 0) {
            (*somma) += giorni;
            (*count)++;
        }
    }

    tempoMedioCompletamento(root->right, somma, count);
}

/*_____________________________________________________________________________________________________  
Stampa a schermo l'elenco dei tecnici che hanno chiuso interventi.
Si appoggia alla funzione helper stampaTecniciConclusi per tirare fuori la lista.
Parametri:
- root: la radice dell'albero
Ritorna:
- Nessun valore (void)
*/
void trovaTecnicoPiuAttivo(Schedule root) {
    if (root == NULL) {
        printf("Nessun intervento concluso.\n");
        return;
    }
    printf("Tecnici con interventi conclusi:\n");
    stampaTecniciConclusi(root);
    printf("(il tecnico che appare piu' volte e' il piu' attivo)\n");
}

/*_____________________________________________________________________________________________________  
Mostra un recap di quanti interventi sono stati fatti per ogni tipo di specializzazione.
Inizializza un array a zero e lo fa riempire da contaPerTipologia.
Parametri:
- root: la radice dell'albero
Ritorna:
- Nessun valore (void)
*/
void interventiPerTipologia(Schedule root) {
    if (root == NULL) return;

    int conteggi[5] = {0};
    contaPerTipologia(root, conteggi); // funzione privata

    printf("\n--- INTERVENTI PER TIPOLOGIA ---\n");
    printf("Idraulico:    %d\n", conteggi[IDRAULICO]);
    printf("Elettricista: %d\n", conteggi[ELETTRICISTA]);
    printf("Muratore:     %d\n", conteggi[MURATORE]);
    printf("Ascensorista: %d\n", conteggi[ASCENSORISTA]);
    printf("Generico:     %d\n", conteggi[GENERICO]);
}

/*_____________________________________________________________________________________________________  
Trova un intervento tramite ID e ne aggiorna lo stato.
Se lo stato che gli passi è CONCLUSA, setta anche la data di chiusura.
Parametri:
- root: la radice dell'albero
- codice: l'ID dell'intervento da modificare
- nuovoStato: lo stato da assegnare (es. CONCLUSA, PIANIFICATA)
- dataChiusura: stringa con la data in cui è stato chiuso (può essere NULL se lo stato non è CONCLUSA)
Ritorna:
- 1 se trova l'intervento e lo aggiorna
- 0 se l'intervento con quel codice non esiste
*/
int aggiornaStatoNelBST(Schedule root, int codice, StatoRichiesta nuovoStato, const char* dataChiusura) {
    if (root == NULL) return 0;
    if (root->req.codice == codice) {
        root->req.stato = nuovoStato;
        if (nuovoStato == CONCLUSA && dataChiusura != NULL)
            strncpy(root->req.data_chiusura, dataChiusura, 10);
        return 1;
    }
    if (aggiornaStatoNelBST(root->left,  codice, nuovoStato, dataChiusura)) return 1;
    return aggiornaStatoNelBST(root->right, codice, nuovoStato, dataChiusura);
}
