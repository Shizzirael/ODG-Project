#ifndef TIPI_H
#define TIPI_H

typedef enum {
    IDRAULICO,
    ELETTRICISTA,
    MURATORE,
    ASCENSORISTA,
    GENERICO
} Specializzazione;

typedef enum {
    APERTA,
    PIANIFICATA,
    IN_LAVORAZIONE,
    CONCLUSA,
    ANNULLATA
} StatoRichiesta;

typedef enum {
    FILTRO_STATO    = 1,
    FILTRO_URGENZA  = 2,
    FILTRO_AREA     = 3,
    FILTRO_TECNICO  = 4,
    FILTRO_TIPOLOGIA= 5
} TipoFiltro;

#endif