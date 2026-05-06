#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/richieste.h"

static const char* statoToString(StatoRichiesta s) {
    switch (s) {
        case APERTA:         return "APERTA";
        case PIANIFICATA:    return "PIANIFICATA";
        case IN_LAVORAZIONE: return "IN LAVORAZIONE";
        case CONCLUSA:       return "CONCLUSA";
        case ANNULLATA:      return "ANNULLATA";
        default:             return "SCONOSCIUTO";
    }
}
 
Richiesta* creaRichiesta(int codice, const char* area, const char* tipologia,
                         const char* descrizione, const char* data, int urgenza) {
    Richiesta* nuova = (Richiesta*)malloc(sizeof(Richiesta));
    if (nuova == NULL) return NULL;
 
    (*nuova).codice  = codice;
    (*nuova).urgenza = urgenza;
    (*nuova).stato   = APERTA;
    (*nuova).next    = NULL;
 
    strncpy((*nuova).area,        area,        MAX_STR - 1);
    strncpy((*nuova).tipologia,   tipologia,   MAX_STR - 1);
    strncpy((*nuova).descrizione, descrizione, MAX_STR - 1);
    strncpy((*nuova).data,        data,        10);
 
    (*nuova).area[MAX_STR - 1]        = '\0';
    (*nuova).tipologia[MAX_STR - 1]   = '\0';
    (*nuova).descrizione[MAX_STR - 1] = '\0';
    (*nuova).data[10]                 = '\0';
 
    return nuova;
}
 
void inserisciRichiesta(Richiesta** testa, Richiesta* nuova) {
    if (testa == NULL || nuova == NULL) return;
 
    if (*testa == NULL) {
        *testa = nuova;
        return;
    }
 
    Richiesta* temp = *testa;
    while ((*temp).next != NULL) {
        temp = (*temp).next;
    }
    (*temp).next = nuova;
}
 
void aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato) {
    if (r == NULL) return;
    (*r).stato = nuovoStato;
}
 
Richiesta* cercaPerCodice(Richiesta* testa, int codice) {
    Richiesta* temp = testa;
    while (temp != NULL) {
        if ((*temp).codice == codice) return temp;
        temp = (*temp).next;
    }
    return NULL;
}
 
void stampaDettaglioRichiesta(Richiesta* r) {
    if (r == NULL) return;
    printf("\n");
    printf("Codice     : %d\n",   (*r).codice);
    printf("Area       : %s\n",   (*r).area);
    printf("Tipologia  : %s\n",   (*r).tipologia);
    printf("Descrizione: %s\n",   (*r).descrizione);
    printf("Data       : %s\n",   (*r).data);
    printf("Urgenza    : %d/5\n", (*r).urgenza);
    printf("Stato      : %s\n",   statoToString((*r).stato));
    printf("\n");
}
 
void stampaRichiesteFiltrate(Richiesta* testa, int filtro, int tipoFiltro) {
    Richiesta* temp = testa;
    int trovata = 0;
 
    while (temp != NULL) {
        if (tipoFiltro == FILTRO_STATO && (*temp).stato == (StatoRichiesta)filtro) {
            stampaDettaglioRichiesta(temp);
            trovata = 1;
        } else if (tipoFiltro == FILTRO_URGENZA && (*temp).urgenza == filtro) {
            stampaDettaglioRichiesta(temp);
            trovata = 1;
        }
        temp = (*temp).next;
    }
 
    if (!trovata) {
        printf("Nessuna richiesta trovata.\n");
    }
}
 
void stampaRichiestePerStringa(Richiesta* testa, const char* valore, int tipoFiltro) {
    if (valore == NULL) return;
 
    Richiesta* temp = testa;
    int trovata = 0;
 
    while (temp != NULL) {
        if (tipoFiltro == FILTRO_TIPOLOGIA && strcmp((*temp).tipologia, valore) == 0) {
            stampaDettaglioRichiesta(temp);
            trovata = 1;
        } else if (tipoFiltro == FILTRO_AREA && strcmp((*temp).area, valore) == 0) {
            stampaDettaglioRichiesta(temp);
            trovata = 1;
        }
        temp = (*temp).next;
    }
 
    if (!trovata) {
        printf("Nessuna richiesta trovata per il valore \"%s\".\n", valore);
    }
}

void liberaListaRichieste(Richiesta* testa) {
    Richiesta* temp;
    while (testa != NULL) {
        temp  = testa;
        testa = (*testa).next;
        free(temp);
    }
}