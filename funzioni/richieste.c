#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/richieste.h"
 
 
//Queste funzioni traducono i valori enum in stringhe leggibili da stampare a schermo. Dato che vengono usate in vari punti del codice
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
        case IDRAULICO:    return "IDRAULICO";
        case ELETTRICISTA: return "ELETTRICISTA";
        case MURATORE:     return "MURATORE";
        case ASCENSORISTA: return "ASCENSORISTA";
        case GENERICO:     return "GENERICO";
        default:           return "SCONOSCIUTO";
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

// Converte una data "GG/MM/AAAA" in giorni per confronti cronologici. AGGIUNTA LUCIA
static int dataInGiorni(const char* data) {
    int g, m, a;
    if (sscanf(data, "%d/%d/%d", &g, &m, &a) != 3) return -1;
    return a * 365 + m * 30 + g;
}
 
 
Richiesta* creaRichiesta(int codice, const char* area, Specializzazione tipologia, const char* descrizione, const char* data, int urgenza) {
    Richiesta* nuova = (Richiesta*)malloc(sizeof(Richiesta));
    if (nuova == NULL) return NULL;
 
    nuova->codice    = codice;
    nuova->tipologia = tipologia;
    nuova->urgenza   = urgenza;
    nuova->stato     = APERTA;  // ogni nuova richiesta parte sempre da APERTA
    nuova->next      = NULL;
 
    //strncpy viene usato al posto di strcpy per evitare buffer overflow: copiamo al massimo MAX_STR-1 caratteri e aggiungiamo sempre il terminatore '\0' manualmente.
    strncpy(nuova->area,        area,        MAX_STR - 1);
    strncpy(nuova->descrizione, descrizione, MAX_STR - 1);
    strncpy(nuova->data,        data,        10);
 
    nuova->area[MAX_STR - 1]        = '\0';
    nuova->descrizione[MAX_STR - 1] = '\0';
    nuova->data[10]                 = '\0';
 
    nuova->data_chiusura[0] = '\0';  //viene inizializzata a stringa vuota: sara' valorizzata solo quando la richiesta raggiunge lo stato CONCLUSA.
    nuova->tecnico[0] = '\0';        //il tecnico non e' noto al momento della creazione: il campo viene lasciato vuoto e sara' scritto dal modulo
	                                 //di pianificazione quando la richiesta viene assegnata.
    return nuova;
}
 
//Aggiunge un nodo in CODA alla lista collegata.
//Si sceglie la coda (e non la testa) per mantenere l'ordine cronologico di inserimento: la prima richiesta inserita rimane la prima della lista.
void inserisciRichiesta(Richiesta** testa, Richiesta* nuova) {
    if (testa == NULL || nuova == NULL) return;
 
    if (*testa == NULL) {
        *testa = nuova;  // Caso base: lista vuota, il nuovo nodo diventa la testa
        return;
    }
 
    {
        Richiesta* temp = *testa;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nuova;  //Collego il nuovo nodo in fondo
    }
}
 
//Modifica lo stato di una richiesta solo se la transizione e' valida
int aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato) {
    if (r == NULL) {
        printf("Errore: richiesta non valida (NULL).\n");
        return 0;
    }
 
    if (!transisioneValida(r->stato, nuovoStato)) {
        printf("Errore: transizione non permessa da '%s' a '%s'.\n",
               statoToString(r->stato),
               statoToString(nuovoStato));
        return 0;
    }
 
    r->stato = nuovoStato;
 
    //Se la richiesta viene chiusa, registriamo la data di chiusura. Cosa necessaria per calcolare il tempo medio di completamento nel report finale.
    if (nuovoStato == CONCLUSA) {
    int g, m, a;
    while (1) {
        printf("Inserisci la data di chiusura (GG/MM/AAAA): ");
        //flush preventivo per eliminare il '\n' residuo lasciato da un scanf("%d") chiamato in precedenza nel menu. 
        while (getchar() != '\n');
        scanf("%10s", r->data_chiusura);
        while (getchar() != '\n');
        r->data_chiusura[10] = '\0';
        if (sscanf(r->data_chiusura, "%d/%d/%d", &g, &m, &a) != 3 ||
            g < 1 || g > 31 || m < 1 || m > 12 || a < 2000 || a > 2100) {
            printf("  Formato non valido. Usa GG/MM/AAAA (es. 15/05/2026).\n");
            continue;
        }
        if (dataInGiorni(r->data_chiusura) < dataInGiorni(r->data)) {
            printf("  Errore: la data di chiusura non puo' essere antecedente alla data di apertura (%s).\n", r->data);
            continue;
        }
        break;
    }
}
 
    printf("Stato aggiornato con successo: '%s'.\n", statoToString(r->stato));
    return 1;
}
 
//cerca la richiesta per codice nella lista e poi chiama aggiornaStato su di essa.
//Separa la logica di ricerca dalla logica di aggiornamento.
int aggiornaStatoDaCodice(Richiesta* testa, int codice, StatoRichiesta nuovoStato) {
    Richiesta* temp = testa;
    while (temp != NULL) {
        if (temp->codice == codice) {
            return aggiornaStato(temp, nuovoStato);
        }
        temp = temp->next;
    }
 
    printf("Errore: nessuna richiesta trovata con codice %d.\n", codice);
    return 0;
}
 
//Mostra all'utente solo le opzioni valide in base allo stato attuale della richiesta, cosi' non e' possibile inserire transizioni non permesse.
void menuAggiornaStato(Richiesta* testa) {
    int codice, scelta;
    Richiesta* r;
 
    printf("\n AGGIORNAMENTO STATO RICHIESTA \n");
    printf("Inserisci il codice della richiesta: ");
    scanf("%d", &codice);
    while (getchar() != '\n');  
 
    //Cerca la richiesta nella lista tramite il codice
    r = cercaPerCodice(testa, codice);
    if (r == NULL) {
        printf("Richiesta con codice %d non trovata.\n", codice);
        return;
    }
 
    //Mostra il dettaglio prima di chiedere la modifica
    stampaDettaglioRichiesta(r);
    printf("Stato attuale: %s\n", statoToString(r->stato));
 
    //Mostra solo le opzioni valide
    printf("\nSeleziona il nuovo stato:\n");
    if (r->stato == APERTA) {
        printf("  1. PIANIFICATA\n");
        printf("  2. ANNULLATA\n");
    } else if (r->stato == PIANIFICATA) {
        printf("  1. IN LAVORAZIONE\n");
        printf("  2. ANNULLATA\n");
    } else if (r->stato == IN_LAVORAZIONE) {
        printf("  1. CONCLUSA\n");
        printf("  2. ANNULLATA\n");
    } else {
        printf("Questa richiesta e' in stato finale (%s), non modificabile.\n",
               statoToString(r->stato));
        return;
    }
 
    printf("Scelta: ");
    scanf("%d", &scelta);
    while (getchar() != '\n');  
 
    if (r->stato == APERTA) {
        if      (scelta == 1) aggiornaStato(r, PIANIFICATA);
        else if (scelta == 2) aggiornaStato(r, ANNULLATA);
        else                  printf("Scelta non valida.\n");
    } else if (r->stato == PIANIFICATA) {
        if      (scelta == 1) aggiornaStato(r, IN_LAVORAZIONE);
        else if (scelta == 2) aggiornaStato(r, ANNULLATA);
        else                  printf("Scelta non valida.\n");
    } else if (r->stato == IN_LAVORAZIONE) {
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
    if (testa == NULL)           return NULL;    //lista esaurita, non trovato
    if (testa->codice == codice) return testa;   //trovato: restituisce il nodo
    return cercaPerCodice(testa->next, codice);  //cerca nel resto
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
static void cercaPerTipologiaHelper(Richiesta* testa, Specializzazione tipologia,
                                    int* trovato) {
    if (testa == NULL) return;                    //lista esaurita
    if (testa->tipologia == tipologia) {          //tipologia trovata: stampa
        stampaDettaglioRichiesta(testa);
        *trovato = 1;
    }
    cercaPerTipologiaHelper(testa->next, tipologia, trovato);  //prosegue sul resto
}
 
void cercaPerTipologia(Richiesta* testa, Specializzazione tipologia) {
    int trovato = 0;
    printf("\n RISULTATI RICERCA (FILTRO TIPOLOGIA) \n");
    cercaPerTipologiaHelper(testa, tipologia, &trovato);
    if (!trovato)
        printf("Nessuna richiesta trovata per la tipologia \"%s\".\n",
               specializzazioneToString(tipologia));
}
 
 
//Stampa tutti i campi di una singola richiesta. Se data_chiusura e' vuota (richiesta non ancora conclusa), stampa "N/A" al posto della stringa vuota.
void stampaDettaglioRichiesta(Richiesta* r) {
    if (r == NULL) return;
    printf("\n");
    printf("------------------------------\n");
    printf("Codice        : %d\n",   r->codice);
    printf("Area          : %s\n",   r->area);
    printf("Tipologia     : %s\n",   specializzazioneToString(r->tipologia));
    printf("Descrizione   : %s\n",   r->descrizione);
    printf("Data apertura : %s\n",   r->data);
    printf("Data chiusura : %s\n",   r->data_chiusura[0] != '\0' ? r->data_chiusura : "N/A");
    printf("Tecnico       : %s\n",   r->tecnico[0] != '\0' ? r->tecnico : "Non assegnato");  //se il tecnico non e' ancora stato assegnato, il campo e' vuoto: stampiamo "Non assegnato" per chiarezza
    printf("Urgenza       : %d/5\n", r->urgenza);
    printf("Stato         : %s\n",   statoToString(r->stato));
    printf("------------------------------\n");
    printf("\n");
}
 
//Scorre tutta la lista e stampa solo le richieste che corrispondono al filtro numerico specificato
void stampaRichiesteFiltrate(Richiesta* testa, int valore, TipoFiltro tipoFiltro) {
    Richiesta* temp = testa;
    int trovato = 0;  //flag per sapere se almeno un risultato e' stato trovato
 
    printf("\n RISULTATI RICERCA (FILTRO NUMERICO) \n");
 
    while (temp != NULL) {
        int corrisponde = 0;
 
        if (tipoFiltro == FILTRO_STATO   && (int)temp->stato == valore) corrisponde = 1;  //filtra per valore dell'enum StatoRichiesta
        if (tipoFiltro == FILTRO_URGENZA && temp->urgenza    == valore) corrisponde = 1;  //filtra per livello di urgenza (1-5)
 
        if (corrisponde) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
        temp = temp->next;
    }
 
    if (!trovato) printf("Nessuna richiesta trovata per i criteri inseriti.\n");
}
 
void stampaRichiestePerStringa(Richiesta* testa, const char* valore, TipoFiltro tipoFiltro) {
    Richiesta* temp;
    int trovato = 0;
 
    if (valore == NULL) return;
 
    printf("\n RISULTATI RICERCA (FILTRO TESTUALE) \n");
 
    for (temp = testa; temp != NULL; temp = temp->next) {
        if (tipoFiltro == FILTRO_AREA && strcmp(temp->area, valore) == 0) {  //strcmp == 0 significa che le due stringhe sono identiche
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
        //FILTRO_TECNICO: confronta il campo tecnico della richiesta con il nome cercato.
        //Il campo tecnico e' scritto dal modulo di pianificazione al momento dell'assegnazione;
        //se la richiesta non ha ancora un tecnico assegnato, il campo e' vuoto e non corrisponde mai.
        if (tipoFiltro == FILTRO_TECNICO && strcmp(temp->tecnico, valore) == 0) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
    }
 
    if (!trovato) printf("Nessuna richiesta trovata per il valore \"%s\".\n", valore);
}
 
void stampaRichiestePerTipologia(Richiesta* testa, Specializzazione tipologia) {
    Richiesta* temp;
    int trovato = 0;
 
    printf("\n RISULTATI RICERCA (FILTRO TIPOLOGIA) \n");
 
    for (temp = testa; temp != NULL; temp = temp->next) {
        if (temp->tipologia == tipologia) {
            stampaDettaglioRichiesta(temp);
            trovato = 1;
        }
    }
 
    if (!trovato)
        printf("Nessuna richiesta trovata per la tipologia \"%s\".\n",
               specializzazioneToString(tipologia));
}
 
void stampaRichiestePerTecnico(Richiesta* testa, const char* nomeTecnico) {
    stampaRichiestePerStringa(testa, nomeTecnico, FILTRO_TECNICO);
}
 
/*Trova e stampa l'area con il maggior numero di richieste.
Algoritmo: due cicli annidati, per ogni nodo i, il ciclo interno conta quante volte
compare la stessa area nel resto della lista.
Se il conteggio supera il massimo trovato finora, aggiorniamo il massimo e salviamo il nome dell'area.
In caso di parita', viene restituita la prima area incontrata con quel conteggio massimo. */
void areaPiuProblematica(Richiesta* testa) {
    char areaMigliore[MAX_STR];
    int  maxConteggio = 0;
    Richiesta* i;
 
    if (testa == NULL) {
        printf("Nessuna richiesta presente.\n");
        return;
    }
 
    for (i = testa; i != NULL; i = i->next) {
        int conteggio = 0;
        Richiesta* j;
        for (j = testa; j != NULL; j = j->next) {
            if (strcmp(j->area, i->area) == 0) conteggio++;
        }
        if (conteggio > maxConteggio) {
            maxConteggio = conteggio;
            strncpy(areaMigliore, i->area, MAX_STR - 1);
            areaMigliore[MAX_STR - 1] = '\0';
        }
    }
 
    printf("\n AREA PIU' PROBLEMATICA \n");
    printf("Area: %s  (%d richieste)\n", areaMigliore, maxConteggio);
}
 
 
void liberaListaRichieste(Richiesta* testa) {
    if (testa == NULL) return;              //caso base: lista vuota
    liberaListaRichieste(testa->next);      //prima libera il resto della lista
    free(testa);                            //poi libera il nodo corrente
	//Si libera prima ricorsivamente il resto della lista e poi il nodo corrente (ordine post-order): 
	//se si liberasse testa prima della chiamata ricorsiva, testa->next diventerebbe invalido
    //e non sarebbe piu' possibile raggiungere i nodi successivi. 
<<<<<<< HEAD:funzioni/richieste-assia.c
} 
 //ADT PRIORITY QUEUE -- Max-Heap per urgenza
=======
}
 
>>>>>>> c520d1f14b61dc6700527b53e74df3efd7792ddb:funzioni/richieste.c
 
//Scambia due puntatori nell'array heap. 
static void swap(Richiesta** a, Richiesta** b) {
    Richiesta* tmp = *a;
    *a = *b;
    *b = tmp;
}
 
//upheap: risale dall'indice k verso la radice finche' l'invariante e' VIOLATO.
static void sali(PriorityQueue* pq, int k) {
    while (k > 1 && pq->heap[k / 2]->urgenza < pq->heap[k]->urgenza) {
        swap(&pq->heap[k / 2], &pq->heap[k]);
        k = k / 2;
    }
}
 
//downheap: scende dall'indice k verso le foglie finche' l'invariante e' VIOLATO.
static void scendi(PriorityQueue* pq, int k) {
    int n = pq->size;
    while (2 * k <= n) {
        int j = 2 * k;             
        if (j < n && pq->heap[j]->urgenza < pq->heap[j + 1]->urgenza)
            j++;                   
        if (pq->heap[k]->urgenza >= pq->heap[j]->urgenza)
            break;                
        swap(&pq->heap[k], &pq->heap[j]);
        k = j;
    }
}
 
// Inizializza la coda a priorita' (heap vuoto). 
void initPQ(PriorityQueue* pq) {
    if (pq == NULL) return;
    pq->size    = 0;
    pq->heap[0] = NULL;  // heap[0] non viene usato (convenzione 1-based): lo azzeriamo per sicurezza
}
 
int emptyPQ(const PriorityQueue* pq) {
    if (pq == NULL) return 1;
    return pq->size == 0;
}
 
int fullPQ(const PriorityQueue* pq) {
    if (pq == NULL) return 1;
    return pq->size >= MAX_HEAP;
}
 
// Inserimento: aggiunge r come ultima foglia (heap[size+1]) e risale con upheap.
int insertPQ(PriorityQueue* pq, Richiesta* r) {
    if (pq == NULL || r == NULL) return 0;
    if (fullPQ(pq)) {
        printf("Errore: coda a priorita' piena, impossibile inserire la richiesta %d.\n",
               r->codice);
        return 0;
    }
    pq->heap[++pq->size] = r;
    sali(pq, pq->size);
    return 1;
}
 
// Estrazione massimo: salva la radice (heap[1]), sposta l'ultima foglia in radice, decrementa size e ripristina l'invariante con downheap.
Richiesta* deleteMax(PriorityQueue* pq) {
    Richiesta* max;
    if (pq == NULL || emptyPQ(pq)) {
        printf("Errore: coda a priorita' vuota.\n");
        return NULL;
    }
    max                = pq->heap[1];
    pq->heap[1]        = pq->heap[pq->size];
    pq->heap[pq->size] = NULL;
    pq->size--;
    if (!emptyPQ(pq))
        scendi(pq, 1);
    return max;
}
 
// Restituisce la radice senza modificare l'heap. 
Richiesta* peekMax(const PriorityQueue* pq) {
    if (pq == NULL || emptyPQ(pq)) return NULL;
    return pq->heap[1];
}
 
/*Stampa tutte le richieste in ordine decrescente di urgenza operando su una copia temporanea dell'heap, 
preservando intatta la struttura originale. La copia e' superficiale (shallow copy):
i puntatori nell'array heap puntano agli stessi oggetti Richiesta dell'originale. Questo e'
corretto perche' deleteMax sulla copia riorganizza solo i puntatori nell'array senza liberare memoria. */
void stampaHeap(const PriorityQueue* pq) {
    PriorityQueue copia;
    Richiesta* r;
 
    if (pq == NULL || emptyPQ(pq)) {
        printf("Nessuna richiesta in attesa di elaborazione.\n");
        return;
    }
 
    copia = *pq;  // copia locale per non modificare l'originale
 
    printf("\n--- RICHIESTE ORDINATE PER URGENZA (DECRESCENTE) ---\n");
    while (!emptyPQ(&copia)) {
        r = deleteMax(&copia);
        printf("[Urgenza %d] Codice %d - %s\n", r->urgenza, r->codice, r->area);
    }
}
 