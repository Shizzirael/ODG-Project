#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "richiesta.h"
 
//Queste funzioni traducono i valori enum in stringhe leggibili da stampare a schermo. dato che vengono usate in vari punti del codice
//è comodo averle come funzioni separate.
 
const char* statoToString(StatoRichiesta s) {
    switch (s) {
        case APERTA:         return "APERTA";
        case PIANIFICATA:    return "PIANIFICATA";
        case IN_LAVORAZIONE: return "IN LAVORAZIONE";
        case CONCLUSA:       return "CONCLUSA";
        case ANNULLATA:      return "ANNULLATA";
        default:             return "SCONOSCIUTO";
    }
}
 
const char* specializzazioneToString(Specializzazione sp) {
    switch (sp) {
        case IDRAULICO:     return "IDRAULICO";
        case ELETTRICISTA:  return "ELETTRICISTA";
        case MURATORE:      return "MURATORE";
        case ASCENSORISTA:  return "ASCENSORISTA";
        case GENERICO:      return "GENERICO";
        default:            return "SCONOSCIUTO";
    }
}
 
//Non tutti i passaggi di stato sono ammessi: una richiesta segue un ciclo di vita preciso:
//APERTA -> PIANIFICATA -> IN_LAVORAZIONE -> CONCLUSA
//In qualsiasi momento e' possibile passare ad ANNULLATA.
//Gli stati CONCLUSA e ANNULLATA sono finali: non si torna indietro.
 
int transisioneValida(StatoRichiesta attuale, StatoRichiesta nuovo) {
    switch (attuale) {
        case APERTA:         return (nuovo == PIANIFICATA    || nuovo == ANNULLATA);
        case PIANIFICATA:    return (nuovo == IN_LAVORAZIONE || nuovo == ANNULLATA);
        case IN_LAVORAZIONE: return (nuovo == CONCLUSA       || nuovo == ANNULLATA);
        case CONCLUSA:       return 0;
        case ANNULLATA:      return 0;
        default:             return 0;
    }
}
 
 
Richiesta* creaRichiesta(int codice, const char* area, Specializzazione tipologia, const char* descrizione, const char* data, int urgenza) {
    Richiesta* nuova = (Richiesta*)malloc(sizeof(Richiesta));
    if (nuova == NULL) return NULL;
 
    (*nuova).codice    = codice;
    (*nuova).tipologia = tipologia;
    (*nuova).urgenza   = urgenza;
    (*nuova).stato     = APERTA;   // ogni nuova richiesta parte sempre da APERTA
    (*nuova).next      = NULL;
 
    //strncpy viene usato al posto di strcpy per evitare buffer overflow: copiamo al massimo MAX_STR-1 caratteri e aggiungiamo sempre il terminatore '\0' manualmente.
    strncpy((*nuova).area,        area,        MAX_STR - 1);
    strncpy((*nuova).descrizione, descrizione, MAX_STR - 1);
    strncpy((*nuova).data,        data,        10);
 
    (*nuova).area[MAX_STR - 1]        = '\0';
    (*nuova).descrizione[MAX_STR - 1] = '\0';
    (*nuova).data[10]                 = '\0';
    (*nuova).data_chiusura[0] = '\0'; //viene inizializzata a stringa vuota: sara' valorizzata solo quando la richiesta raggiunge lo stato CONCLUSA.
    (*nuova).tecnico[0]       = '\0'; //il tecnico non e' noto al momento della creazione: il campo viene lasciato vuoto e sara' scritto dal modulo di pianificazione quando la richiesta viene assegnata.
 
    return nuova;
}
 
//Aggiunge un nodo in CODA alla lista collegata.
//Si sceglie la coda (e non la testa) per mantenere l'ordine cronologico di inserimento: la prima richiesta inserita rimane la prima della lista.
 
void inserisciRichiesta(Richiesta** testa, Richiesta* nuova) {
    if (testa == NULL || nuova == NULL) return; /* controllo parametri */
 
    // Caso base: lista vuota, il nuovo nodo diventa la testa
    if (*testa == NULL) {
        *testa = nuova;
        return;
    }
 
    Richiesta* temp = *testa;
    while ((*temp).next != NULL) {
        temp = (*temp).next;
    }
    //Collego il nuovo nodo in fondo
    (*temp).next = nuova;
}
 
//Modifica lo stato di una richiesta solo se la transizione e' valida
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
 
    //Se la richiesta viene chiusa, registriamo la data di chiusura. Cosa necessaria per calcolare il tempo medio di completamento nel report finale.
    if (nuovoStato == CONCLUSA) {
        printf("Inserisci la data di chiusura (GG/MM/AAAA): ");
        scanf("%10s", (*r).data_chiusura);
        (*r).data_chiusura[10] = '\0';
    }
 
    printf("Stato aggiornato con successo: '%s'.\n", statoToString((*r).stato));
    return 1;
}
 
//cerca la richiesta per codice nella lista e poi chiama aggiornaStato su di essa.
//Separa la logica di ricerca dalla logica di aggiornamento.
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
 
//Mostra all'utente solo le opzioni valide in base allo stato attuale della richiesta, cosi' non e' possibile inserire transizioni non permesse.
void menuAggiornaStato(Richiesta* testa) {
    int codice, scelta;
 
    printf("\n AGGIORNAMENTO STATO RICHIESTA \n");
    printf("Inserisci il codice della richiesta: ");
    scanf("%d", &codice);
 
    //Cerca la richiesta nella lista tramite il codice
    Richiesta* r = cercaPerCodice(testa, codice);
    if (r == NULL) {
        printf("Richiesta con codice %d non trovata.\n", codice);
        return;
    }
 
    //Mostra il dettaglio prima di chiedere la modifica
    stampaDettaglioRichiesta(r);
    printf("Stato attuale: %s\n", statoToString((*r).stato));
 
    //Mostra solo le opzioni valide
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
        else                  printf("Scelta non valida.\n");
    } else if ((*r).stato == PIANIFICATA) {
        if      (scelta == 1) aggiornaStato(r, IN_LAVORAZIONE);
        else if (scelta == 2) aggiornaStato(r, ANNULLATA);
        else                  printf("Scelta non valida.\n");
    } else if ((*r).stato == IN_LAVORAZIONE) {
        if      (scelta == 1) aggiornaStato(r, CONCLUSA);
        else if (scelta == 2) aggiornaStato(r, ANNULLATA);
        else                  printf("Scelta non valida.\n");
    }
}
 
 
//Ricerca un nodo nella lista tramite il suo codice univoco. Abbiamo scelto un approccio RICORSIVO.
// caso base 1: lista vuota -> restituisce NULL
// caso base 2: codice trovato -> restituisce il nodo
// caso ricorsivo: prosegue sul nodo successivo
 
Richiesta* cercaPerCodice(Richiesta* testa, int codice) {
    if (testa == NULL) return NULL;               //lista esaurita, non trovato
    if ((*testa).codice == codice) return testa;  //trovato: restituisce il nodo
    return cercaPerCodice((*testa).next, codice); //cerca nel resto
}
 

//cercaPerCodice viene mantenuta separata perche' e' usata internamente da altre funzioni (es. menuAggiornaStato).
void cercaEStampaPerCodice(Richiesta* testa, int codice) {
    Richiesta* r = cercaPerCodice(testa, codice);
    if (r != NULL)
        stampaDettaglioRichiesta(r);
    else
        printf("Nessuna richiesta trovata con codice %d.\n", codice);
}
 

// caso base 1: lista vuota -> termina
// caso base 2: tipologia corrisponde -> stampa il nodo e continua
// caso ricorsivo: prosegue sul nodo successivo
void cercaPerTipologia(Richiesta* testa, Specializzazione tipologia) {
    if (testa == NULL) return;                              //lista esaurita
    if ((*testa).tipologia == tipologia)                    //tipologia trovata: stampa
        stampaDettaglioRichiesta(testa);
    cercaPerTipologia((*testa).next, tipologia);            //prosegue sul resto
}
 
//Stampa tutti i campi di una singola richiesta. Se data_chiusura e' vuota (richiesta non ancora conclusa), stampa "N/A" al posto della stringa vuota.
void stampaDettaglioRichiesta(Richiesta* r) {
    if (r == NULL) return; /* niente da stampare */
    printf("\n");
    printf("Codice        : %d\n",   (*r).codice);
    printf("Area          : %s\n",   (*r).area);
    printf("Tipologia     : %s\n",   specializzazioneToString((*r).tipologia));
    printf("Descrizione   : %s\n",   (*r).descrizione);
    printf("Data apertura : %s\n",   (*r).data);
    printf("Data chiusura : %s\n",   (*r).data_chiusura[0] != '\0' ? (*r).data_chiusura : "N/A");
    printf("Tecnico       : %s\n",   (*r).tecnico[0]       != '\0' ? (*r).tecnico       : "Non assegnato"); //se il tecnico non e' ancora stato assegnato, il campo e' vuoto: stampiamo "Non assegnato" per chiarezza
    printf("Urgenza       : %d/5\n", (*r).urgenza);
    printf("Stato         : %s\n",   statoToString((*r).stato));
    printf("\n");
}
 
//Scorre tutta la lista e stampa solo le richieste che corrispondono al filtro numerico specificato:
void stampaRichiesteFiltrate(Richiesta* testa, int valore, TipoFiltro tipoFiltro) {
    Richiesta* temp = testa;
    int trovato = 0; //flag per sapere se almeno un risultato e' stato trovato
 
    printf("\n RISULTATI RICERCA (FILTRO NUMERICO) \n");
 
    while (temp != NULL) {
        int corrisponde = 0;
 
        if (tipoFiltro == FILTRO_STATO   && (int)(*temp).stato == valore) corrisponde = 1;  //filtra per valore dell'enum StatoRichiesta
        if (tipoFiltro == FILTRO_URGENZA && (*temp).urgenza     == valore) corrisponde = 1; //filtra per livello di urgenza (1-5)
 
        if (corrisponde) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
        temp = (*temp).next;
    }
 
    if (!trovato) printf("Nessuna richiesta trovata per i criteri inseriti.\n");
}
 
void stampaRichiestePerStringa(Richiesta* testa, const char* valore, TipoFiltro tipoFiltro) {
    if (valore == NULL) return;
 
    Richiesta* temp = testa;
    int trovato = 0;
 
    printf("\n RISULTATI RICERCA (FILTRO TESTUALE) \n");
 
    while (temp != NULL) {
        //strcmp == 0 significa che le due stringhe sono identiche
        if (tipoFiltro == FILTRO_AREA    && strcmp((*temp).area,    valore) == 0) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
        //FILTRO_TECNICO: confronta il campo tecnico della richiesta con il nome cercato.
        //Il campo tecnico e' scritto dal modulo di pianificazione al momento dell'assegnazione;
        //se la richiesta non ha ancora un tecnico assegnato, il campo e' vuoto e non corrisponde mai.
        if (tipoFiltro == FILTRO_TECNICO && strcmp((*temp).tecnico, valore) == 0) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
        temp = (*temp).next;
    }
 
    if (!trovato) printf("Nessuna richiesta trovata per il valore \"%s\".\n", valore);
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
 
    if (!trovato) printf("Nessuna richiesta trovata per la tipologia \"%s\".\n", specializzazioneToString(tipologia));
}
 
//Trova e stampa l'area con il maggior numero di richieste.
//Algoritmo: due cicli annidati, per ogni nodo i, il ciclo interno conta quante volte
//compare la stessa area nel resto della lista.
//Se il conteggio supera il massimo trovato finora, aggiorniamo il massimo e salviamo il nome dell'area.
//In caso di parita', viene restituita la prima area incontrata con quel conteggio massimo.
void areaPiuProblematica(Richiesta* testa) {
    if (testa == NULL) {
        printf("Nessuna richiesta presente.\n");
        return;
    }
 
    char areaMigliore[MAX_STR];
    int  maxConteggio = 0;
 
    Richiesta* i = testa;
    while (i != NULL) {
        int conteggio = 0;
        Richiesta* j = testa;
        while (j != NULL) {
            if (strcmp((*j).area, (*i).area) == 0) conteggio++;
            j = (*j).next;
        }
 
        if (conteggio > maxConteggio) {
            maxConteggio = conteggio;
            strncpy(areaMigliore, (*i).area, MAX_STR - 1);
            areaMigliore[MAX_STR - 1] = '\0';
        }
        i = (*i).next;
    }
 
    printf("\n AREA PIU' PROBLEMATICA \n");
    printf("Area: %s  (%d richieste)\n", areaMigliore, maxConteggio);
}
 
 
void liberaListaRichieste(Richiesta* testa) {
    if (testa == NULL) return;           //caso base: lista vuota
    liberaListaRichieste((*testa).next); //prima libera il resto della lista
    free(testa);                         // poi libera il nodo corrente
    //Liberare prima la coda garantisce che nessun puntatore venga perso prima della deallocazione.
}