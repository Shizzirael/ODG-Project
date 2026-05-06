#ifndef TECNICO_H
#define TECNICO_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_STR 100

/* Definizione dell'ADT Tecnico */
typedef struct NodoTecnico {
    int codice_id;
    char nome[MAX_STR];
    char specializzazione[MAX_STR];
    bool disponibilita;
    struct NodoTecnico* next; /* Puntatore per la lista */
} Tecnico;

Tecnico* creaTecnico(int id, const char* nome, const char* specializzazione);

Tecnico* cercaTecnicoCompatibile(Tecnico* testaLista, const char* tipologiaProblema);

#endif /* TECNICO_H */