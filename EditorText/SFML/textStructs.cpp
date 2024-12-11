#include "textStructs.h"

void textDocument::init()
{
    this->first = nullptr;
    this->charCount = 0;
    this->cursorPos = 0;
    this->firstVisibleLine = 0;
}

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
}

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
    this->cursorPos--;
    return;
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

void textDocument::gotoPrevNewline()
{
    character* p = this->getChar(this->cursorPos);
    bool endOfDoc = false;

	if (this->charCount == 0)
	{
		return;
	}

    // Dacă caracterul curent nu există, probabil ne aflăm la sfârșitul documentului.
    if (p == nullptr)
    {
		endOfDoc = true;
        p = this->getChar(this->cursorPos - 1);
    }
    
	// Dacă caracterul curent este '\n', îl ignorăm și trecem în interiorul liniei.
	if (p != nullptr && p->c == '\n' && !endOfDoc)
	{
		p = p->prev;
		this->cursorPos--;
	}

	// Căutăm precedentul '\n'.
	while (p != nullptr && p->c != '\n')
	{
		p = p->prev;
		this->cursorPos--;
	}

}