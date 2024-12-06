#pragma once

struct RGBColor
{
    int r;
    int g;
    int b;
};

struct character
{
    char c;
    character* prev;
    character* next;
};

struct textDocument
{
    character* first;
    unsigned long long charCount;
    unsigned long long cursorPos;

	// Inițializează documentul text vid.
    void init();

    // Returnează un pointer la caracterul de la poziția pos.
    character* getChar(unsigned long long pos);

	// Inserează un caracter în documentul text la poziția cursorului.
    void insertChar(char c);

	// Șterge caracterul de la poziția cursorului.
    void deleteChar();

	// Returnează numărul de linii din documentul text.
    //GRIJA CA RETURNEAZA CU INDEXARE DE LA 0
    unsigned long long getLineCount();

    // Returnează numărul liniei la care se află cursorul
    //GRIJA CA ASTA AFISEAZA CU INDEXARE DE LA 1
	unsigned long long getCursorLine(int cursorPos);

    //Returneaza lungimea liniei curente
    unsigned long long getLineLength(int cursorPos);
};

// CONSTS

const RGBColor             COLOR_BG = { 32,  32,  32 };
const RGBColor COLOR_TEXT_HIGHLIGHT = { 255, 255, 128 };
const RGBColor           COLOR_TEXT = { 255, 255, 255 };

