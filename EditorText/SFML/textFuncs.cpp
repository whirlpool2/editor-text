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
	std::cout << "Cursor pos is " << doc->cursorPos << std::endl;
	sf::String text = textObject.getString();
	text.insert(doc->cursorPos, c);
	textObject.setString(text);
	doc->insertChar(c);
}

void deleteCharInTextObject(textDocument* doc, sf::Text& textObject)
{
    std::cout << "Cursor pos is " << doc->cursorPos << std::endl;
	sf::String text = textObject.getString();
	text.erase(doc->cursorPos - 1, 1);
	textObject.setString(text);
	doc->deleteChar();
}

void bottomBar(sf::Text& bottomBar, int lineNumber, int linePos, sf::Font& font, unsigned int windowHeight)
{
    std::string barText = "Line: " + std::to_string(lineNumber) + " Ch: " + std::to_string(linePos);
    bottomBar.setFont(font);
    bottomBar.setString(barText);
    bottomBar.setCharacterSize(20);
    bottomBar.setPosition(575, windowHeight - 30);
    bottomBar.setFillColor(sf::Color(255, 255, 255, 100));
}

unsigned long long 