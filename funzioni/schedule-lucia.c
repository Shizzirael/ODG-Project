#include <stdio.h>
#include <stdlib.h>
#include "headers/schedule.h"


// NON VISIBILI !!!

// struttura nodo
struct node_schedule {
    Data dataAppuntamento;
    Richiesta req;
    Tecnico* tec;
    struct node_schedule *left;
    struct node_schedule *right;
};

// confronta due date
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
    if(d1.ora != d2.ora){
        return (d1.ora < d2.ora) ? -1 : 1;
    }
    return 0; //se le date sono uguali ritorna zero pk tutti confronti sono falliti
}

// crea un nuovo nodo
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
// printa tutti i tecnici che hanno concluso qualcosa nella vita
static void stampaTecniciConclusi(Schedule root) {
    if (root == NULL) return;

    stampaTecniciConclusi(root->left);

    if (root->req.stato == CONCLUSA) {
        printf("- %s\n", root->tec->nome);
    }

    stampaTecniciConclusi(root->right);
}

// FUNZIONI PUBBLICHE!!!

// crea l'albero vuoto
Schedule creaAlbero(){
    return NULL;
}

//cerca se la data esiste gia'
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

// TODO [PER INTEGRAZIONE]: planIntervento si aspetta un Tecnico* (puntatore).
// Chi chiama questa funzione nel main deve passare &tecnico oppure un Tecnico* già allocato con malloc. NON passare un Tecnico per valore.
// Inserisce un intervento nell'albero
Schedule planIntervento(Schedule root, Richiesta req, Tecnico* tec, Data data) {
    if (root == NULL) return creaNodo(req, tec, data); // posto trovato!

    int risultato = confrontaDate(data, root->dataAppuntamento);

    if (risultato < 0)
        root->left  = planIntervento(root->left,  req, tec, data);

    else if (risultato > 0)
        root->right = planIntervento(root->right, req, tec, data);

    return root;
} 


// Stampa in ordine cronologico gli interventi CONCLUSI
void stampaStorico(Schedule root) {
    if (root == NULL) return;
    stampaStorico(root->left);

    if (root->req.stato == CONCLUSA) {
        printf("%02d/%02d/%d %02d:00 | Tecnico: %s | ID: %d\n",
            root->dataAppuntamento.giorno,
            root->dataAppuntamento.mese,
            root->dataAppuntamento.anno,
            root->dataAppuntamento.ora,
            root->tec->nome,
            root->req.codice);
    }

    stampaStorico(root->right);
}


// Cerca un intervento per codice (deve guardare TUTTI i nodi perché l'albero è ordinato per date non per codici)
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


// Conta interventi aperti e chiusi
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

// calcola differenza in giorni tra due date 
static int giorniTra(const char* d1, const char* d2) {
    int g1, m1, a1, g2, m2, a2;
    sscanf(d1, "%d/%d/%d", &g1, &m1, &a1);
    sscanf(d2, "%d/%d/%d", &g2, &m2, &a2);
    // approssimazione
    int tot1 = a1*365 + m1*30 + g1;
    int tot2 = a2*365 + m2*30 + g2;
    return tot2 - tot1;
}

// Calcola il tempo medio di completamento
void tempoMedioCompletamento(Schedule root, int* somma, int* count) {
    if (root == NULL) return;

    tempoMedioCompletamento(root->left, somma, count);

    if (root->req.stato == CONCLUSA) {
        int giorni = giorniTra(root->req.data, root->req.data_chiusura); //TODO dire ad assia di aggiungere la data chiusura
        if (giorni >= 0) {
            (*somma) += giorni;
            (*count)++;
        }
    }

    tempoMedioCompletamento(root->right, somma, count);
}

//chi esce più volte è il più attivo
void trovaTecnicoPiuAttivo(Schedule root) {
    if (root == NULL) {
        printf("Nessun intervento concluso.\n");
        return;
    }
    printf("Tecnici con interventi conclusi:\n");
    stampaTecniciConclusi(root);
    printf("(il tecnico che appare piu' volte e' il piu' attivo)\n");
}