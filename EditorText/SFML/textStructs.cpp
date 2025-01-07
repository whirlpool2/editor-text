#include "textStructs.h"
#include <iostream>
void textDocument::init()
{
    this->first = nullptr;
    this->charCount = 0;
    this->cursorPos = 0;
    this->firstVisibleLine = 0;
}
/*
character* textDocument::getChar(unsigned long long pos)
{
    if (pos >= this->charCount)
    {
        return nullptr;
    }
    int i = 0;
    character* p = this->first;
    while (i < pos)
    {
        p = p->next;
        i++;
    }
    return p;
}*/

character* textDocument::getChar(unsigned long long pos)
    {
        if (this->first == nullptr || pos >= this->charCount)
            return nullptr;
      
        unsigned long long i = 0;
        character* p = this->first;
        while (p != nullptr && i < pos)
        {
            p = p->next;
            i++;
        }
        return p;
    }
/*
character* textDocument::getChar(unsigned long long pos)
{
    if (pos >= this->charCount)
    {
        return nullptr;
    }

    if (this->first == nullptr)
    {
        return nullptr;
    }

	// Vede daca cursorul e mai aproape de inceput sau de pozitia data
    unsigned long long distanceFromStart = pos;
    unsigned long long distanceFromCursor = (this->cursorPos > pos) ? (this->cursorPos - pos) : (pos - this->cursorPos);

    character* p = nullptr;
    unsigned long long currentPos = 0;

    // Iar aici verificam in care caz ne incadram
    if (distanceFromStart <= distanceFromCursor)
    {
        p = this->first;
        currentPos = 0;
    }
    else
    {
        // In else incepem de la pozitia cursorului
        // De la 0 la pozitia cursorului
        p = this->first;
        currentPos = 0;
        while (currentPos < this->cursorPos)
        {
            p = p->next;
            currentPos++;
        }
    }

    if (currentPos <= pos)
    {
		// Mergem inainte
        while (currentPos < pos)
        {
            p = p->next;
            currentPos++;
        }
    }
    else
    {
        // Ne intoarcem
        while (currentPos > pos)
        {
            p = p->prev;
            currentPos--;
        }
    }

    return p;
}

*/
void textDocument::insertChar(char c)
{
    character* p = new character;
    p->c = c;
    p->prev = nullptr;
    p->next = nullptr;
    if (this->first == nullptr)
    {
        this->first = p;
        this->charCount++;
        this->cursorPos++;
        return;
    }
    if (this->cursorPos == 0)
    {
        p->next = this->first;
        this->first->prev = p;
        this->first = p;
        this->charCount++;
        this->cursorPos++;
        return;
    }
    character* q = this->getChar(this->cursorPos - 1);
    p->next = q->next;
    p->prev = q;
    if (q->next != nullptr)
    {
        q->next->prev = p;
    }
    q->next = p;
    this->charCount++;
    this->cursorPos++;
    return;
}

void textDocument::deleteChar()
{
    if (this->cursorPos == 0)
    {
        return;
    }
    unsigned long long originalPos = this->cursorPos;

    character* p = this->getChar(this->cursorPos - 1);
    if (p->prev != nullptr)
    {
        p->prev->next = p->next;
    }
    if (p->next != nullptr)
    {
        p->next->prev = p->prev;
    }
    if (p == this->first)
    {
        this->first = p->next;
    }
    delete p;
    this->charCount--;
    this->cursorPos = originalPos - 1;
}

unsigned long long textDocument::getLineCount()
{
    unsigned long long lineCount = 0;
    character* p = this->first;
    while (p != nullptr)
    {
        if (p->c == '\n')
        {
            lineCount++;
        }
        p = p->next;
    }
    return lineCount;
}


unsigned long long textDocument::getACursorLine(int cursorpos)
{
    unsigned long long lineCount = 0;
    unsigned long long i = 0;
    character* p = this->first;
    if (cursorpos > this->charCount)
        return 0;

    if (p == nullptr)
        return 0;
    

    while (p->next != nullptr and cursorpos != i)
    {
        p = p->next;
        i++;

        if (p->c == '\n')
            lineCount++;
        
    }
    if (p->c != '\n' or cursorpos == this->charCount)
        lineCount++;

    return lineCount;
}

unsigned long long textDocument::getCursorLine()
{
	unsigned long long count = 0;

	if (this->charCount == 0)
	{
		return 0;
	}

	character* p = getChar(cursorPos);

	if (p == nullptr)
	{
		return this->getLineCount();
	}

	while (p->prev != nullptr)
	{
		p = p->prev;
		if (p->c == '\n')
		{
			count++;
		}
	}

	return count;
}

unsigned long long textDocument ::getALineLength(int CursorPos) {
    if (CursorPos > this->charCount)
        return 0;
    int LineLength = 0;
    int currLine = getACursorLine(CursorPos) - 1;//functioneaza bine

    unsigned long long lineCount = 0;
    character* p = this->first;
    if (p->next == nullptr)
        return 1;
    while (p != nullptr and currLine >= lineCount) {

        if (currLine == lineCount)
            LineLength++;
        if (p->c == '\n')
            lineCount++;
        p = p->next;
    }
    return LineLength - 1;

}

void textDocument::deleteText(unsigned long long start, unsigned long long end) {
    //Verific un bug, sterg mai incol
    if (start > end)
        std::swap(start, end);

    //verific daca intervalul e valid
    if (start >= this->charCount || end > this->charCount)
        return;

    unsigned long long initialPos = this->cursorPos;
    character* startchar = getChar(start);
    character* endchar = getChar(end);
    //daca nu exista caracterul de startatunci iesim, pentru end nu verificam
    // pntru ca functia getChar returneaza nullptr pentru ultima pozitie
    if (startchar == nullptr)
        return;
    //std::cout << "NICI MACAR NU INTRU AICI" << std::endl;
    
    //fac legatura dupa stergere
    //daca nu incepem de la inceput, mergem la sfaristul selectiei
    if (startchar->prev != nullptr) {
        startchar->prev->next = endchar;
    }
	//daca incepem de la inceput atunci lua mpointerul first
    else
        this->first = endchar;

	//daca nu suntem la sfarsitul documentului facem legatura
    if(endchar != nullptr)
		endchar->prev = startchar->prev;
    
    
	//ca sa evitam memory leaks stergem caracterele dintre start si end
    character* current = startchar;
    while (current != endchar) {
        character* next = current->next;
        delete current;
        current = next;
        this->charCount--;
    }
	if (initialPos >= end)
		this->cursorPos = initialPos - (end - start);
	else if (initialPos >= start)
		this->cursorPos = start;
	else
		this->cursorPos = initialPos;

    //if (startchar->prev != nullptr)
    //    startchar->prev->next = endchar;
    //if (endchar != nullptr && endchar->next != nullptr)
    //    q->next->prev = p->prev;

    //if (p == this->first)
    //    this->first = q;

    //while (p != q) {
    //    character* aux = p;
    //    p = p->next;
    //    delete aux;
    //    this->charCount--;
    //}

    //// Ajustez pozitie cursor
    //if (originalPos >= end) {
    //    this->cursorPos = originalPos - (end - start);
    //}
    //else if (originalPos >= start) {
    //    this->cursorPos = start;
    //}
    //else {
    //    this->cursorPos = originalPos;
    //}
}

unsigned long long textDocument::getCursorLineLength()
{
    unsigned long long originalPos = this->cursorPos;
    unsigned long long lineStartPos;
    unsigned long long lineEndPos;

	this->gotoPrevNewline();
	lineStartPos = this->cursorPos;

	this->gotoNextNewline();
	lineEndPos = this->cursorPos;

	this->cursorPos = originalPos;

	return lineEndPos - lineStartPos;
}

unsigned long long textDocument::getCursorPositionInLine()
{
    unsigned long long originalPos = this->cursorPos;
    unsigned long long pos = 0;
	character* p = this->getChar(this->cursorPos);

    if (p == nullptr)
    {
        pos++;
    }

    this->gotoPrevNewline();
    pos += originalPos - this->cursorPos;
	this->cursorPos = originalPos;

	return pos - 1; // -1 deoarece indexăm de la 0.

}

character* textDocument::getLineStart(unsigned long long line)
{
	return this->getChar(this->cursorPos - getCursorPositionInLine());
}

void textDocument::setCursorPositionInLine(unsigned int pos)
{
	if (pos > this->getCursorLineLength())
	{
		pos = this->getCursorLineLength();
	}
	this->cursorPos = this->cursorPos - this->getCursorPositionInLine() + pos;
}

void textDocument::gotoNextNewline()
{
    character* p = this->getChar(this->cursorPos);

	// Dacă caracterul curent nu există, probabil nu există niciun caracter în document
	// sau ne aflăm la sfârșitul documentului.
	if (p == nullptr)
	{
		return;
	}

	// Dacă caracterul curent este '\n', îl ignorăm și trecem în interiorul liniei.
	if (p->c == '\n')
	{
		p = p->next;
		this->cursorPos++;
	}

	// Căutăm următorul '\n'.
    while (p != nullptr && p->c != '\n')
    {
        p = p->next;
        this->cursorPos++;
    }
}

void textDocument::gotoPrevNewline() {
    character* p = this->getChar(this->cursorPos);
    bool endOfDoc = false;

    if (p == nullptr) {
        endOfDoc = true;
        p = this->getChar(this->cursorPos - 1);
    }

    if (p != nullptr && p->c == '\n' && !endOfDoc) {
        p = p->prev;
        this->cursorPos--;
    }

    while (p != nullptr && p->c != '\n') {
        p = p->prev;
        this->cursorPos--;

        if (p == nullptr) {
            break;
        }
    }
}
