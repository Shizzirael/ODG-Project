***Casi di Test:***

**TC1**: Verifica della registrazione delle richieste   

**TC2**: Test della registrazione dei tecnici
        testa aggiungiTecnico e monitoraCarico in tecnici.c   

**TC3**: Verifica dell'assegnazione corretta  
        testa assegnaRichiesta e trovaTecnico in tecnici.c  

**TC4**: Test dell'assegnazione con fallback su tecnico GENERICO   
        testa assegnaRichiesta in tecnici.c (quando non c'è il tecnico specializzato)

**TC5**: Verifica dell'assegnazione fallita per nessun tecnico disponibile  
        testa assegnaRichiesta in tecnici.c (con lista vuota)

**TC6**: Test della pianificazione interventi senza conflitti   

**TC7**: Verifica della pianificazione con rilevamento conflitti   

**TC8**: Test dell'aggiornamento di stato con transizione valida   

**TC9**: Verifica dell'aggiornamento di stato con transizione invalida   

**TC10**: Test della ricerca per codice (richiesta trovata)   

**TC11**: Verifica della ricerca per codice (richiesta non trovata)   

**TC12**: Test dello storico degli interventi conclusi   

**TC13**: Verifica della generazione del report completo

