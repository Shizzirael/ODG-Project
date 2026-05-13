/* 
File: lucia.h (Modulo di Pianificazione)
Autore: Lucia Schettino
Matricola: NF12100025
Descrizione: Interfaccia per la gestione degli interventi
             utilizzando un Albero di Ricerca Binaria.
*/

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "richieste.h"
#include "tecnici.h"

// struttura data e ora
typedef struct {
    int anno;
    int mese;
    int giorno;
    int ora_inizio;
    int ora_fine;
} Data;

// puntatore alla struttura
typedef struct node_schedule *Schedule;

// prototipi funzioni
Schedule creaAlbero();
int verificaConflitti (Schedule root, Data target);
Schedule planIntervento(Schedule root, Richiesta req, Tecnico* tec, Data data);
void stampaStorico(Schedule root);
int cercaIntervento(Schedule root, int codiceTarget);
void generaReport(Schedule root, int* aperti, int* conclusi); 
void tempoMedioCompletamento(Schedule root, int* somma, int* count);
void trovaTecnicoPiuAttivo(Schedule root);
void interventiPerTipologia(Schedule root);
void liberaAlbero(Schedule root);

#endif