#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
 
static const char* specializzazioneToString(Specializzazione sp) {
    switch (sp) {
        case IDRAULICO:     return "IDRAULICO";
        case ELETTRICISTA:  return "ELETTRICISTA";
        case MURATORE:      return "MURATORE";
        case ASCENSORISTA:  return "ASCENSORISTA";
        case GENERICO:      return "GENERICO";
        default:            return "SCONOSCIUTO";
    }
}
 

static int transisioneValida(StatoRichiesta attuale, StatoRichiesta nuovo) {
    switch (attuale) {
        case APERTA:         return (nuovo == PIANIFICATA    || nuovo == ANNULLATA);
        case PIANIFICATA:    return (nuovo == IN_LAVORAZIONE || nuovo == ANNULLATA);
        case IN_LAVORAZIONE: return (nuovo == CONCLUSA       || nuovo == ANNULLATA);
        case CONCLUSA:       return 0;
        case ANNULLATA:      return 0;
        default:             return 0;
    }
}
 

Richiesta* creaRichiesta(int codice, const char* area, Specializzazione tipologia,
                         const char* descrizione, const char* data, int urgenza) {
    Richiesta* nuova = (Richiesta*)malloc(sizeof(Richiesta));
    if (nuova == NULL) return NULL;
 
    (*nuova).codice     = codice;
    (*nuova).tipologia  = tipologia;   // enum, assegnazione diretta //
    (*nuova).urgenza    = urgenza;
    (*nuova).stato      = APERTA;
    (*nuova).next       = NULL;
 
    strncpy((*nuova).area,        area,        MAX_STR - 1);
    strncpy((*nuova).descrizione, descrizione, MAX_STR - 1);
    strncpy((*nuova).data,        data,        10);
 
    (*nuova).area[MAX_STR - 1]        = '\0';
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

int aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato) {
    if (r == NULL) {
        printf("Errore: richiesta non valida (NULL).\n");
        return 0;
    }
 
    if (!transisioneValida((*r).stato, nuovoStato)) {
        printf("Errore: transizione non permessa da '%s' a '%s'.\n",
               statoToString((*r).stato),
               statoToString(nuovoStato));
        return 0;
    }
 
    (*r).stato = nuovoStato;

    if (nuovoStato == CONCLUSA) { //modifica fatta da lucia ASSIA CIAO MI SERVIVA PER LA QUESTIONE DEL TEMPO MEDIO SALVA SEMPLICEMENTE ANCHE LA DATA CHIUSURA
        time_t ora = time(NULL);
        struct tm* t = localtime(&ora);
        strftime((*r).data_chiusura, 11, "%d/%m/%Y", t);
    }

    printf("Stato aggiornato con successo: '%s'.\n", statoToString((*r).stato));
    return 1;
}
 

int aggiornaStatoDaCodice(Richiesta* testa, int codice, StatoRichiesta nuovoStato) {
    Richiesta* temp = testa;
    while (temp != NULL) {
        if ((*temp).codice == codice) {
            return aggiornaStato(temp, nuovoStato);
        }
        temp = (*temp).next;
    }
    printf("Errore: nessuna richiesta trovata con codice %d.\n", codice);
    return 0;
}
 

void menuAggiornaStato(Richiesta* testa) {
    int codice, scelta;
 
    printf("\n AGGIORNAMENTO STATO RICHIESTA \n");
    printf("Inserisci il codice della richiesta: ");
    scanf("%d", &codice);
 
    Richiesta* r = cercaPerCodice(testa, codice);
    if (r == NULL) {
        printf("Richiesta con codice %d non trovata.\n", codice);
        return;
    }
 
    printf("\nRichiesta trovata:\n");
    stampaDettaglioRichiesta(r);
    printf("Stato attuale: %s\n", statoToString((*r).stato));
 
    printf("\nSeleziona il nuovo stato:\n");
    if ((*r).stato == APERTA) {
        printf("  1. PIANIFICATA\n");
        printf("  2. ANNULLATA\n");
    } else if ((*r).stato == PIANIFICATA) {
        printf("  1. IN LAVORAZIONE\n");
        printf("  2. ANNULLATA\n");
    } else if ((*r).stato == IN_LAVORAZIONE) {
        printf("  1. CONCLUSA\n");
        printf("  2. ANNULLATA\n");
    } else {
        printf("Questa richiesta e' in stato finale (%s), non modificabile.\n",
               statoToString((*r).stato));
        return;
    }
 
    printf("Scelta: ");
    scanf("%d", &scelta);
 
    if ((*r).stato == APERTA) {
        if      (scelta == 1) aggiornaStato(r, PIANIFICATA);
        else if (scelta == 2) aggiornaStato(r, ANNULLATA);
        else printf("Scelta non valida.\n");
    } else if ((*r).stato == PIANIFICATA) {
        if      (scelta == 1) aggiornaStato(r, IN_LAVORAZIONE);
        else if (scelta == 2) aggiornaStato(r, ANNULLATA);
        else printf("Scelta non valida.\n");
    } else if ((*r).stato == IN_LAVORAZIONE) {
        if      (scelta == 1) aggiornaStato(r, CONCLUSA);
        else if (scelta == 2) aggiornaStato(r, ANNULLATA);
        else printf("Scelta non valida.\n");
    }
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
    printf("Tipologia  : %s\n",   specializzazioneToString((*r).tipologia));
    printf("Descrizione: %s\n",   (*r).descrizione);
    printf("Data       : %s\n",   (*r).data);
    printf("Urgenza    : %d/5\n", (*r).urgenza);
    printf("Stato      : %s\n",   statoToString((*r).stato));
    printf("\n");
}
 

void stampaRichiesteFiltrate(Richiesta* testa, int valore, int tipoFiltro) {
    Richiesta* temp = testa;
    int trovato = 0;
 
    printf("\n RISULTATI RICERCA (FILTRO NUMERICO) \n");
 
    while (temp != NULL) {
        int corrisponde = 0;
 
        if (tipoFiltro == FILTRO_STATO && (int)(*temp).stato == valore) {
            corrisponde = 1;
        } else if (tipoFiltro == FILTRO_URGENZA && (*temp).urgenza == valore) {
            corrisponde = 1;
        }
 
        if (corrisponde) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
        temp = (*temp).next;
    }
 
    if (!trovato) {
        printf("Nessuna richiesta trovata per i criteri inseriti.\n");
    }
}
 

void stampaRichiestePerStringa(Richiesta* testa, const char* valore, int tipoFiltro) {
    if (valore == NULL) return;
 
    Richiesta* temp = testa;
    int trovato = 0;
 
    printf("\n RISULTATI RICERCA (FILTRO TESTUALE) \n");
 
    while (temp != NULL) {
        int corrisponde = 0;
 
        if (tipoFiltro == FILTRO_AREA && strcmp((*temp).area, valore) == 0) {
            corrisponde = 1;
        }
 
        if (corrisponde) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
        temp = (*temp).next;
    }
 
    if (!trovato) {
        printf("Nessuna richiesta trovata per il valore \"%s\".\n", valore);
    }
}
 
void stampaRichiestePerTipologia(Richiesta* testa, Specializzazione tipologia) {
    Richiesta* temp = testa;
    int trovato = 0;
 
    printf("\n RISULTATI RICERCA (FILTRO TIPOLOGIA) \n");
 
    while (temp != NULL) {
        if ((*temp).tipologia == tipologia) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
        temp = (*temp).next;
    }
 
    if (!trovato) {
        printf("Nessuna richiesta trovata per la tipologia \"%s\".\n",
               specializzazioneToString(tipologia));
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
 
// CIAO ASSIA la prof vuole anche le aree piu problematiche
// la funzione scorre la lista e stampa tutte le aree con conteggio
void areaPiuProblematica(Richiesta* testa) {
    Richiesta* curr = testa;
    char aree[100][MAX_STR];
    int conteggi[100] = {0};
    int n = 0;

    while (curr != NULL) {
        int trovata = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(aree[i], curr->area) == 0) {
                conteggi[i]++;
                trovata = 1;
                break;
            }
        }
        if (!trovata && n < 100) {
            strcpy(aree[n], curr->area);
            conteggi[n] = 1;
            n++;
        }
        curr = curr->next;
    }

    printf("\n--- AREE CON PIU' PROBLEMI ---\n");
    for (int i = 0; i < n; i++) {
        printf("%s: %d richieste\n", aree[i], conteggi[i]);
    }
}