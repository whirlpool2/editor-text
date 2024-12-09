#define _CRT_SECURE_NO_WARNINGS
#include "textFuncs.h"
#include <iostream>

sf::String docToString(textDocument* doc)
{
	sf::String result = "";

	character* p = doc->first;
	while (p != nullptr)
    {
        result += p->c;
		p = p->next;
    }

    return result;
}

void debugString(textDocument* doc)
{
    character* p = doc->first;
    while (p != nullptr)
    {
		if (p->c == '\n')
		{
			std::cout << "\\n";
		}
		else
		{
			std::cout << p->c;
		}
        p = p->next;
    }
    
    std::cout << std::endl;

	for (int i = 0; i < doc->cursorPos; i++)
	{
		if (doc->getChar(i)!= nullptr && doc->getChar(i)->c == '\n')
		{
			std::cout << " ";
		}
		std::cout << " ";
	}
    std::cout << "^ (" << doc->cursorPos << ")" << std::endl;
}

sf::String docToVisible(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject, unsigned int firstLine)
{
	sf::String result = "";
    unsigned int lineCount = 0;

	character* p = doc->getChar(0);
	while (p != nullptr)
    {
		if (lineCount >= firstLine + visibleLineCount(window, textObject))
        {
            break;
        }
        if (lineCount >= firstLine)
        {
            result += p->c;
        }
        if (p->c == '\n')
        {
            lineCount++;
        }
		p = p->next;
    }

    return result;
}

float distVec2f(sf::Vector2f u, sf::Vector2f v)
{
    sf::Vector2f d = u - v;
    return sqrt(d.x * d.x + d.y * d.y);
}

template <typename T>
T absDiff(T x, T y)
{
    return x > y ? x - y : y - x;
}

sf::Font loadFont(char* path)
{
    sf::Font font;

    unsigned int pathLength = 0;
    while (pathLength < 256 && path[pathLength] != '\0')
    {
        pathLength++;
    }

    if (pathLength >= 256)
    {
        std::cerr << "Font path is invalid.\n";
        font.loadFromFile("Fonts/CascadiaMono.ttf");
        return font;
    }

    if (!(font.loadFromFile(path))) // Dacă nu găsim fontul, incărcăm fontul inclus cu proiectul.
    {
        std::cerr << "Font " << path << " could not be found.\n";
        font.loadFromFile("Fonts/CascadiaMono.ttf");
        return font;
    }

    return font;
}

void setFont(sf::Text& textObject, sf::Font& font, unsigned int size, RGBColor color, char* path)
{
    font = loadFont(path);
    textObject.setFont(font);
    textObject.setCharacterSize(size);
    textObject.setFillColor(sf::Color(color.r, color.g, color.b));
    textObject.setPosition(10, 10);
}

void cursorAnimate(sf::RectangleShape& cursor, sf::Clock& blinkInterval, bool& visible)
{
    //in esteVizibil retin daca afisam cursor care are forma de dreptunghi , daca estevizibil dau culoare alb , altfel fac transparent
    if (blinkInterval.getElapsedTime().asMilliseconds() > 500)
    {
        visible = 1 - visible; // Alternăm vizibilitatea după 500 milisecunde.
        blinkInterval.restart();
    }

    if (visible)
    {
        cursor.setFillColor(sf::Color(COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b));
    }
    else
    {
        cursor.setFillColor(sf::Color::Transparent);
    }
}

unsigned long long cursorClickPos(sf::Vector2i& mousePos, textDocument& doc, sf::Text textObject)
{
    sf::Vector2f mousePosFloat = { mousePos.x * 1.0f, mousePos.y * 1.0f };
    sf::String text = textObject.getString();
    int fontSize = textObject.getCharacterSize();

    // Verificăm fiecare caracter dacă se află la cel mult o dimensiune de caracter distanță de click.
    for (unsigned int i = 0; i < text.getSize(); i++)
    {
        sf::Vector2f charPos = textObject.findCharacterPos(i);
        if (distVec2f(charPos, mousePosFloat) <= fontSize)
        {
            return i + 1;
        }
        if (absDiff(mousePosFloat.y, charPos.y) <= fontSize && text[i] == '\n')
        {
            return i;
        }
    }
    return text.getSize();
}

unsigned int visibleLineCount(sf::RenderWindow& window, sf::Text textObject)
{
    const sf::Font* font = textObject.getFont();
	return window.getSize().y / font->getLineSpacing(textObject.getCharacterSize());
}

void updateTextObject(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject, unsigned int firstLine)
{
    sf::String visibleLines = docToVisible(doc, window, textObject, firstLine);
    textObject.setString(visibleLines);
}

void insertCharInTextObject(textDocument* doc, sf::Text& textObject, char c)
{
    // std::cout << "DEBUG: Cursor pos is " << doc->cursorPos << " (relative to line: " << doc->getCursorPositionInLine() << ")" << std::endl;
    sf::String text = textObject.getString();
    text.insert(doc->cursorPos, c);
    textObject.setString(text);
    doc->insertChar(c);
}

void deleteCharInTextObject(textDocument* doc, sf::Text& textObject)
{
    // std::cout << "DEBUG: Cursor pos is " << doc->cursorPos << " (relative to line: " << doc->getCursorPositionInLine() << ")" << std::endl;
    sf::String text = textObject.getString();
    text.erase(doc->cursorPos - 1, 1);
    textObject.setString(text);
    doc->deleteChar();
}

void bottomBar(textDocument& doc, sf::Text& bottomBar, sf::Font& font, unsigned int windowHeight)
{
    std::string barText = "Line: " + std::to_string(doc.getCursorLine()) + " Ch: " + std::to_string(doc.getCursorPositionInLine());
    bottomBar.setFont(font);
    bottomBar.setString(barText);
    bottomBar.setCharacterSize(20);
    bottomBar.setPosition(575, windowHeight - 30);
    bottomBar.setFillColor(sf::Color(255, 255, 255, 100));
}

void loadFile(textDocument& doc, char* path)
{
	FILE* file = fopen(path, "r");
	if (file == nullptr)
	{
		std::cerr << "Fisierul " << path << " nu a fost gasit.\n";
		return;
	}
	doc.init();
	char c;
	while ((c = fgetc(file)) != EOF)
	{
		doc.insertChar(c);
	}
	fclose(file);
}

void saveFile(textDocument& doc, char* path)
{
	FILE* file = fopen(path, "w");
	if (file == nullptr)
	{
		std::cerr << "Fisierul " << path << " nu a putut fi deschis pentru scriere.\n";
		return;
	}
	character* p = doc.first;
	while (p != nullptr)
	{
		fputc(p->c, file);
		p = p->next;
	}
	fclose(file);
}

void moveCursorDown(textDocument& doc)
{
    if (doc.cursorPos >= doc.charCount)
    {
        return;
    }

    unsigned long long linePos = doc.getCursorPositionInLine();

    // Efectuăm toate acestea doar dacă NU suntem la sfârșitul documentului.
    if (doc.getChar(doc.cursorPos) != nullptr)
    {
        // În cazul în care ne aflăm la sfârșitul unei linii, ne aflăm deja unde trebuie.
        // În caz contrar, trebuie să ajungem la sfârșitul ei.
        if (doc.getChar(doc.cursorPos)->c != '\n')
        {
            doc.gotoNextNewline();
        }

        // Dacă linie de pe care venim este mai lungă decât cea pe care mergem,
        // considerăm poziția nouă a fi pe ultimul caracter al liniei.
        if (linePos > doc.getCursorLineLength())
        {
            linePos = doc.getCursorLineLength();
        }

        // Ne deplasăm până la poziția echivalentă, sau până terminăm linia.
        for (int i = 0; i < linePos + 1; i++) // +1 pentru a sări '\n'-ul.
        {
            doc.cursorPos++;
            if (doc.getChar(doc.cursorPos) == nullptr || doc.getChar(doc.cursorPos)->c == '\n')
            {
                break;
            }
        }
    }
}

void moveCursorUp(textDocument& doc)
{
    if (doc.cursorPos <= 0)
    {
        return;
    }

    unsigned long long linePos = doc.getCursorPositionInLine();

    // Dacă cursorul este la sfârșitul documentului, plasăm poziția lui pe ultimul caracter.
    if (doc.getChar(doc.cursorPos) == nullptr)
    {
        doc.cursorPos--;
    }

    // Avem două '\n', unul ce marchează începutul liniei curente, și unul ce marchează
    // începutul liniei precedente.
    doc.gotoPrevNewline();
    doc.gotoPrevNewline();

    // Ne deplasăm până la poziția echivalentă, sau până terminăm linia.
    for (int i = 0; i <= linePos; i++)
    {
        doc.cursorPos++;
        if (doc.getChar(doc.cursorPos)->c == '\n')
        {
            break;
        }
    }
}

void scrollUp(unsigned int& firstLine) {
    if (firstLine > 0)
        firstLine--;
}

void scrollDown(unsigned int& firstLine, textDocument* doc, sf::RenderWindow& window, sf::Text& textCurent) {
    unsigned int maxLines = visibleLineCount(window, textCurent);
    unsigned int totalLines = 0;

    character* p = doc->getChar(0);
    while (p != nullptr) {
        if (p->c == '\n') {
            totalLines++;
        }
        p = p->next;
    }
    if (firstLine + maxLines < totalLines) {
        firstLine++;
    }
}