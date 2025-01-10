#pragma once
#include <vector>
#include <string>

struct RGBColor
{
    int r;
    int g;
    int b;
};

int Max(int a, int b);

// Implementare simplă a unui generator de int-uri, nu este prea uniformă.
int randInt(int low, int high);

bool isBackgroundBright();

void randomColorScheme();

void loadColorScheme(char* path);

void saveColorScheme(char* path);

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
    unsigned long long firstVisibleLine;

	// Inițializează documentul text vid.
    void init();

    // Returnează un pointer la caracterul de la poziția pos.
    character* getChar(unsigned long long pos);

    //functie pentru a sterge un interval de text
	void deleteText(unsigned long long start, unsigned long long end);

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

    //Returneaza Lungimea Liniei de la pozitia  cursorPos
	unsigned long long getALineLength(int CursorPos);

    unsigned long long getACursorLine(int cursorpos);

    // Returnează un pointer la primul caracter al unei linii date.
	character* getLineStart(unsigned long long line);

    // Setează poziția cursorului relativ la linia curentă.
	void setCursorPositionInLine(unsigned int pos);

    // Setează poziția cursorului pe următorul '\n'.
    void gotoNextNewline();

    // Setează poziția cursorului pe precedentul '\n'.
    void gotoPrevNewline();

	//Insereaza un caracter la o anumita pozitie
    void insertCharAtPos(char c, unsigned long long pos);


    //Functie pentru a da replace la te4xt
    void replaceText(const std::string& searchText, const std::string& replaceText);

    //functie care returneaza vector cu pozitiile stringurilor cautate
    std::vector<unsigned long long> findText(const std::string& searchText);
};

// CONSTS

extern RGBColor COLOR_BG;
extern RGBColor COLOR_TEXT_HIGHLIGHT;
extern RGBColor COLOR_TEXT;

