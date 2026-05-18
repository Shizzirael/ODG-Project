/* 
 File:        utile.h (Modulo di supporto per il programma principale)
 Autori:      Assuntapia Piccolo,Fabiana Sasso, Lucia Schettino
 Matricole:   NF12100574, NF12100245, NF12100025 
 Descrizione: Funzioni di utilità per la gestione dell'input e altre operazioni comuni, 
              utilizzate nei moduli principali del programma.
*/
#ifndef UTILE_H
#define UTILE_H

/* Funzione:flushInput
Pulisce il buffer di input, rimuovendo eventuali caratteri residui dopo una lettura fallita 
o per preparare il buffer per la prossima lettura.
*/
static void flushInput(void);

/*____________________________________________________________________________________________________
Funzione:leggiIntero
Legge un intero da stdin e lo memorizza in dest, restituendo 1 se l'input è valido o 0 altrimenti. 
In caso di input non valido, il buffer viene pulito per evitare problemi nei successivi input.

Parametri:
- dest: un puntatore a un intero dove verrà memorizzato il valore letto se l'input è valido

Pre-condizioni:
- dest deve essere un puntatore valido a un intero

Post-condizioni:
- Se l'input è un intero valido, dest viene aggiornato con il valore letto e la funzione restituisce 1.
- Se l'input non è un intero valido, dest non viene modificato, il buffer viene pulito e la funzione restituisce 0.

Ritorna:
- 1 se l'input è un intero valido, 
- 0 altrimenti (in caso di input non valido, il buffer viene pulito per evitare problemi nei successivi input)
*/
static int leggiIntero(int* dest);

/*____________________________________________________________________________________________________
Funzione:leggiParola
Legge una parola da stdin, con un limite massimo di caratteri, e la memorizza in buf.

Parametri:
- buf: un array di caratteri dove verrà memorizzata la parola letta
- max: la dimensione massima della parola da leggere, inclusa la terminazione null

Pre-condizioni:
- buf deve essere un array di caratteri valido con una dimensione di almeno max
- max deve essere un intero maggiore di 1

Post-condizioni:
- Se l'input è una parola valida, buf viene popolato con la parola letta e terminato con null.
- Se l'input non è una parola valida, buf viene impostato a una stringa vuota e terminato con null.

Ritorna:
- Nessun valore restituito (void), ma buf viene popolato con la parola letta se l'input è valido, 
o con una stringa vuota se l'input non è valido
*/
static void leggiParola(char* buf, int max);

/*____________________________________________________________________________________________________
Funzione:LeggiRiga
Legge una riga intera da stdin, con un limite massimo di caratteri, e la memorizza in buf.

Parametri:
- buf: un array di caratteri dove verrà memorizzata la riga letta
- max: la dimensione massima della riga da leggere, inclusa la terminazione null

Pre-condizioni:
- buf deve essere un array di caratteri valido con una dimensione di almeno max
- max deve essere un intero maggiore di 1

Post-condizioni:
- Se l'input è una riga valida, buf viene popolato con la riga letta e terminato con null.
- Se l'input non è una riga valida, buf viene impostato a una stringa vuota e terminato con null.

Ritorna:
- Nessun valore restituito (void), ma buf viene popolato con la riga letta se l'input è valido,
o con una stringa vuota se l'input non è valido
*/
static void leggiRiga(char* buf, int max);

/*____________________________________________________________________________________________________
Funzione:leggiData
Legge una data da stdin in formato "dd/mm/yyyy" e la memorizza in buf.

Parametri:
- buf: un array di caratteri dove verrà memorizzata la data letta

Pre-condizioni:
- buf deve essere un array di caratteri valido con una dimensione sufficiente per contenere una data in formato "dd/mm/yyyy" 
(almeno 11 caratteri inclusa la terminazione null)

Post-condizioni:
- Se l'input è una data valida in formato "dd/mm/yyyy", buf viene popolato con la data letta e terminato con null.
- Se l'input non è una data valida, buf viene impostato a una stringa vuota e terminato con null.

Ritorna:
- Nessun valore restituito (void), ma buf viene popolato con la data letta se l'input è valido,
o con una stringa vuota se l'input non è valido
*/
static void leggiData(char* buf);

/*____________________________________________________________________________________________________
Funzione:prossimoCodice
Genera un nuovo codice ID univoco per una richiesta, incrementando un contatore statico interno.

Parametri:
- Nessun parametro

Pre-condizioni:
- Nessuna pre-condizione specifica

Post-condizioni:
- Viene restituito un intero che rappresenta il nuovo codice ID univoco, incrementato rispetto al precedente ad ogni chiamata della funzione. 
Il primo codice generato sarà 1, il secondo 2, e così via.

Ritorna:
- Un intero che rappresenta il nuovo codice ID univoco generato, incrementato ad ogni chiamata della funzione (1, 2, 3, ...)
*/
static int prossimoCodice(void);

/*____________________________________________________________________________________________________
Funzione:stampaBanner
Stampa un banner di benvenuto sullo schermo.
*/
static void stampaBanner(void);


#endif