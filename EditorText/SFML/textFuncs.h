#pragma once

#include "textStructs.h"
#include <SFML/Graphics.hpp>

// Convertește structura de document text într-un string.
sf::String docToString(textDocument* doc);

// Convertește structura de document text într-un string ce conține doar liniile vizibile,
// în funcție de dimensiunea fontului și a mărimii ferestrei.
sf::String docToVisible(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject, unsigned int firstLine);

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
unsigned int cursorClickPos(sf::Vector2i& mousePos, textDocument& doc, sf::Text textObject);

// Returnează numărul de linii care pot fi afișate în funcție de dimensiunea fontului și a ferestrei.
unsigned int visibleLineCount(sf::RenderWindow& window, sf::Text textObject);

// Actualizează obiectul text cu textul nou din document.
void updateTextObject(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject, unsigned int firstLine);

// Inserează un caracter în obiectul text la poziția cursorului.
void insertCharInTextObject(textDocument* doc, sf::Text& textObject, char c);

// Șterge caracterul de la poziția cursorului din obiectul text.
void deleteCharInTextObject(textDocument* doc, sf::Text& textObject);