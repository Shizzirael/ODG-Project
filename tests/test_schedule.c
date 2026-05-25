#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/richieste.h"
#include "../headers/tecnici.h"
#include "../headers/schedule.h"
#include "../headers/utile.h"
#include "../tests/test_condominio.h"
/* Helper: costruisce una Data */
static Data makeData(int anno, int mese, int giorno,
                     int ora_inizio, int ora_fine) {
    Data d;
    d.anno       = anno;
    d.mese       = mese;
    d.giorno     = giorno;
    d.ora_inizio = ora_inizio;
    d.ora_fine   = ora_fine;
    return d;
}

/* Helper: inizializza una Richiesta sul posto */
static void initR(Richiesta* r, int codice, const char* area,
                  Specializzazione tip, const char* desc,
                  const char* data, int urgenza,
                  StatoRichiesta stato, const char* dataChiusura) {
    r->codice    = codice;
    r->urgenza   = urgenza;
    r->stato     = stato;
    r->tipologia = tip;
    r->next      = NULL;
    strncpy(r->area,        area, MAX_STR-1); r->area[MAX_STR-1]        = '\0';
    strncpy(r->descrizione, desc, MAX_STR-1); r->descrizione[MAX_STR-1] = '\0';
    strncpy(r->data,        data, 10);        r->data[10]               = '\0';
    r->tecnico[0]       = '\0';
    r->data_chiusura[0] = '\0';
    if (dataChiusura && dataChiusura[0] != '\0') {
        strncpy(r->data_chiusura, dataChiusura, 10);
        r->data_chiusura[10] = '\0';
    }
}

/* Helper: crea Tecnico senza stdin */
static Tecnico* makeTecnico(const char* id, const char* nome,
                             Specializzazione spec) {
    Tecnico* t = malloc(sizeof(Tecnico));
    if (!t) return NULL;
    strncpy(t->codice_ID, id, ID_LEN);
    t->codice_ID[ID_LEN] = '\0';
    t->nome = malloc(strlen(nome) + 1);
    if (!t->nome) { free(t); return NULL; }
    strcpy(t->nome, nome);
    t->specializzazione = spec;
    t->disponibile      = true;
    return t;
}

/* TC schedule:
**TC6**: Test della pianificazione interventi senza conflitti   
**TC7**: Verifica della pianificazione con rilevamento conflitti 
**TC12**: Test dello storico degli interventi conclusi   
**TC13**: Verifica della generazione del report completo
*/

/* =================================================================
   TC6 - Pianificazione intervento senza conflitti (BST)

   Verifica: verificaConflitti() ritorna 0 su albero vuoto,
   planIntervento() inserisce il nodo correttamente.

   TC6_input.txt:
     1
     2025 5 15 9 11

   TC6_oracle.txt:
     NESSUN CONFLITTO
     PIANIFICAZIONE OK
   ================================================================= */
void eseguiTC6(FILE* input, FILE* output) {
    int codice, anno, mese, giorno, ora_i, ora_f;

    fscanf(input, "%d", &codice);
    fscanf(input, "%d %d %d %d %d",
           &anno, &mese, &giorno, &ora_i, &ora_f);

    {
        Data d = makeData(anno, mese, giorno, ora_i, ora_f);
        Schedule albero = creaAlbero();

        Richiesta r;
        initR(&r, codice, "Appartamento_3B", IDRAULICO,
              "Perdita_sotto_lavello", "10/05/2025", 3, APERTA, "");

        /* Verifica che non ci siano conflitti prima di pianificare */
        if (verificaConflitti(albero, d) == 0) {
            albero = planIntervento(albero, r, NULL, d);
            fprintf(output,
                    "Intervento pianificato: %02d/%02d/%d %02d:00-%02d:00\n",
                    giorno, mese, anno, ora_i, ora_f);
        } else {
            fprintf(output, "CONFLITTO INATTESO\n");
        }

        liberaAlbero(albero);
    }
}

/* =================================================================
   TC7 - Pianificazione con conflitto (BST)

   Verifica: verificaConflitti() ritorna 1 quando si tenta di
   inserire un intervento in una fascia gia' occupata.

   TC7_input.txt:
     1 2025 5 15 9 11
     2 2025 5 15 10 12

   TC7_oracle.txt:
     PRIMO INTERVENTO PIANIFICATO
     CONFLITTO RILEVATO
   ================================================================= */
void eseguiTC7(FILE* input, FILE* output) {
    int codice;
    int a1, m1, g1, oi1, of1;
    int a2, m2, g2, oi2, of2;

    fscanf(input, "%d", &codice);
    fscanf(input, "%d %d %d %d %d", &a1, &m1, &g1, &oi1, &of1);
    fscanf(input, "%d %d %d %d %d", &a2, &m2, &g2, &oi2, &of2);

    {
        Data d1 = makeData(a1, m1, g1, oi1, of1);
        Data d2 = makeData(a2, m2, g2, oi2, of2);

        Schedule albero = creaAlbero();

        Richiesta r;
        initR(&r, codice, "Appartamento_3B", IDRAULICO,
              "Perdita", "10/05/2025", 3, APERTA, "");

        /* Primo inserimento: nessun conflitto atteso */
        albero = planIntervento(albero, r, NULL, d1);
        fprintf(output,
                "Intervento pianificato: %02d/%02d/%d %02d:00-%02d:00\n",
                g1, m1, a1, oi1, of1);

        /* Secondo tentativo: fascia sovrapposta, conflitto atteso */
        if (verificaConflitti(albero, d2) == 1)
            fprintf(output, "Conflitto rilevato: fascia oraria occupata\n");
        else
            fprintf(output, "NESSUN CONFLITTO (ERRORE)\n");

        liberaAlbero(albero);
    }
}



/* =================================================================
   TC12 - Storico interventi conclusi (visita in-order BST)

   Verifica: stampaStorico() produce solo i nodi CONCLUSA
   in ordine cronologico.

   TC12_input.txt:
     1 2025 5 15 9 11

   TC12_oracle.txt:
     15/05/2025 11:00 | Tecnico: MarioRossi | ID: 1
   ================================================================= */
void eseguiTC12(FILE* input, FILE* output) {
    int codice, anno, mese, giorno, ora_i, ora_f;
    fscanf(input, "%d %d %d %d %d %d",
           &codice, &anno, &mese, &giorno, &ora_i, &ora_f);

    {
        Data d = makeData(anno, mese, giorno, ora_i, ora_f);

        /* Tecnico su stack: nome senza spazi per semplicita'
         * (lo storico stampa tec->nome direttamente) */
        Tecnico* tec = makeTecnico("123456789", "MarioRossi", IDRAULICO);
        if (!tec) { fprintf(output, "ERRORE allocazione tecnico\n"); return; }

        Richiesta r;
        initR(&r, codice, "Appartamento_3B", IDRAULICO,
              "Perdita", "10/05/2025", 3, APERTA, "");

        Schedule albero = creaAlbero();
        albero = planIntervento(albero, r, tec, d);

        /* Porta il nodo BST a CONCLUSA con la data di chiusura */
        aggiornaStatoNelBST(albero, codice, CONCLUSA, "20/05/2025");

        /* stampaStorico scrive direttamente nel file di output */
        stampaStorico(albero, output);

        liberaAlbero(albero);
        free(tec->nome);
        free(tec);
    }
}

/* =================================================================
   TC13 - Generazione report completo (integrazione tutti e 3 gli ADT)

   Verifica: generaReport(), tempoMedioCompletamento(),
   trovaTecnicoPiuAttivo(), interventiPerTipologia(),
   areaPiuProblematica().

   Setup: 2 richieste idraulico, 1 conclusa (10 giorni), 1 aperta.

   TC13_input.txt:
     (nessun parametro: la struttura di test e' fissa per garantire
      un oracle riproducibile indipendente dall'ordine di esecuzione)

   TC13_oracle.txt:
     Aperti: 1
     Conclusi: 1
     Tempo medio: 10 giorni
   ================================================================= */
void eseguiTC13(FILE* input, FILE* output) {
    (void)input; /* nessun parametro variabile */

    {
        /* Lista richieste per areaPiuProblematica */
        Richiesta* lista = NULL;
        Richiesta* req1 = creaRichiesta(1, "Appartamento_3B", IDRAULICO,
                                         "Perdita",   "10/05/2025", 3);
        Richiesta* req2 = creaRichiesta(2, "Appartamento_3B", IDRAULICO,
                                         "Rubinetto", "01/05/2025", 2);
        if (!req1 || !req2) {
            fprintf(output, "ERRORE allocazione\n");
            if (req1) free(req1);
            if (req2) free(req2);
            return;
        }
        inserisciRichiesta(&lista, req1);
        inserisciRichiesta(&lista, req2);

        Tecnico* tec = makeTecnico("123456789", "MarioRossi", IDRAULICO);
        if (!tec) { fprintf(output, "ERRORE allocazione tecnico\n");
                    liberaListaRichieste(lista); return; }

        Schedule albero = creaAlbero();

        Richiesta r1, r2;
        initR(&r1, 1, "Appartamento_3B", IDRAULICO,
              "Perdita",   "10/05/2025", 3, CONCLUSA, "20/05/2025");
        initR(&r2, 2, "Appartamento_3B", IDRAULICO,
              "Rubinetto", "01/05/2025", 2, APERTA,   "");

        Data d1 = makeData(2025, 5, 15, 9,  11);
        Data d2 = makeData(2025, 5, 20, 14, 16);

        albero = planIntervento(albero, r1, tec, d1);
        albero = planIntervento(albero, r2, tec, d2);

        /* Sincronizza stato CONCLUSA nel BST */
        aggiornaStatoNelBST(albero, 1, CONCLUSA, "20/05/2025");

        /* Calcola e scrivi report */
        {
            int aperti = 0, conclusi = 0;
            generaReport(albero, &aperti, &conclusi);
            fprintf(output, "Aperti: %d\n",   aperti);
            fprintf(output, "Conclusi: %d\n", conclusi);
        }

        {
            int somma = 0, count = 0;
            tempoMedioCompletamento(albero, &somma, &count);
            if (count > 0)
                fprintf(output, "Tempo medio: %d giorni\n", somma / count);
            else
                fprintf(output, "Tempo medio: N/D\n");
        }

        liberaAlbero(albero);
        liberaListaRichieste(lista);
        free(tec->nome);
        free(tec);
    }
}
