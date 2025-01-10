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

void cursorAnimate(sf::RectangleShape& cursor, sf::Clock& cursorClock, bool& visible)
{
    //in esteVizibil retin daca afisam cursor care are forma de dreptunghi , daca estevizibil dau culoare alb , altfel fac transparent
    if (cursorClock.getElapsedTime().asMilliseconds() > 500)
    {
        visible = 1 - visible; // Alternăm vizibilitatea după 500 milisecunde.
        cursorClock.restart();
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
    int fontSize = textObject.getCharacterSize(); // -2 pentru a îmbunătăți precizia.

    // Verifică dacă am dat click pe primul caracter.
    if (mousePosFloat.x <= textObject.findCharacterPos(0).x && mousePosFloat.y <= textObject.findCharacterPos(0).y + fontSize)
    {
        doc.cursorPos = 0;
        return;
    }

    // Verificăm dacă click-ul este mai jos de text.
    if (mousePosFloat.y >= textObject.findCharacterPos(doc.charCount - 1).y + fontSize)
    {
        doc.cursorPos = doc.charCount;
        return;
    }
   
    /*
    Folosim 2 căutări binare.
    
    Prima căutare binară va amplasa cursorul pe un caracter din linia pe care se află click-ul.
    În cazul în care click-ul este mai jos decât ultimul caracter din linie, cursorul va fi plasat pe ultimul caracter.
    În cazul în s-a dat click pe o linie, dar click-ul este mai în dreapta decât ultimul caracter, cursorul va fi plasat pe ultimul caracter.
    În rest, putem să trecem la următoarea căutare binară.

    A doua căutare binară va amplasa cursorul pe caracterul cel mai apropiat de click.
    Limitele acestei căutări sunt primul și ultimul caracter din linie, aflate prin mutarea cursorului între '\n'-uri.
    */

    long long left = 0;
    long long right = doc.charCount - 1;
    while (left <= right)
    {
        long long mid = (left + right) / 2;
        sf::Vector2f charPos = textObject.findCharacterPos(mid);
        if (mousePosFloat.y >= charPos.y && mousePosFloat.y <= charPos.y + fontSize)
        {
            doc.cursorPos = mid;
            break;
        }
        if (charPos.y < mousePosFloat.y)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    // Dacă ne aflăm pe prima linie, nu există niciun '\n' înainte de cursor.
    // Atunci, ne ducem pe primul caracter.
    if (doc.getCursorLine() == 0)
    {
        doc.cursorPos = 0;
    }
    else
    {
        doc.gotoPrevNewline();
    }
    left = doc.cursorPos;
    doc.gotoNextNewline();
    right = doc.cursorPos;
    
    if (textObject.findCharacterPos(doc.cursorPos).x + fontSize <= mousePosFloat.x)
    {
        return;
    }

    while (left <= right)
    {
        long long mid = (left + right) / 2;
        sf::Vector2f charPos = textObject.findCharacterPos(mid);
        if (mousePosFloat.x >= charPos.x + 2 && mousePosFloat.x <= charPos.x + fontSize - 2)
        {
            doc.cursorPos = mid + 1; // +1 pentru că altfel am fi pe caracterul din stânga. (Trebuie găsită o explicație mai bună)
            return;
        }
        if (charPos.x < mousePosFloat.x)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
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

void updateCursorVisual(textDocument& doc, sf::Text& textObject, sf::RectangleShape& cursorVisual, sf::Clock& cursorClock, bool& cursorVisible)
{
	sf::String text = textObject.getString();
	int fontSize = textObject.getCharacterSize();
	sf::Vector2f charPos = textObject.findCharacterPos(doc.cursorPos);
	cursorVisual.setPosition(charPos.x, charPos.y);
	cursorClock.restart();
    cursorVisible = true;
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
    if(!text.isEmpty())
    text.erase(doc->cursorPos - 1, 1);
    textObject.setString(text);
    doc->deleteChar();
}

//Am optimizat functia bottomBar, folosindu-ma de reactualizarea pozitiei numai cand are loc un event
void initializeBottomBar(sf::Text& bottomBar, sf::Font& font,unsigned int windowWidth, unsigned int windowHeight, sf::RectangleShape& bottomBorder, int FontSize)
{
	bottomBar.setFont(font);
	bottomBar.setCharacterSize(FontSize * 0.6);
    bottomBar.setFillColor(sf::Color::White);
	
    bottomBar.setOrigin(bottomBar.getGlobalBounds().width, bottomBar.getGlobalBounds().height);
	bottomBar.setPosition(windowWidth - 10, windowHeight - 5);

	bottomBorder.setSize(sf::Vector2f(windowWidth - 20, 20));
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




void moveCursorUp(textDocument& doc) {
    if (doc.getACursorLine(doc.cursorPos) == 1 or doc.cursorPos == 0) {
        doc.cursorPos = doc.getALineLength(doc.cursorPos);
    }
    else {
        int currLineLength = doc.getALineLength(doc.cursorPos);


        //Duc cursorul la inceputul liniei curente

        int currLineNumber = doc.getACursorLine(doc.cursorPos);
        int currCursorPos = doc.cursorPos;
        int posinLine = 0;//Memorez pe ce pozitie este cursorul

        while (doc.getACursorLine(doc.cursorPos) == doc.getACursorLine(currCursorPos)) {
            doc.cursorPos--;
            posinLine++;
        }

        std::cout << "CURRLINE LENGTH" << currLineLength << ' ';
        int prevLineLength = doc.getALineLength(doc.cursorPos); //-2 ca ai si '\n'

        std::cout << "PrevLineLength: " << prevLineLength << " PosinLine: " << posinLine << std::endl;
        if (prevLineLength >= posinLine)
            doc.cursorPos = doc.cursorPos - (prevLineLength - posinLine + 1);


    }
}

void moveCursorDown(textDocument& doc) {
    
        if (doc.getLineCount() != doc.getACursorLine(doc.cursorPos) - 1) {// Daca nu sunt pe ultima linie
            int currLinelength = doc.getALineLength(doc.cursorPos);

            //cred ca este inutil IF ul asta dar merge asa ca nu-l scot
            if (doc.cursorPos == '\n')
                currLinelength = doc.getALineLength(doc.cursorPos - 1);
            if (currLinelength + doc.cursorPos >= doc.charCount)
                doc.cursorPos = doc.charCount;
            //Daca linia curenta e mai mare decat urmatoarea
            else if (doc.getACursorLine(doc.cursorPos + currLinelength + 1) != doc.getACursorLine(doc.cursorPos) + 1) {
                while (doc.cursorPos < doc.charCount && doc.getChar(doc.cursorPos)->c != '\n')
                    doc.cursorPos++;//daca nu merge fa do while

                doc.cursorPos++;    //na ca nu ma faci do while ca merge si asa
                doc.cursorPos += doc.getALineLength(doc.cursorPos);
            }
            else {
                doc.cursorPos += currLinelength + 1;
            }
        }
        else if (doc.getLineCount() == doc.getACursorLine(doc.cursorPos) - 1) //daca suntem pe ultima linie
            doc.cursorPos = doc.charCount;                                    //ne ducem la capatul ei
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

    //conditie oprire sa nu putem da pageup cand suntem partial la inceputul textului
	if (textObject.getPosition().y >= 10)
	{
		return;
	}

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

//TREBUIE ADAUGAT PARAMETRU PENTRU A PUTEA DA RESIZE SCROLL BARULUI!!!!!
void ScrollBar(sf::Event& event, sf::RenderWindow& window, sf::RectangleShape& background, sf::RectangleShape& slider,
    bool& isDragged, float& scrollPos, unsigned int WindowWidth, unsigned int WindowHeight)
{
    background.setSize(sf::Vector2f(20, WindowHeight));
    background.setPosition(WindowWidth - 20, 0);
    background.setFillColor(sf::Color(200, 200, 200));

	//Dimensiunea slide urului trebuie sa fie proportionala cu dimensiunea ferestrei
    float sliderheight = WindowHeight * 0.05f;
    slider.setSize(sf::Vector2f(20, sliderheight));
    slider.setPosition(WindowWidth - 20, scrollPos * (background.getSize().y - slider.getSize().y));
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

void makeScrollBarWork(sf::Event& event, textDocument& doc, sf::RenderWindow& window, sf::RectangleShape& background, sf::RectangleShape& slider, bool& isDragged, float& scrollPosNew, float& scrollPosCurrent, sf::Text& textObject)
{
    float lineSpacing = textObject.getLineSpacing() * textObject.getCharacterSize();
    float verticalOffset = 5 * lineSpacing;
    float totalTextHeight = doc.getLineCount() * lineSpacing;
    float viewHeight = background.getSize().y;

    //Daca dau click pe slider si il duc in jos, duc textul sus , altfel il duc jos
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
        scrollPosNew = (slider.getPosition().y - backgroundTop) / scrollLimit;

        if (scrollPosNew > scrollPosCurrent)
        {
            scrollPosCurrent = scrollPosNew;
            scrollDown(doc, window, textObject);
        }
        else if (scrollPosNew < scrollPosCurrent)
        {
            scrollPosCurrent = scrollPosNew;
            scrollUp(doc, window, textObject);
        }
    }
    //verificare daca folosesc mouse wheel pentru scroll, acum trebuie sa fac o conexiune in cele doua cazuri ca sa se mute si slider-ul
	else if (event.type == sf::Event::MouseWheelScrolled)
	{
		if (event.mouseWheelScroll.delta > 0)
		{
			scrollUp(doc, window, textObject);
            slider.move(0, verticalOffset);
		}
		else if (event.mouseWheelScroll.delta < 0)
		{
			scrollDown(doc, window, textObject);
            slider.move(0, -verticalOffset);
		}
	}
}
