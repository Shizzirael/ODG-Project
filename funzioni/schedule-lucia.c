#include <stdio.h>
#include <stdlib.h>
#include "headers/schedule.h"


// NON VISIBILI !!!

// struttura nodo
struct node_schedule {
    Data dataAppuntamento;
    Richiesta req;   //TODO vedi se assia lo chiama richiesta
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

// TODO: controlla nomi campi con ASSIA
// Stampa in ordine cronologico gli interventi CONCLUSI
void stampaStorico(Schedule root) {
    if (root == NULL) return;
    stampaStorico(root->left);

    if (root->req.stato == 3) {
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

// TODO assia
// Cerca un intervento per codice (deve guardare TUTTI i nodi perché l'albero è ordinato per date non per codici)
int cercaPerCodice(Schedule root, int codiceTarget) {
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
    if (cercaPerCodice(root->left,  codiceTarget)){
        return 1;
    }
    return cercaPerCodice(root->right, codiceTarget);
}

//TODO: confrontati con assia sulla corrispondenza dei numeri con gli stati (tipo 3 == concluso)
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
