# Gestione Manutenzione Condominio

Progetto per il corso di Programmazione e Strutture Dati — A.A. 2025/2026

## Il gruppo

- Assuntapia Piccolo (NF12100574) — richieste e priority queue
- Fabiana Sasso (NF12100245) — tecnici e lista concatenata
- Lucia Schettino (NF12100025) — schedule e BST

## Come compilare

```bash
make        # compila il programma principale
make test   # compila i test
make clean  # pulizia
```

## Come funziona

Il programma gestisce le richieste di manutenzione di un condominio: si possono inserire richieste, assegnarle a un tecnico, pianificare l'intervento e generare report.

Abbiamo usato tre strutture dati:
- **Lista concatenata** per i tecnici
- **Max-Heap** per gestire le richieste in ordine di urgenza
- **BST** per la pianificazione degli interventi (ordinati per data, con rilevamento conflitti di orario)

## Test

```bash
# singolo test
./test TC1 tests/TC1_input.txt tests/TC1_oracle.txt tests/TC1_output.txt

# tutta la suite
./test --suite tests/test_suite.txt tests/result.txt
```

I test confrontano l'output del programma con un oracolo riga per riga e scrivono PASS/FAIL su result.txt.

## Note

Sviluppato e testato su Linux, standard C99.
