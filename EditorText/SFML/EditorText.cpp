#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "textStructs.h"
#include "textFuncs.h"
#include <stdlib.h>

using namespace std;

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


// Primește informații de la tastatură și modifică documentul corespunzător.
void handleKeyboardInput(sf::RenderWindow& Window, textDocument& doc)
{
    sf::Font font;
    sf::Text text;

    int fontSize = 24;
    unsigned int firstLine = 0;

    // Declarăm forma pentru cursorul vizual.
    sf::RectangleShape cursorVisual;
    sf::Clock cursorBlinkInterval;
    bool cursorVisible = true;
    cursorVisual.setFillColor(sf::Color(COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b));
    cursorVisual.setSize(sf::Vector2f(2, text.getCharacterSize()));

    // Se pune cursorul vizual pe prima poziție a documentului.
    cursorVisual.setPosition(text.getPosition().x, text.getPosition().y);

    // Setăm fontul la unul arbitrar.
    setFont(text, font, 24, COLOR_TEXT, (char*)"Fonts/CascadiaMono.ttf");

    // cout << visibleLineCount(Window, text) << endl;

    bool isDragged = false;
    float scrollPos = 0.0f;

    updateTextObject(&doc, Window, text, firstLine);

    while (Window.isOpen()) // Cât timp fereastra este deschisă, tot codul rulează la infinit.
    {
        sf::Event event;
        while (Window.pollEvent(event)) // Verificăm event-ul curent.
        {
            if (event.type == sf::Event::Closed)
            {
                Window.close();
            }

            // Daca este apăsat click stânga.
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(Window);
                    doc.cursorPos = cursorClickPos(mousePos, doc, text);
                }
            }
            // Dacă event-ul curent presupune caractere date de la tastatură...
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    // Convertim tasta apăsată într-un char.
                    char key = static_cast<char>(event.text.unicode);

                    // Dacă se apasă backspace și se poate șterge...
                    if (key == 8 && doc.charCount != 0 && doc.cursorPos > 0)
                    {
                        deleteCharInTextObject(&doc, text);
                    }
                    else if (key >= 32 && key <= 126)
                    {
                        // key in [32, 126] implică caracterele alfanumerice, punctuații și paranteze, etc.
                        insertCharInTextObject(&doc, text, key);
                    }
                    else if (key == 13)
                    {
                        // 13 corespude CR, adică ENTER.
                        insertCharInTextObject(&doc, text, '\n');
                    }
                }
                // TODO: Revizitează optimizarea asta.
                // Probabil ar trebui să țin minte doi cursori diferiți, unul pentru cursorul vizual și unul pentru cursorul documentului.
                // Ar fi cam greu totuși.
                /*
                if (doc.getCursorLine() > visibleLineCount(Window, text))
                {
                    cout << "Cursorul este la o linie mai mare decat numarul de linii vizibile. (" << doc.getCursorLine() << " > " << visibleLineCount(Window, text) << ")" << endl;
                    updateTextObject(&doc, Window, text, doc.getCursorLine() - visibleLineCount(Window, text));
            }
                */

                // Test pentru funcția docToString. Se va șterge, și va fi apelată la nevoie ulterior.
                // debugString(&doc);
            }
            if (event.type == sf::Event::KeyPressed) // Acest caz tratează tastele ce nu produc caractere.
            {
                if (event.key.code == sf::Keyboard::Left && doc.cursorPos > 0)
                {
                    doc.cursorPos--;
                }
                if (event.key.code == sf::Keyboard::Right && doc.cursorPos < doc.charCount)
                {
                    doc.cursorPos++;
                }
                if (event.key.code == sf::Keyboard::Down && doc.cursorPos < doc.charCount) // Trecem la linia următoare.
                {
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
                if (event.key.code == sf::Keyboard::Up && doc.cursorPos > 0) // Trecem la linia precedentă.
                {
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
                if (event.key.code == sf::Keyboard::Equal && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) // Zoom-in (CTRL + '=')
                {
                    fontSize = fontSize + 4;
                    text.setCharacterSize(fontSize);
                    cursorVisual.setSize(sf::Vector2f(2, text.getCharacterSize()));
                }
                if (event.key.code == sf::Keyboard::Dash && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) // Zoom-out (CTRL + '-')
                {
                    fontSize = (fontSize > 6) ? fontSize - 4 : fontSize;
                    text.setCharacterSize(fontSize);
                    cursorVisual.setSize(sf::Vector2f(2, text.getCharacterSize()));
                }
                if (event.key.code == sf::Keyboard::PageDown)
                {
                    scrollDown(firstLine, &doc, Window, text);
                }
                if (event.key.code == sf::Keyboard::PageUp)
                {
                    scrollUp(firstLine);
                }

                updateTextObject(&doc, Window, text, firstLine);
            }
        }

        // Se actualizează poziția cursorului vizual.
        if (doc.charCount > 0 && doc.cursorPos > 0)
        {
            sf::Vector2f cursorVisualPos = text.findCharacterPos(doc.cursorPos);
            cursorVisual.setPosition(cursorVisualPos.x, cursorVisualPos.y);
            // Funcția findCharacterPos returnează poziția unui caracter dintr-un text ca o pereche de 2 float-uri.
        }
        else // Dacă currentText este gol, documentul este gol. Atunci, cursorul este poziționat unde ar veni primul caracter.
        {
            cursorVisual.setPosition(text.getPosition().x, text.getPosition().y);
        }

        // Se animează cursorul.
        cursorAnimate(cursorVisual, cursorBlinkInterval, cursorVisible);

        //creez scrollbar si animatie

        sf::RectangleShape Bar;
        sf::RectangleShape Slider;

        ScrollBar(event, Window, Bar, Slider, isDragged, scrollPos);

        sf::Text bottomBarText;
        int cursorLine = doc.getCursorLine();
        int linePos = doc.getCursorPositionInLine();
        bottomBar(doc, bottomBarText, font, Window.getSize().y);

        // Actualizăm window-ul.
        Window.clear(sf::Color(COLOR_BG.r, COLOR_BG.g, COLOR_BG.b));
        Window.draw(text);
        Window.draw(cursorVisual);
        Window.draw(Bar);
        Window.draw(Slider);
        Window.draw(bottomBarText);
        Window.display();
    }
}

int main()
{
    sf::RenderWindow EditorText(sf::VideoMode(800, 800), "Editor Text", sf::Style::Default);
    EditorText.setFramerateLimit(60);

    textDocument doc;

    doc.init();

    // Schimbă asta dacă vrei să încarci un fișier.
    char path[] = "test.txt";
    
    loadFile(doc, path);

    handleKeyboardInput(EditorText, doc);

    return 0;
}