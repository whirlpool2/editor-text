#include "textStructs.h"

void textDocument::init()
{
	this->first = nullptr;
	this->charCount = 0;
	this->cursorPos = 0;
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

unsigned long long textDocument::getCursorLine(int cursorpos)
{
	unsigned long long lineCount = 0;
	unsigned long long i = 0;
	character* p = this->first;
	if (cursorpos > this->charCount)
		return 0;

	if (p == nullptr)
	{
		return 0;
	}

	while (p->next != nullptr and cursorpos != i)
	{
		p = p->next;
		i++;


		if (p->c == '\n')
		{
			lineCount++;
		}
	}
	if (p->c != '\n' or cursorpos == this->charCount)
		lineCount++;

	return lineCount;
}

unsigned long long textDocument::getLineLength(int CursorPos) {
	if (CursorPos > this->charCount)
		return 0;
	int LineLength = 0;
	int currLine = getCursorLine(CursorPos) - 1;//functioneaza bine
	
	unsigned long long lineCount = 0;
	character* p = this->first;
	while (p != nullptr and currLine >= lineCount) {

		if (currLine == lineCount)
			LineLength++;
		if (p->c == '\n')
			lineCount++;
		p = p->next;
	}
	return LineLength - 1;
}
