/* 
 File:        tipi.h (Modulo di supporto per richieste e tecnici)
 Autori:      Assuntapia Piccolo, Fabiana Sasso, Lucia Schettino
 Matricole:   NF12100574, NF12100245, NF12100025
 Descrizione: Dichiarazione di tipi e enumerazioni comuni utilizzati nei moduli principali del programma.
*/
#ifndef TIPI_H
#define TIPI_H

//definizione specializzazione usata come campo "tipologia" nella struct Richiesta e come campo "specializzazione" nella struct Tecnico
typedef enum {
    IDRAULICO,
    ELETTRICISTA,
    MURATORE,
    ASCENSORISTA,
    GENERICO
} Specializzazione;

//definizione dello stato di una richiesta, usato come campo "stato" nella struct Richiesta
typedef enum {
    APERTA,
    PIANIFICATA,
    IN_LAVORAZIONE,
    CONCLUSA,
    ANNULLATA
} StatoRichiesta;

//definizione dei tipi di filtro per le ricerche, usati come parametro nella funzione stampaRichiesteFiltrate in richieste.h
typedef enum {
    FILTRO_STATO    = 1,
    FILTRO_URGENZA  = 2,
    FILTRO_AREA     = 3,
    FILTRO_TECNICO  = 4,
    FILTRO_TIPOLOGIA= 5
} TipoFiltro;

#endif