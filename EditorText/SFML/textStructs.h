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
    unsigned long long getLineCount();

	// Returnează numărul liniei la care se află cursorul (indexat de la 0).
	unsigned long long getCursorLine();

	// Returnează poziția cursorului în linia curentă (indexată de la 0).
	unsigned long long getCursorPositionInLine();

    // Returnează lungimea liniei curente.
    unsigned long long getCursorLineLength();

    // Returnează un pointer la primul caracter al unei linii date.
	character* getLineStart(unsigned long long line);

    // Setează poziția cursorului relativ la linia curentă.
	void setCursorPositionInLine(unsigned int pos);

    // Setează poziția cursorului pe primul caracter al următoarei linii.
    void gotoNextLine();

    // Setează poziția cursorului pe primul caracter al liniei precedente.
    void gotoPrevLine();
};

// CONSTS

const RGBColor             COLOR_BG = { 32,  32,  32 };
const RGBColor COLOR_TEXT_HIGHLIGHT = { 255, 255, 128 };
const RGBColor           COLOR_TEXT = { 255, 255, 255 };

