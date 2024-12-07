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
		return -1;
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

unsigned long long textDocument::getCursorLineLength()
{
	if (this->cursorPos > this->charCount)
	{
		return 0;
	}

	int lineLength = 0;
	character* p = this->getChar(this->cursorPos);

	// Pentru a afla lungimea liniei curente, ne ducem la începutul ei și numărăm caracterele până la întâlnirea unui '\n'.
	while (p->prev->c != '\n' && p != nullptr)
	{
		p = p->prev;
	}
	while (p->next->c != '\n' && p != nullptr)
	{
		lineLength++;
		p = p->next;
	}
	
	return lineLength;
}

unsigned long long textDocument::getCursorPositionInLine()
{
	unsigned long long pos = 0;
	character* p = getChar(cursorPos);

	// Mergem înapoi până la începutul liniei, numărând caracterele.
	while (p->prev->c != '\n' && p != nullptr)
	{
		p = p->prev;
		pos++;
	}
	return pos;
}

character* textDocument::getLineStart(unsigned long long line)
{
	return this->getChar(this->cursorPos - getCursorPositionInLine());
}

void textDocument::setCursorPositionInLine(unsigned int pos)
{
	this->cursorPos = this->cursorPos - getCursorPositionInLine() + pos;
}

void textDocument::gotoNextLine()
{
	character* p = this->getChar(this->cursorPos);
	while (p->c != '\n' && p != nullptr)
	{
		p = p->next;
		this->cursorPos++;
	}
	this->cursorPos++;
}

void textDocument::gotoPrevLine()
{
	character* p = this->getChar(this->cursorPos);
	while (p->c != '\n' && p != nullptr)
	{
		p = p->prev;
		this->cursorPos--;
	}
	this->cursorPos--;
}