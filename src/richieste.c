#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/richieste.h"
#include "../headers/utile.h"
 
/*
Converte un valore enum StatoRichiesta nella stringa corrispondente.
Utilizzata da stampaDettaglioRichiesta e da altre funzioni che producono output su stdout.
Parametri:
- s: il valore enum da convertire
Ritorna:
- la stringa corrispondente allo stato, o "SCONOSCIUTO" se il valore non e' riconosciuto.
*/
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

/*
Converte un valore enum Specializzazione nella stringa corrispondente.
Utilizzata da stampaDettaglioRichiesta e da cercaPerTipologia per produrre output leggibile.
Parametri:
- sp: il valore enum da convertire
Ritorna:
- la stringa corrispondente alla specializzazione, o "SCONOSCIUTO" se il valore non e' riconosciuto.
*/
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


/*
Verifica se una transizione di stato e' ammessa secondo il ciclo di vita della richiesta.
Utilizzata da aggiornaStato prima di applicare qualsiasi modifica.
Parametri:
- attuale: lo stato corrente della richiesta
- nuovo:   lo stato verso cui si vuole transitare
Ritorna:
- 1 se la transizione e' permessa, 0 altrimenti.
*/
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
/*
APERTA -> PIANIFICATA -> IN_LAVORAZIONE -> CONCLUSA
In qualsiasi momento e' possibile passare ad ANNULLATA.
Gli stati CONCLUSA e ANNULLATA sono finali: non si torna indietro*/

/*
Converte una data in formato "GG/MM/AAAA" in un intero che rappresenta i giorni totali.
Utilizzata da menuAggiornaStato per verificare che la data di chiusura non sia antecedente
alla data di apertura della richiesta.
Parametri:
- data: stringa della data nel formato GG/MM/AAAA (non NULL)
Ritorna:
- il numero di giorni calcolato come a*365 + m*30 + g, oppure -1 se il formato non e' valido.
*/
static int dataInGiorni(const char* data) {
    int g, m, a;
    if (sscanf(data, "%d/%d/%d", &g, &m, &a) != 3) return -1;
    return a * 365 + m * 30 + g;
}

/*
Alloca e inizializza una nuova richiesta con i dati forniti.
Utilizzata dal modulo di inserimento ogni volta che l'utente registra un nuovo intervento.
I campi tecnico e data_chiusura vengono lasciati vuoti.
Parametri:
- codice:      identificativo univoco della richiesta
- area:        zona dove si trova il problema (non NULL)
- tipologia:   specializzazione richiesta per l'intervento
- descrizione: testo descrittivo del problema (non NULL)
- data:        data di apertura nel formato GG/MM/AAAA (non NULL)
- urgenza:     livello di urgenza compreso tra 1 e 5
Ritorna:
- puntatore alla nuova Richiesta allocata con stato=APERTA e next=NULL, oppure NULL se malloc fallisce.
*/
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

/*
Inserisce una nuova richiesta in fondo alla lista collegata.
Utilizzata ogni volta che una nuova richiesta viene creata e deve essere aggiunta allo storico.
Parametri:
- testa: puntatore al puntatore della testa della lista (non NULL)
- nuova: puntatore alla richiesta da inserire (non NULL)
Ritorna:
- void
*/
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

/*
Aggiorna lo stato di una richiesta verificando prima che la transizione sia valida tramite transisioneValida.
Utilizzata da aggiornaStatoDaCodice e da menuAggiornaStato.
Parametri:
- r:          puntatore alla richiesta da aggiornare (non NULL)
- nuovoStato: il nuovo stato da assegnare
Ritorna:
- 1 se lo stato e' stato aggiornato con successo, 0 se la transizione non e' permessa o r e' NULL.
*/
int aggiornaStato(Richiesta* r, StatoRichiesta nuovoStato) {
    if (r == NULL) {
        printf("Errore: richiesta non valida (NULL).\n");
        return 0;
    }
 
    if (!transisioneValida(r->stato, nuovoStato)) {
               statoToString(r->stato),
               statoToString(nuovoStato);
        return 0;
    }
 
    r->stato = nuovoStato;
 
    return 1;
}

/*
Cerca una richiesta per codice nella lista e ne aggiorna lo stato.
Utilizzata quando si conosce il codice della richiesta e si vuole aggiornarne lo stato senza
passare per il menu interattivo.
Parametri:
- testa:      puntatore alla testa della lista (puo' essere NULL)
- codice:     codice univoco della richiesta da cercare
- nuovoStato: il nuovo stato da assegnare
Ritorna:
- 1 se la richiesta e' stata trovata e lo stato aggiornato, 0 altrimenti.
*/
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

/*
Interfaccia da terminale per aggiornare lo stato di una richiesta tramite input utente.
Chiede il codice della richiesta, mostra solo le transizioni valide per lo stato attuale
e, nel caso di chiusura, richiede e valida la data di chiusura.
Parametri:
- testa: puntatore alla testa della lista (puo' essere NULL)
Ritorna:
- void
*/
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
       if (scelta == 1) {
            char dataChiusura[11];
            int g, m, a;
            while (1) {
                leggiData(dataChiusura);
                if (sscanf(dataChiusura, "%d/%d/%d", &g, &m, &a) == 3 &&
                    dataInGiorni(dataChiusura) >= dataInGiorni(r->data))
                    break;
                printf(" Data antecedente all'apertura (%s). Riprova.\n", r->data);
            }
            strncpy(r->data_chiusura, dataChiusura, 10);
            r->data_chiusura[10] = '\0';
            aggiornaStato(r, CONCLUSA);
        }
        else if (scelta == 2) aggiornaStato(r, ANNULLATA);
        else                  printf("Scelta non valida.\n");
    }
}


/*
Ricerca ricorsiva di una richiesta nella lista tramite codice univoco.
Utilizzata internamente da menuAggiornaStato, cercaEStampaPerCodice e aggiornaStatoDaCodice.
Parametri:
- testa:  puntatore alla testa della lista (puo' essere NULL)
- codice: codice univoco della richiesta da cercare
Ritorna:
- puntatore al nodo trovato, oppure NULL se la lista e' esaurita senza trovare il codice.
*/
Richiesta* cercaPerCodice(Richiesta* testa, int codice) {
    if (testa == NULL)           return NULL;    //lista esaurita, non trovato
    if (testa->codice == codice) return testa;   //trovato: restituisce il nodo
    return cercaPerCodice(testa->next, codice);  //cerca nel resto
}

//cercaPerCodice viene mantenuta separata perche' e' usata internamente da altre funzioni (es. menuAggiornaStato).

/*
Cerca una richiesta per codice e ne stampa il dettaglio su stdout, oppure un messaggio di errore.
Utilizzata dal menu principale quando l'utente vuole consultare una richiesta specifica.
Parametri:
- testa:  puntatore alla testa della lista (puo' essere NULL)
- codice: codice univoco della richiesta da cercare
Ritorna:
- void
*/
void cercaEStampaPerCodice(Richiesta* testa, int codice) {
    Richiesta* r = cercaPerCodice(testa, codice);
    if (r != NULL)
        stampaDettaglioRichiesta(r);
    else
        printf("Nessuna richiesta trovata con codice %d.\n", codice);
}


/*
Funzione helper ricorsiva per cercaPerTipologia.
Scorre la lista stampando ogni nodo che corrisponde alla tipologia cercata e aggiorna il flag trovato.
Parametri:
- testa:     puntatore alla testa della lista (puo' essere NULL)
- tipologia: specializzazione da ricercare
- trovato:   puntatore al flag che viene impostato a 1 se almeno un risultato e' trovato
Ritorna:
- void
*/
static void cercaPerTipologiaHelper(Richiesta* testa, Specializzazione tipologia,
                                    int* trovato) {
    if (testa == NULL) return;                    //lista esaurita
    if (testa->tipologia == tipologia) {          //tipologia trovata: stampa
        stampaDettaglioRichiesta(testa);
        *trovato = 1;
    }
    cercaPerTipologiaHelper(testa->next, tipologia, trovato);  //prosegue sul resto
}

/*
Stampa tutte le richieste con la tipologia indicata, usando un approccio ricorsivo tramite helper.
Utilizzata dal menu di ricerca quando l'utente filtra per specializzazione.
Parametri:
- testa:     puntatore alla testa della lista (puo' essere NULL)
- tipologia: specializzazione da ricercare
Ritorna:
- void
*/
void cercaPerTipologia(Richiesta* testa, Specializzazione tipologia) {
    int trovato = 0;
    printf("\n RISULTATI RICERCA (FILTRO TIPOLOGIA) \n");
    cercaPerTipologiaHelper(testa, tipologia, &trovato);
    if (!trovato)
        printf("Nessuna richiesta trovata per la tipologia \"%s\".\n",
               specializzazioneToString(tipologia));
}

/*
Stampa tutti i campi di una singola richiesta in formato leggibile su stdout.
Utilizzata da tutte le funzioni di ricerca e visualizzazione per produrre output uniforme.
I campi non ancora valorizzati (tecnico, data_chiusura) vengono stampati come "Non assegnato" e "N/A".
Parametri:
- r: puntatore alla richiesta da stampare (puo' essere NULL, in tal caso non stampa nulla)
Ritorna:
- void
*/
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

/*
Scorre la lista e stampa le richieste che corrispondono al filtro numerico specificato.
Utilizzata dal menu di ricerca per filtrare per stato o per livello di urgenza.
Parametri:
- testa:      puntatore alla testa della lista (puo' essere NULL)
- valore:     valore numerico da confrontare (valore enum dello stato oppure livello urgenza 1-5)
- tipoFiltro: FILTRO_STATO oppure FILTRO_URGENZA
Ritorna:
- void
*/
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

/*
Scorre la lista e stampa le richieste che corrispondono al filtro testuale specificato.
Utilizzata da stampaRichiestePerTecnico e dal menu di ricerca per filtrare per area o per tecnico.
Il confronto e' case-sensitive: "Piano 3" e "piano 3" sono considerati valori diversi.
Parametri:
- testa:      puntatore alla testa della lista (puo' essere NULL)
- valore:     stringa da confrontare (non NULL)
- tipoFiltro: FILTRO_AREA oppure FILTRO_TECNICO
Ritorna:
- void
*/
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

/*
Scorre la lista e stampa tutte le richieste con la tipologia specificata.
Versione iterativa di cercaPerTipologia, utilizzata dal menu di ricerca principale.
Parametri:
- testa:     puntatore alla testa della lista (puo' essere NULL)
- tipologia: specializzazione da ricercare
Ritorna:
- void
*/
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

/*
Stampa tutte le richieste assegnate al tecnico indicato.
Parametri:
- testa:       puntatore alla testa della lista (puo' essere NULL)
- nomeTecnico: nome del tecnico da ricercare (non NULL)
Ritorna:
- void
*/
void stampaRichiestePerTecnico(Richiesta* testa, const char* nomeTecnico) {
    stampaRichiestePerStringa(testa, nomeTecnico, FILTRO_TECNICO);
}


/*
Trova e stampa l'area con il maggior numero di richieste presenti in lista.
Utilizzata dal modulo di reportistica per identificare le zone piu' critiche dell'edificio.
Algoritmo O(n^2): due cicli annidati, il ciclo esterno itera su ogni nodo,
quello interno conta quante richieste hanno la stessa area.
Parametri:
- testa: puntatore alla testa della lista (puo' essere NULL)
Ritorna:
- void
*/
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

/*
Dealloca ricorsivamente tutti i nodi della lista collegata.
Utilizzata alla chiusura del programma per liberare tutta la memoria allocata per le richieste.
Parametri:
- testa: puntatore alla testa della lista (puo' essere NULL)
Ritorna:
- void
*/
void liberaListaRichieste(Richiesta* testa) {
    if (testa == NULL) return;              //caso base: lista vuota
    liberaListaRichieste(testa->next);      //prima libera il resto della lista
    free(testa);                            //poi libera il nodo corrente
	//Si libera prima ricorsivamente il resto della lista e poi il nodo corrente (ordine post-order): 
	//se si liberasse testa prima della chiamata ricorsiva, testa->next diventerebbe invalido
    //e non sarebbe piu' possibile raggiungere i nodi successivi. 
}

//ADT PRIORITY QUEUE
/*
Scambia due puntatori a Richiesta nell'array heap.
Funzione di supporto utilizzata da sali e scendi per riorganizzare i nodi dell'heap.
Parametri:
- a: puntatore al primo elemento da scambiare (non NULL)
- b: puntatore al secondo elemento da scambiare (non NULL)
Ritorna:
- void
*/ 
static void swap(Richiesta** a, Richiesta** b) {
    Richiesta* tmp = *a;
    *a = *b;
    *b = tmp;
}

/*✦+
Ripristina l'invariante Max-Heap risalendo dall'indice k verso la radice.
Chiamata da insertPQ dopo aver inserito un nuovo elemento come ultima foglia.
Continua a scambiare il nodo con il genitore finche' il genitore ha urgenza maggiore o uguale,
oppure finche' si raggiunge la radice.
Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
- k:  indice di partenza nell'array heap (1-based)
Ritorna:
- void
*/
static void sali(PriorityQueue* pq, int k) {
    while (k > 1 && pq->heap[k / 2]->urgenza < pq->heap[k]->urgenza) {
        swap(&pq->heap[k / 2], &pq->heap[k]);
        k = k / 2;
    }
}

/*
Ripristina l'invariante Max-Heap scendendo dall'indice k verso le foglie.
Chiamata da deleteMax dopo aver spostato l'ultima foglia in radice.
Ad ogni passo sceglie il figlio con urgenza maggiore e scambia se necessario,
fermandosi quando il nodo corrente ha urgenza maggiore o uguale di entrambi i figli.
Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
- k:  indice di partenza nell'array heap (1-based)
Ritorna:
- void
*/
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

/*
Inizializza la coda a priorita' producendo un heap vuoto.
Deve essere chiamata prima di qualsiasi altro utilizzo della PriorityQueue.
Parametri:
- pq: puntatore alla struttura PriorityQueue da inizializzare (non NULL)
Ritorna:
- void
*/ 
void initPQ(PriorityQueue* pq) {
    if (pq == NULL) return;
    pq->size    = 0;
    pq->heap[0] = NULL;  // heap[0] non viene usato (convenzione 1-based): lo azzeriamo per sicurezza
}

/*
Verifica se la coda a priorita' e' vuota.
Utilizzata da deleteMax, stampaHeap e dai cicli di estrazione per controllare la condizione di uscita.
Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
Ritorna:
- 1 se la coda e' vuota, 0 altrimenti.
*/
int emptyPQ(const PriorityQueue* pq) {
    if (pq == NULL) return 1;
    return pq->size == 0;
}

/*
Verifica se la coda a priorita' ha raggiunto la capacita' massima MAX_HEAP.
Utilizzata da insertPQ prima di tentare l'inserimento.
Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
Ritorna:
- 1 se la coda e' piena, 0 altrimenti.
*/
int fullPQ(const PriorityQueue* pq) {
    if (pq == NULL) return 1;
    return pq->size >= MAX_HEAP;
}

/*
Inserisce una richiesta nella coda a priorita'.
Aggiunge il nuovo elemento come ultima foglia (heap[size+1]) e chiama sali per ripristinare
l'invariante Max-Heap.
Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
- r:  puntatore alla richiesta da inserire (non NULL)
Ritorna:
- 1 se l'inserimento ha avuto successo, 0 se la coda e' piena o i parametri sono NULL.
*/
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

/*
Estrae e restituisce la richiesta con urgenza massima (radice dell'heap).
Sposta l'ultima foglia in posizione 1, decrementa size e chiama scendi per ripristinare
l'invariante Max-Heap.
Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
Ritorna:
- puntatore alla richiesta con urgenza massima, oppure NULL se la coda e' vuota.
*/
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

/*
Restituisce la richiesta con urgenza massima senza modificare l'heap.
Utilizzata quando si vuole consultare la prossima richiesta da elaborare senza estrarla.
Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
Ritorna:
- puntatore alla richiesta in cima all'heap, oppure NULL se la coda e' vuota.
*/
Richiesta* peekMax(const PriorityQueue* pq) {
    if (pq == NULL || emptyPQ(pq)) return NULL;
    return pq->heap[1];
}


/*
Stampa tutte le richieste dell'heap in ordine decrescente di urgenza.
Opera su una copia superficiale (shallow copy) dell'heap per non alterare la struttura originale:
deleteMax sulla copia riorganizza solo i puntatori nell'array senza toccare gli oggetti Richiesta.
Parametri:
- pq: puntatore alla struttura PriorityQueue (non NULL)
Ritorna:
- void
*/
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
