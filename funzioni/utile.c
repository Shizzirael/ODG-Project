#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/utile.h"

// Svuota il buffer di stdin dopo ogni scanf.
static void flushInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


//_____________________________________________________________________________________________________
/*
Legge un intero e restituisce 1 se l'input è valido, altrimenti restituisce 0 e pulisce il buffer.
Parametri:
- dest: un puntatore a un intero dove verrà memorizzato il valore letto se l'input è valido
Ritorna:
- 1 se l'input è un intero valido, 0 altrimenti (in caso di input non valido, il buffer viene pulito per evitare problemi nei successivi input)
*/
int leggiIntero(int* dest) {
    if (scanf("%d", dest) != 1) {
        flushInput();
        return 0;
    }
    flushInput();
    return 1;
}


//_____________________________________________________________________________________________________
/*
Legge una parola da stdin, con un limite massimo di caratteri, e la memorizza in buf
Parametri:
- buf: un array di caratteri dove verrà memorizzata la parola letta, deve essere di dimensione almeno max
- max: la dimensione massima della parola da leggere, inclusa la terminazione null, deve essere maggiore di 1
Ritorna:
- Nessun valore restituito (void), ma buf viene popolato con la parola letta se l'input è valido, o con una stringa vuota se l'input non è valido
*/
void leggiParola(char* buf, int max) {
    char fmt[16];
    sprintf(fmt, "%%%ds", max - 1); // Costruisce un formato dinamico per scanf, ad esempio "%99s" per max=100
    if (scanf(fmt, buf) != 1) { // Se l'input non è valido, svuota il buffer e imposta la stringa a vuota
        buf[0] = '\0';
    }   
    flushInput();
    buf[max - 1] = '\0'; 
}


//_____________________________________________________________________________________________________
/*
Legge una riga intera da stdin, con un limite massimo di caratteri, e la memorizza in buf
Parametri:
- buf: un array di caratteri dove verrà memorizzata la riga letta, deve essere di dimensione almeno max
- max: la dimensione massima della riga da leggere, inclusa la terminazione null, deve essere maggiore di 1
Ritorna:
- Nessun valore restituito (void), ma buf viene popolato con la riga letta se l'input è valido, o con una stringa vuota se l'input non è valido
*/
void leggiRiga(char* buf, int max) {
    if (fgets(buf, max, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    buf[strlen(buf) - 1] = '\0';
}


//_____________________________________________________________________________________________________
/*
Legge una data nel formato GG/MM/AAAA e la memorizza in buf, con validazione del formato e dei valori
Parametri:
- buf: un array di caratteri dove verrà memorizzata la data letta, deve essere di dimensione almeno 11 (10 per la data + 1 per la terminazione null)
Ritorna:
- Nessun valore restituito (void), ma buf viene popolato con la data letta se l'input è valido, o con una stringa vuota se l'input non è valido
*/
void leggiData(char* buf) {
    int g, m, a;
    while (1) {
        printf("Data (GG/MM/AAAA): ");
        leggiParola(buf, 11);
        if ((strlen(buf) == 10) &&
            (sscanf(buf, "%d/%d/%d", &g, &m, &a) == 3) &&
            (g >= 1 && g <= 31) &&
            (m >= 1 && m <= 12) &&
            (a >= 1000 && a <= 9999)) {
            break;
        }
        printf(" Formato non valido o data fuori range. Riprova.\n");
    }
}


//Genera un codice univoco per le nuove richieste.
//_____________________________________________________________________________________________________
/*
Genera un codice univoco incrementale per le nuove richieste, partendo da 1 e incrementando ad ogni chiamata.
Parametri:
- Nessun parametro richiesto, la funzione mantiene un contatore interno statico che viene incrementato ad ogni chiamata
Ritorna:
- Un intero che rappresenta il codice univoco generato, incrementale ad ogni chiamata (1, 2, 3, ...)
*/
int prossimoCodice(void) {
    static int contatore = 1;
    return contatore++;
}


// Stampa un banner di benvenuto all'avvio del programma.
void stampaBanner(void) {
    printf("\n");
    printf("------------------------------------------\n");
    printf("* GESTIONE E MANUTENZIONE CONDOMINIO *\n");
    printf("------------------------------------------\n");
}