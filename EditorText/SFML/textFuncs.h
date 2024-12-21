#pragma once

#include "textStructs.h"
#include <SFML/Graphics.hpp>

// Convertește structura de document text într-un string.
sf::String docToString(textDocument* doc);

// Afișează conținutul structurii textDocument în consolă.
void debugString(textDocument* doc);

// Convertește structura de document text într-un string ce conține doar liniile vizibile,
// în funcție de dimensiunea fontului și a mărimii ferestrei.
sf::String docToVisible(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject);

// Returnează distanța între 2 puncte determinate de perechi de float-uri.
float distVec2f(sf::Vector2f u, sf::Vector2f v);

// Returnează diferența în modul între 2 valori.
template <typename T>
T absDiff(T x, T y);

// Încarcă un font de la locația dată.
sf::Font loadFont(char* path);

// Setează caracteristicile fontului unui obiect text.
void setFont(sf::Text& textObject, sf::Font& font, unsigned int size, RGBColor color, char* path);

// Animează cursorul vizual în funcție de intervalul de timp dat.
void cursorAnimate(sf::RectangleShape& cursor, sf::Clock& cursorClock, bool& visible);

// Returnează poziția cursorului cea mai apropiată de locația click-ului.
void cursorClickPos(sf::Vector2i& mousePos, textDocument& doc, sf::Text textObject);

// Returnează numărul de linii care pot fi afișate în funcție de dimensiunea fontului și a ferestrei.
unsigned int visibleLineCount(sf::RenderWindow& window, sf::Text textObject);

// Actualizează obiectul text cu textul nou din document.
void updateTextObject(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject);

// Actualizează obiectul text cu textul întreg conținut în structură.
void updateWholeTextObject(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject);

// Actualizează cursorul vizual în funcție de poziția cursorului din document.
// Restartează și animația prin resetarea ceasului.
void updateCursorVisual(textDocument& doc, sf::Text& textObject, sf::RectangleShape& cursorVisual, sf::Clock& cursorClock, bool& cursorVisible);

// Inserează un caracter în obiectul text la poziția cursorului.
void insertCharInTextObject(textDocument* doc, sf::Text& textObject, char c);

// Șterge caracterul de la poziția cursorului din obiectul text.
void deleteCharInTextObject(textDocument* doc, sf::Text& textObject);

// Mută cursorul în jos pe verticală.
void moveCursorDown(textDocument& doc);

// Mută cursorul în sus pe verticală.
void moveCursorUp(textDocument& doc);

//Initial era o functie bottomBar care initializa si updata dar am separat-o in doua functii pentru a actualiza 
// cand are loc un event care actualizeaza pozitia cursorului
// Generează o bară ce afișează numărul liniei și numărul caracterului curent din aceasta.
void initializeBottomBar(sf::Text& bottomBar, sf::Font& font, unsigned int windowHeight, sf::RectangleShape& bottomBorder);

// Actualizeaza bara de jos cu numarul liniei si numarul caracterului curent din aceasta.
void updateBottomBar(sf::Text& bottomBar, textDocument& doc);

// Încarcă un fișier într-un obiect textDocument.
// Nu se setează aici și obiectul text pentru a permite schimbarea între documente pe același obiect text.
void loadFile(textDocument& doc, char* path);

// Salvează un obiect textDocument într-un fișier.
// Nu se setează aici și obiectul text pentru a permite schimbarea între documente pe același obiect text.
void saveFile(textDocument& doc, char* path);

// Dă scroll 5 linii în sus, prin schimbarea a cărei linii este prima.
void scrollUp(textDocument& doc, sf::RenderWindow& window, sf::Text& textObject);

// Dă scroll 5 linii în jos, prin schimbarea a cărei linii este prima.
void scrollDown(textDocument& doc, sf::RenderWindow& window, sf::Text& textObject);

// Functie pentru creare bara scroll
void ScrollBar(sf::Event& event, sf::RenderWindow& window, sf::RectangleShape& background, sf::RectangleShape& slider, bool& isDragged, float& scrollPos);

//Functie pentru functionarea scroll bar-ului
void makeScrollBarWork(sf::Event& event, textDocument& doc, sf::RenderWindow& window, sf::RectangleShape& background, sf::RectangleShape& slider, bool& isDragged, float& scrollPosNew, float& scrollPosCurrent, sf::Text& textObject);
unsigned long long getCursorPosFromMouse(textDocument& doc, sf::Text& text, sf::Vector2i mousePosition);