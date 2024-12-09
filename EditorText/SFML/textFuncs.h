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
void cursorAnimate(sf::RectangleShape& cursor, sf::Clock& blinkInterval, bool& visible);

// Returnează poziția cursorului cea mai apropiată de locația click-ului.
unsigned long long cursorClickPos(sf::Vector2i& mousePos, textDocument& doc, sf::Text textObject);

// Returnează numărul de linii care pot fi afișate în funcție de dimensiunea fontului și a ferestrei.
unsigned int visibleLineCount(sf::RenderWindow& window, sf::Text textObject);

// Actualizează obiectul text cu textul nou din document.
void updateTextObject(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject);

// Actualizează obiectul text cu textul întreg conținut în structură.
void updateWholeTextObject(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject);

// Inserează un caracter în obiectul text la poziția cursorului.
void insertCharInTextObject(textDocument* doc, sf::Text& textObject, char c);

// Șterge caracterul de la poziția cursorului din obiectul text.
void deleteCharInTextObject(textDocument* doc, sf::Text& textObject);

// Mută cursorul în jos pe verticală.
void moveCursorDown(textDocument& doc);

// Mută cursorul în sus pe verticală.
void moveCursorUp(textDocument& doc);

// Generează o bară ce afișează numărul liniei și numărul caracterului curent din aceasta.
void bottomBar(textDocument& doc, sf::Text& bottomBar, sf::Font& font, unsigned int windowHeight);

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