/* 
File: lucia.h (Modulo di Pianificazione)
Autore: Lucia Schettino
Matricola: NF12100025
Descrizione: Interfaccia per la gestione degli interventi
             utilizzando un Albero di Ricerca Binaria.
*/

#ifndef LUCIA_H
#define LUCIA_H

#include "richieste.h"
#include "tecnici.h"

// struttura data e ora
typedef struct {
    int anno;
    int mese;
    int giorno;
    int ora;
} Data;

// puntatore alla struttura
typedef struct node_schedule *Schedule;

// prototipi funzioni
Schedule creaAlbero();
int verificaConflitti (Schedule root, Data target);
Schedule planIntervento(Schedule root, Richiesta req, Tecnico* tec, Data data);
void stampaStorico(Schedule root);
int cercaPerCodice(Schedule root, int codiceTarget);
void generaReport(Schedule root, int* aperti, int* conclusi); //sempre se assia li chiama aperti e conclusi

#endif