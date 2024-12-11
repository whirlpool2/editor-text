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

sf::String docToVisible(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject)
{
	sf::String result = "";
    unsigned long long lineCount = 0;
	unsigned long long maximumVisibleLines = visibleLineCount(window, textObject);

	character* p = doc->first;
	while (p != nullptr)
    {
		if (lineCount >= doc->firstVisibleLine + maximumVisibleLines)
        {
            break;
        }
        if (lineCount >= doc->firstVisibleLine)
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

void cursorClickPos(sf::Vector2i& mousePos, textDocument& doc, sf::Text textObject)
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
            doc.cursorPos = i + 1;
            return;
        }
        if (absDiff(mousePosFloat.y, charPos.y) <= fontSize && text[i] == '\n')
        {
            doc.cursorPos = i;
            return;
        }
    }
    doc.cursorPos = doc.charCount;
}

unsigned int visibleLineCount(sf::RenderWindow& window, sf::Text textObject)
{
    const sf::Font* font = textObject.getFont();
	return window.getSize().y / font->getLineSpacing(textObject.getCharacterSize());
}

void updateTextObject(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject)
{
    sf::String visibleLines = docToVisible(doc, window, textObject);
    textObject.setString(visibleLines);
}

void updateWholeTextObject(textDocument* doc, sf::RenderWindow& window, sf::Text& textObject)
{
    sf::String text = docToString(doc);
    textObject.setString(text);
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

//Am optimizat functia bottomBar, folosindu-ma de reactualizarea pozitiei numai cand are loc un event
void initializeBottomBar(sf::Text& bottomBar, sf::Font& font, unsigned int windowHeight, sf::RectangleShape& bottomBorder)
{
	bottomBar.setFont(font);
	bottomBar.setCharacterSize(16);
    bottomBar.setFillColor(sf::Color::White);
	

    bottomBar.setOrigin(bottomBar.getGlobalBounds().width, bottomBar.getGlobalBounds().height);
	bottomBar.setPosition(windowHeight - 10, windowHeight - 5);

	bottomBorder.setSize(sf::Vector2f(windowHeight - 20, 20));
	bottomBorder.setFillColor(sf::Color::Black);
	bottomBorder.setPosition(0, windowHeight - 20);
}

void updateBottomBar(sf::Text& bottomBar, textDocument& doc)
{
	bottomBar.setString("Line: " + std::to_string(doc.getCursorLine()) + " | Char: " + std::to_string(doc.getCursorPositionInLine()));
	bottomBar.setOrigin(bottomBar.getGlobalBounds().width + 40, bottomBar.getGlobalBounds().height);
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

void scrollUp(textDocument& doc, sf::RenderWindow& window, sf::Text& textObject)
{
    // O mică problemă cu aceste funcții, poziția cursorului nu mai este corectă dacă
	// trunchiem string-ul la documentul vizibil. Astfel, orice operație pe text nu mai
    // este corectă vizual.

    /*
	if (doc.firstVisibleLine > 5)
	{
		doc.firstVisibleLine -= 5;
	}
	else
	{
		doc.firstVisibleLine = 0;
	}
    */

    float lineSpacing = textObject.getLineSpacing() * textObject.getCharacterSize();
    float verticalOffset = 5 * lineSpacing;

    textObject.move(0, verticalOffset);

}

void scrollDown(textDocument& doc, sf::RenderWindow& window, sf::Text& textObject)
{
    /*
	if (doc.firstVisibleLine < doc.getLineCount() - 5)
	{
		doc.firstVisibleLine += 5;
	}
	else
	{
		doc.firstVisibleLine = doc.getLineCount() - visibleLineCount(window, textObject);
	}
    */

    float lineSpacing = textObject.getLineSpacing() * textObject.getCharacterSize();
    float verticalOffset = 5 * lineSpacing;

    textObject.move(0, -verticalOffset);
}

// Implementează scrollbar-ul pentru text.
void ScrollBar(sf::Event& event, sf::RenderWindow& window, sf::RectangleShape& background, sf::RectangleShape& slider, bool& isDragged, float& scrollPos)
{
    background.setSize(sf::Vector2f(20, 800));
    background.setPosition(780, 0);
    background.setFillColor(sf::Color(200, 200, 200));

    slider.setSize(sf::Vector2f(20, 50));
    slider.setPosition(780, scrollPos * (background.getSize().y - slider.getSize().y));
    slider.setFillColor(sf::Color(100, 100, 100));

    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (slider.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            isDragged = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        isDragged = false;
    }
    else if (event.type == sf::Event::MouseMoved && isDragged)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float newY = mousePos.y - (slider.getSize().y / 2.0f);
        float backgroundTop = background.getPosition().y;
        float backgroundBottom = background.getPosition().y + background.getSize().y - slider.getSize().y;

        if (newY < backgroundTop)
        {
            newY = backgroundTop;
        }
        if (newY > backgroundBottom)
        {
            newY = backgroundBottom;
        }

        slider.setPosition(slider.getPosition().x, newY);
        float scrollLimit = background.getSize().y - slider.getSize().y;
        scrollPos = (slider.getPosition().y - backgroundTop) / scrollLimit;
    }
}
