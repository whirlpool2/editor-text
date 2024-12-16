# editor-text
Proiect la "Introducere în programare", 2024-2025, realizat de Bereșteanu Mihail, Mihalache Ștefan, Pascaniuc Lucas

# Lucruri de făcut
- [x] Randarea unui string de text.
- [x] Structură de listă dublă-înlănțuită pentru reținerea unui document text.
- [x] Navigare pe orizontală cu săgețile.
- [x] Navigare pe verticală cu săgețile.
- [x] Navigare folosind click.
- [x] Afișarea poziției cursorului în document (numărul liniei și poziția în linie).
- [x] Schimbarea dimensiunii fontului folosind `CTRL` + `=` și `CTRL` + `-`.
- [x] Scroll vizual.
- [x] Încărcarea și salvarea fișierelor.
    - ⚠️ Nu se suportă codificări înafară de ANSI/ASCII. 
- [x] Optimizare bottomBar.
- [ ] ~~Creare structură specială pentru cursor (implică actualizarea funcțiilor)~~ (Rescrierea funcțiilor ar dura prea mult, plus s-a dat test la SD)
    - ~~Pointer la caracterul la care se află.~~
    - ~~Numărul liniei la care se află.~~
    - ~~Poziția în interiorul liniei în care se află.~~
    - ~~Poziția în document.~~
- [x] Optimizare cursorClickPos.
    - ⚠️ Deși e mult mai rapidă, funcția încă merge relativ încet la 20000 caractere, trebuie să vedem restul programului și funcțiile din SFML ce fac.
- [ ] Selecție
    - Posibilitatea de a înlocui textul selectat.
- [ ] Funcții copy, paste și cut.
- [ ] Funcții find și replace.
- [x] Meniuri
    - [ ] Deschiderea fișierelor, crearea de fișiere noi
    - [ ] Schimbarea fontului.
    - [ ] Schimbarea culorilor.
- [ ] Suportare codificărilor de tip Unicode
    - Probabil implică doar schimbarea unui tip de date în structura de caracter.
- [ ] Suport pentru _schimbarea rezoluției ferestrei_.
