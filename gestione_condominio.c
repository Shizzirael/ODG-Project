/* 
 File:        gestione_condominio.c (Modulo principale per la gestione del condominio)
 Autori:      Assuntapia Piccolo,Fabiana Sasso, Lucia Schettino
 Matricole:   NF12100574, NF12100245, NF12100025 
 Descrizione: Punto di ingresso del programma, gestisce il menu principale e coordina le operazioni sui moduli di richieste e tecnici.
              Utilizza funzioni di utilità per la gestione dell'input e altre operazioni comuni.
*/

#include <stdio.h>
#include <stdlib.h>

#include "headers/tecnici.h"
#include "headers/richieste.h"
#include "headers/schedule.h"
#include "headers/utile.h"

/*
  Sottomenu 1 - Richieste
*/

static void menuNuovaRichiesta(Richiesta** testa, ListaTecnici* tecnici) {
    char area[MAX_STR];
    char descrizione[MAX_STR];
    char data[11];
    int  urgenza, sceltaTipologia;

    printf("\n--- NUOVA RICHIESTA ---\n");

    printf("Area/Appartamento: ");
    leggiParola(area, MAX_STR);

    printf("Tipologia (0=Idraulico 1=Elettricista 2=Muratore 3=Ascensorista 4=Generico): ");
    if (!leggiIntero(&sceltaTipologia) || sceltaTipologia < 0 || sceltaTipologia > 4) {
        printf("Tipologia non valida.\n");
        return;
    }

    
    printf("Descrizione (frase intera): ");
    leggiRiga(descrizione, MAX_STR);

    
    leggiData(data);

    do {
        printf("Urgenza (1-5): ");
        if (!leggiIntero(&urgenza)) urgenza = 0;
    } while (urgenza < 1 || urgenza > 5);

    int codice = prossimoCodice();
    Richiesta* nuova = creaRichiesta(codice, area,
                                     (Specializzazione)sceltaTipologia,
                                     descrizione, data, urgenza);
    if (nuova == NULL) {
        printf("Errore: impossibile allocare la richiesta.\n");
        return;
    }

    inserisciRichiesta(testa, nuova);
    printf("Richiesta creata con codice %d.\n", codice);

    // FIX: aggiorniamo *tecnici cosi il puntatore resta valido nel main
    *tecnici = assegnaRichiesta(*tecnici, (Specializzazione)sceltaTipologia, nuova);
    if (nuova->tecnico[0] != '\0')
        printf("Tecnico assegnato automaticamente: %s\n", nuova->tecnico);
    else
        printf("Nessun tecnico disponibile per questa tipologia al momento.\n");
}

static void menuVisualizzaRichieste(Richiesta* testa) {
    int scelta, valore;
    char stringa[MAX_STR];

    printf("\n--- VISUALIZZA RICHIESTE ---\n");
    printf("  1. Tutte\n");
    printf("  2. Per stato\n");
    printf("  3. Per urgenza\n");
    printf("  4. Per area/appartamento\n");
    printf("  5. Per tecnico\n");
    printf("  6. Per tipologia\n");
    printf("  0. Indietro\n");
    printf("Scelta: ");

    if (!leggiIntero(&scelta)) return;

    switch (scelta) {
        case 1:
            if (testa == NULL) { printf("Nessuna richiesta presente.\n"); break; }
            {
                Richiesta* temp = testa;
                while (temp != NULL) {
                    stampaDettaglioRichiesta(temp);
                    temp = temp->next;
                }
            }
            break;

        case 2:
            printf("Stato (0=APERTA 1=PIANIFICATA 2=IN_LAVORAZIONE 3=CONCLUSA 4=ANNULLATA): ");
            if (!leggiIntero(&valore)) return;
            stampaRichiesteFiltrate(testa, valore, FILTRO_STATO);
            break;

        case 3:
            printf("Urgenza (1-5): ");
            if (!leggiIntero(&valore)) return;
            stampaRichiesteFiltrate(testa, valore, FILTRO_URGENZA);
            break;

        case 4:
            printf("Area: ");
            leggiParola(stringa, MAX_STR);
            stampaRichiestePerStringa(testa, stringa, FILTRO_AREA);
            break;

        case 5:
            printf("Nome tecnico: ");
            leggiParola(stringa, MAX_STR);
            stampaRichiestePerStringa(testa, stringa, FILTRO_TECNICO);
            break;

        case 6: {
            int tip;
            printf("Tipologia (0=Idraulico 1=Elettricista 2=Muratore 3=Ascensorista 4=Generico): ");
            if (!leggiIntero(&tip)) return;
            stampaRichiestePerTipologia(testa, (Specializzazione)tip);
            break;
        }

        case 0: break;
        default: printf("Scelta non valida.\n");
    }
}

static void menuRicercaRichieste(Richiesta* testa) {
    int scelta;

    printf("\n--- RICERCA RICHIESTE ---\n");
    printf("  1. Per codice\n");
    printf("  2. Per tipologia\n");
    printf("  0. Indietro\n");
    printf("Scelta: ");

    if (!leggiIntero(&scelta)) return;

    if (scelta == 1) {
        int codice;
        printf("Codice: ");
        if (!leggiIntero(&codice)) return;
        cercaEStampaPerCodice(testa, codice);
    } else if (scelta == 2) {
        int tip;
        printf("Tipologia (0=Idraulico 1=Elettricista 2=Muratore 3=Ascensorista 4=Generico): ");
        if (!leggiIntero(&tip)) return;
        cercaPerTipologia(testa, (Specializzazione)tip);
    }
}

static void menuRichieste(Richiesta** testa, ListaTecnici* tecnici, Schedule albero) {
    int scelta;

    do {
        printf("\n=== GESTIONE RICHIESTE ===\n");
        printf("  1. Nuova richiesta\n");
        printf("  2. Aggiorna stato\n");
        printf("  3. Visualizza richieste\n");
        printf("  4. Ricerca\n");
        printf("  5. Area piu' problematica\n");
        printf("  0. Indietro\n");
        printf("Scelta: ");

        if (!leggiIntero(&scelta)) continue;

        switch (scelta) {
            case 1:
                menuNuovaRichiesta(testa, tecnici);
                break;

            case 2: {
                menuAggiornaStato(*testa);
                printf("Inserisci il codice della richiesta appena aggiornata\n");
                printf("(per aggiornare anche il piano interventi): ");
                int cod;
                if (leggiIntero(&cod)) {
                    Richiesta* r = cercaPerCodice(*testa, cod);
                    if (r != NULL)
                        aggiornaStatoNelBST(albero, cod, r->stato, r->data_chiusura);
                }
                break;
            }

            case 3: menuVisualizzaRichieste(*testa); break;
            case 4: menuRicercaRichieste(*testa);    break;
            case 5: areaPiuProblematica(*testa);      break;
            case 0: break;
            default: printf("Scelta non valida.\n");
        }
    } while (scelta != 0);
}

// Sottomenu 2 - Tecnici

static void assegnaRichiesteOrfane(ListaTecnici* tecnici, Richiesta* richieste) {
    Richiesta* r = richieste;
    int assegnate = 0;

    while (r != NULL) {
        if (r->stato == APERTA && r->tecnico[0] == '\0') {
            *tecnici = assegnaRichiesta(*tecnici, r->tipologia, r);
            if (r->tecnico[0] != '\0') {
                printf("  -> Richiesta #%d (%s) assegnata a: %s\n",
                       r->codice, r->area, r->tecnico);
                assegnate++;
            }
        }
        r = r->next;
    }

    if (assegnate == 0)
        printf("  Nessuna richiesta in attesa compatibile con questo tecnico.\n");
    else
        printf("  %d richiesta/e assegnata/e automaticamente.\n", assegnate);
}

static void menuTecnici(ListaTecnici* tecnici, Richiesta* richieste) {
    int scelta;

    do {
        printf("\n=== GESTIONE TECNICI ===\n");
        printf("  1. Aggiungi tecnico\n");
        printf("  2. Monitora carico di lavoro\n");
        printf("  0. Indietro\n");
        printf("Scelta: ");

        if (!leggiIntero(&scelta)) continue;

        switch (scelta) {
            case 1:
                *tecnici = aggiungiTecnico(*tecnici);
                printf("Tecnico aggiunto con successo.\n");
                printf("Controllo richieste in attesa compatibili...\n");
                assegnaRichiesteOrfane(tecnici, richieste);
                break;
            case 2:
                monitoraCarico(*tecnici);
                break;
            case 0: break;
            default: printf("Scelta non valida.\n");
        }
    } while (scelta != 0);
}


// Sottomenu 3 - Pianificazione

static void menuNuovoPiano(Schedule* albero, Richiesta* richieste, ListaTecnici tecnici) {
    int codice;
    Data data;

    printf("\n--- PIANIFICA INTERVENTO ---\n");
    printf("Codice richiesta da pianificare: ");
    if (!leggiIntero(&codice)) return;

    Richiesta* r = cercaPerCodice(richieste, codice);
    if (r == NULL) {
        printf("Richiesta con codice %d non trovata.\n", codice);
        return;
    }
    if (r->stato != APERTA && r->stato != PIANIFICATA) {
        printf("La richiesta e' in stato finale: non pianificabile.\n");
        return;
    }

//ug fix blocca doppia pianificazione della stessa richiesta.
//cercaIntervento visita tutto l'albero cercando il codice.
    if (cercaIntervento(*albero, codice)) {
        printf("Errore: la richiesta %d ha gia' un intervento pianificato.\n", codice);
        return;
    }

    int annoRic = 0, meseRic = 0, giornoRic = 0;
    sscanf(r->data, "%d/%d/%d", &giornoRic, &meseRic, &annoRic);

    do {
        printf("Anno intervento (2000-2100): ");
        if (!leggiIntero(&data.anno)) return;
        if (data.anno < 2000 || data.anno > 2100)
            printf("  Anno non valido, inserisci un valore tra 2000 e 2100.\n");
        else if (data.anno < annoRic)
            printf("  Anno non valido: la richiesta e' stata aperta nel %d.\n", annoRic);
    } while (data.anno < 2000 || data.anno > 2100 || data.anno < annoRic);

    do {
        printf("Mese (1-12): ");
        if (!leggiIntero(&data.mese)) return;
        if (data.mese < 1 || data.mese > 12)
            printf("  Mese non valido, inserisci un valore tra 1 e 12.\n");
        else if (data.anno == annoRic && data.mese < meseRic)
            printf("  Mese non valido: la richiesta e' stata aperta nel mese %d/%d.\n", meseRic, annoRic);
    } while (data.mese < 1 || data.mese > 12 || (data.anno == annoRic && data.mese < meseRic));

    do {
        printf("Giorno (1-31): ");
        if (!leggiIntero(&data.giorno)) return;
        if (data.giorno < 1 || data.giorno > 31)
            printf("  Giorno non valido, inserisci un valore tra 1 e 31.\n");
        else if (data.anno == annoRic && data.mese == meseRic && data.giorno < giornoRic)
            printf("  Giorno non valido: la richiesta e' stata aperta il %02d/%02d/%d.\n", giornoRic, meseRic, annoRic);
    } while (data.giorno < 1 || data.giorno > 31 ||
             (data.anno == annoRic && data.mese == meseRic && data.giorno < giornoRic));

    do {
        printf("Ora inizio (0-23): ");
        if (!leggiIntero(&data.ora_inizio)) return;
        if (data.ora_inizio < 0 || data.ora_inizio > 23)
            printf("  Ora non valida, inserisci un valore tra 0 e 23.\n");
    } while (data.ora_inizio < 0 || data.ora_inizio > 23);

    do {
        printf("Ora fine (%d < ora_fine <= 24): ", data.ora_inizio);
        if (!leggiIntero(&data.ora_fine)) return;
        if (data.ora_fine <= data.ora_inizio || data.ora_fine > 24)
            printf("  Ora fine non valida: deve essere maggiore di %d e al massimo 24.\n", data.ora_inizio);
    } while (data.ora_fine <= data.ora_inizio || data.ora_fine > 24);

    if (verificaConflitti(*albero, data)) {
        printf("Conflitto rilevato: esiste gia' un intervento in questa fascia oraria.\n");
        return;
    }

    Tecnico* tec = NULL;
    if (r->tecnico[0] != '\0') {
        tec = trovaTecnicoPerNome(tecnici, r->tecnico);
        if (tec == NULL)
            printf("Attenzione: tecnico '%s' non trovato nella lista. Procedo senza.\n", r->tecnico);
    } else {
        printf("Attenzione: nessun tecnico assegnato a questa richiesta.\n");
        printf("Procedere comunque? (1=si 0=no): ");
        int conf;
        if (!leggiIntero(&conf) || conf != 1) return;
    }

    *albero = planIntervento(*albero, *r, tec, data);
    printf("Intervento pianificato con successo.\n");
}

static void menuPianificazione(Schedule* albero, Richiesta* richieste, ListaTecnici tecnici) {
    int scelta;

    do {
        printf("\n=== PIANIFICAZIONE INTERVENTI ===\n");
        printf("  1. Pianifica nuovo intervento\n");
        printf("  2. Cerca intervento per codice\n");
        printf("  3. Storico interventi conclusi\n");
        printf("  0. Indietro\n");
        printf("Scelta: ");

        if (!leggiIntero(&scelta)) continue;

        switch (scelta) {
            case 1:
                menuNuovoPiano(albero, richieste, tecnici);
                break;

            case 2: {
                int cod;
                printf("Codice intervento: ");
                if (!leggiIntero(&cod)) break;
                if (!cercaIntervento(*albero, cod))
                    printf("Nessun intervento trovato con codice %d.\n", cod);
                break;
            }

            case 3:
                if (*albero == NULL)
                    printf("Nessun intervento pianificato.\n");
                else {
                    int aperti = 0, conclusi = 0;
                    generaReport(*albero, &aperti, &conclusi);
                    if (conclusi == 0)
                        printf("Nessun intervento concluso ancora.\n");
                    else
                        stampaStorico(*albero);
                }
                break;

            case 0: break;
            default: printf("Scelta non valida.\n");
        }
    } while (scelta != 0);
}


// Sottomenu 4 - Report
 

static void menuReport(Schedule albero, Richiesta* richieste) {
    printf("\n=== REPORT E STATISTICHE ===\n");

    int aperti = 0, conclusi = 0;
    generaReport(albero, &aperti, &conclusi);
    printf("\nInterventi pianificati/aperti : %d\n", aperti);
    printf("Interventi conclusi           : %d\n", conclusi);

    int somma = 0, count = 0;
    tempoMedioCompletamento(albero, &somma, &count);
    if (count > 0)
        printf("Tempo medio completamento     : %d giorni (su %d interventi)\n",
               somma / count, count);
    else
        printf("Tempo medio completamento     : N/D (nessun intervento concluso)\n");

    printf("\nTecnico piu' attivo:\n");
    trovaTecnicoPiuAttivo(albero);

    interventiPerTipologia(albero);

    areaPiuProblematica(richieste);
}


// MENU PRINCIPALE

int main(void) {
    Richiesta*   richieste = NULL;
    ListaTecnici tecnici   = NULL;
    Schedule     albero    = NULL;

    int scelta;

    stampaBanner();

    do {
        printf("\n=== MENU PRINCIPALE ===\n");
        printf("  1. Gestione Richieste\n");
        printf("  2. Gestione Tecnici\n");
        printf("  3. Pianificazione Interventi\n");
        printf("  4. Report e Statistiche\n");
        printf("  0. Esci\n");
        printf("Scelta: ");

        if (!leggiIntero(&scelta)) {
            printf("Input non valido, riprova.\n");
            continue;
        }

        switch (scelta) {
            case 1:
                // Passa&tecnici cosi menuRichieste puo' aggiornarlo
                // quando assegnaRichiesta riordina la lista
                menuRichieste(&richieste, &tecnici, albero);
                break;

            case 2:
                menuTecnici(&tecnici, richieste);
                break;

            case 3:
                menuPianificazione(&albero, richieste, tecnici);
                break;

            case 4:
                menuReport(albero, richieste);
                break;

            case 0:
                printf("Uscita dal programma. Liberazione memoria in corso...\n");
                break;

            default:
                printf("Scelta non valida, riprova.\n");
        }
    } while (scelta != 0);

    liberaListaRichieste(richieste);
    liberaLista(tecnici);
    liberaAlbero(albero);

    return 0;
}
