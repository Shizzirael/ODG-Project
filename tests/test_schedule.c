#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/richieste.h"
#include "../headers/tecnici.h"
#include "../headers/schedule.h"
#include "../headers/utile.h"
#include "../tests/test_condominio.h"

/* TC schedule:
TC6 - Test della pianificazione interventi senza conflitti   
TC7 - Verifica della pianificazione con rilevamento conflitti 
TC12 - Test dello storico degli interventi conclusi   
TC13 - Verifica della generazione del report completo
*/


 void eseguiTC6(FILE* input, FILE* output) {
    int codice, anno, mese, giorno, ora_i, ora_f;

    fscanf(input, "%d", &codice);
    fscanf(input, "%d %d %d %d %d", &anno, &mese, &giorno, &ora_i, &ora_f);

    {
        Data d;
        d.anno = anno; d.mese = mese; d.giorno = giorno;
        d.ora_inizio = ora_i; d.ora_fine = ora_f;

        Schedule albero = creaAlbero();
        Richiesta* r = creaRichiesta(codice, "AreaTest", IDRAULICO,
                                     "Desc", "15/05/2025", 3);
        if (r == NULL) { fprintf(output, "ERRORE allocazione\n"); return; }

        if (verificaConflitti(albero, d) == 0) {
            fprintf(output, "NESSUN CONFLITTO\n");
            albero = planIntervento(albero, *r, NULL, d);
            fprintf(output, "PIANIFICAZIONE OK\n");
        } else {
            fprintf(output, "CONFLITTO INATTESO\n");
        }

        liberaAlbero(albero);
        free(r);
    }
}


 void eseguiTC7(FILE* input, FILE* output) {
    int cod1, a1, m1, g1, oi1, of1;
    int cod2, a2, m2, g2, oi2, of2;

    fscanf(input, "%d %d %d %d %d %d", &cod1, &a1, &m1, &g1, &oi1, &of1);
    fscanf(input, "%d %d %d %d %d %d", &cod2, &a2, &m2, &g2, &oi2, &of2);

    {
        Data d1; d1.anno=a1; d1.mese=m1; d1.giorno=g1;
                 d1.ora_inizio=oi1; d1.ora_fine=of1;
        Data d2; d2.anno=a2; d2.mese=m2; d2.giorno=g2;
                 d2.ora_inizio=oi2; d2.ora_fine=of2;

        Schedule albero = creaAlbero();
        Richiesta* r1 = creaRichiesta(cod1, "AreaA", IDRAULICO,
                                      "Desc1", "15/05/2025", 3);
        Richiesta* r2 = creaRichiesta(cod2, "AreaB", ELETTRICISTA,
                                      "Desc2", "15/05/2025", 5);
        if (r1 == NULL || r2 == NULL) {
            fprintf(output, "ERRORE allocazione\n");
            if (r1) free(r1);
            if (r2) free(r2);
            liberaAlbero(albero);
            return;
        }

        if (verificaConflitti(albero, d1) == 0) {
            albero = planIntervento(albero, *r1, NULL, d1);
            fprintf(output, "PRIMO INTERVENTO PIANIFICATO\n");
        }

        if (verificaConflitti(albero, d2) == 1)
            fprintf(output, "CONFLITTO RILEVATO\n");
        else
            fprintf(output, "NESSUN CONFLITTO INATTESO\n");

        liberaAlbero(albero);
        free(r1);
        free(r2);
    }
}




 void eseguiTC12(FILE* input, FILE* output) {
    int codice, anno, mese, giorno, ora_i, ora_f;
    if (fscanf(input, "%d %d %d %d %d %d",
           &codice, &anno, &mese, &giorno, &ora_i, &ora_f) != 6) return;

    {
      Data d;
      d.anno = anno; 
      d.mese = mese; 
      d.giorno = giorno;
      d.ora_inizio = ora_i; 
      d.ora_fine = ora_f;

        Tecnico* tec = malloc(sizeof(Tecnico));
        if (!tec) return;

        strncpy(tec->codice_ID, "123456789", ID_LEN);
        tec->codice_ID[ID_LEN] = '\0';
        tec->nome = malloc(strlen("MarioRossi") + 1);
        strcpy(tec->nome, "MarioRossi");
        tec->specializzazione = IDRAULICO;
        tec->disponibile = 1;

        Richiesta* r = malloc(sizeof(Richiesta));
        if (!r) { free(tec->nome); free(tec); return; }
        
        r->codice = codice; 
        r->tipologia = IDRAULICO;
        r->urgenza = 3;
        r->stato = APERTA;
        r->next = NULL;

        strncpy(r->area, "Appartamento_3B", MAX_STR - 1);  
            r->area[MAX_STR - 1] = '\0';
        strncpy(r->descrizione, "Perdita", MAX_STR - 1);  
            r->descrizione[MAX_STR - 1] = '\0';
        strncpy(r->data, "10/05/2025", 10);              
            r->data[10] = '\0';
            r->tecnico[0] = '\0';
            r->data_chiusura[0] = '\0';

        Schedule albero = creaAlbero();
        albero = planIntervento(albero, *r, tec, d);

        aggiornaStatoNelBST(albero, codice, CONCLUSA, "20/05/2025");

        stampaStorico(albero, output);

        liberaAlbero(albero);
        free(tec->nome);
        free(tec);
        free(r);
    }
}


 void eseguiTC13(FILE* input, FILE* output) {
    (void)input; 

    {
        Richiesta* lista = NULL;
        Richiesta* r1 = creaRichiesta(1, "Appartamento_3B", IDRAULICO,
                                      "Perdita", "10/05/2025", 3);
        Richiesta* r2 = creaRichiesta(2, "Appartamento_3B", IDRAULICO,
                                      "Rubinetto", "01/05/2025", 2);
        if (r1 == NULL || r2 == NULL) {
            fprintf(output, "ERRORE allocazione\n");
            if (r1) free(r1);
            if (r2) free(r2);
            return;
        }
        inserisciRichiesta(&lista, r1);
        inserisciRichiesta(&lista, r2);

        Tecnico tec;
        strncpy(tec.codice_ID, "123456789", ID_LEN);
        tec.codice_ID[ID_LEN] = '\0';
        tec.nome = "MarioRossi";
        tec.specializzazione = IDRAULICO;
        tec.disponibile = 1;

        Schedule albero = creaAlbero();
        Data d1; d1.anno=2025; d1.mese=5; d1.giorno=15;
                 d1.ora_inizio=9;  d1.ora_fine=11;
        Data d2; d2.anno=2025; d2.mese=5; d2.giorno=20;
                 d2.ora_inizio=14; d2.ora_fine=16;

        albero = planIntervento(albero, *r1, &tec, d1);
        albero = planIntervento(albero, *r2, &tec, d2);

        aggiornaStatoNelBST(albero, 1, CONCLUSA, "20/05/2025");

        int aperti = 0, conclusi = 0;
        generaReport(albero, &aperti, &conclusi);
        fprintf(output, "Aperti: %d\n",   aperti);
        fprintf(output, "Conclusi: %d\n", conclusi);

        int somma = 0, count = 0;
        tempoMedioCompletamento(albero, &somma, &count);
        if (count > 0)
            fprintf(output, "Tempo medio: %d giorni\n", somma / count);
        else
            fprintf(output, "Tempo medio: N/D\n");

        liberaAlbero(albero);
        liberaListaRichieste(lista);
    }
}
