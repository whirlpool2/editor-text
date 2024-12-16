#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "textStructs.h"
#include "textFuncs.h"
#include "menuFuncs.h"
#include <stdlib.h>

using namespace std;



// Primește informații de la tastatură și modifică documentul corespunzător.
void handleKeyboardInput(sf::RenderWindow& Window, textDocument& doc)
{
    sf::Font font;
    sf::Text text;

    int fontSize = 24;
    unsigned int firstLine = 0;

    sf::RectangleShape Bar;
    sf::RectangleShape Slider;

    // Declarăm forma pentru cursorul vizual.
    sf::RectangleShape cursorVisual;
    sf::Clock cursorClock;
    bool cursorVisible = true;
    cursorVisual.setFillColor(sf::Color(COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b));
    cursorVisual.setSize(sf::Vector2f(2, text.getCharacterSize()));

    // Se pune cursorul vizual pe prima poziție a documentului.
    cursorVisual.setPosition(text.getPosition().x, text.getPosition().y);

    // Setăm fontul la unul arbitrar.
    setFont(text, font, 24, COLOR_TEXT, (char*)"Fonts/CascadiaMono.ttf");

    sf::Text bottomBarText;
    sf::RectangleShape bottomBorder;
    initializeBottomBar(bottomBarText, font, Window.getSize().y, bottomBorder);

    // cout << visibleLineCount(Window, text) << endl;

    bool isDragged = false;
    float scrollPos = 0.0f;

    // Declarăm și inițializăm meniul.
    bool menuActive = false;
    fullscreenMenu menu;
    menu.init();
    menu.addButton("New");
    menu.addButton("Open");
    menu.addButton("Save");
    menu.addButton("Exit");
    menu.update(Window, font);

    // Actualizăm conținutul textului (pentru a prelua datele încărcate).
    updateTextObject(&doc, Window, text);

    while (Window.isOpen()) // Cât timp fereastra este deschisă, tot codul rulează la infinit.
    {
        sf::Event event;
        while (Window.pollEvent(event)) // Verificăm event-ul curent.
        {
            if (menuActive)
            {
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Escape)
					{
						menuActive = !menuActive;
					}
				}
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        int clickedButton = menu.getClickedButton(sf::Mouse::getPosition(Window));
                        if (clickedButton != -1)
                        {
                            switch (clickedButton)
                            {
                            case 0:
                                // New
                                cout << "New" << endl;
                                break;
                            case 1:
                                // Open
                                cout << "Open" << endl;
                                break;
                            case 2:
                                // Save
                                cout << "Save" << endl;
                                break;
                            case 3:
                                // Exit
                                Window.close();
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                if (event.type == sf::Event::Closed)
                {
                    Window.close();
                }
                updateBottomBar(bottomBarText, doc);
                // Daca este apăsat click stânga.
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(Window);
                        cursorClickPos(mousePos, doc, text);
                        updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
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
                    updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
                }
                //implementare makeScrollBarWork
                float scrollPosCurrent = scrollPos;
                makeScrollBarWork(event, doc, Window, Bar, Slider, isDragged, scrollPos, scrollPosCurrent, text);
                if (event.type == sf::Event::KeyPressed) // Acest caz tratează tastele ce nu produc caractere.
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        menuActive = !menuActive;
                    }
                    if (event.key.code == sf::Keyboard::Left && doc.cursorPos > 0)
                    {
                        doc.cursorPos--;
                    }
                    if (event.key.code == sf::Keyboard::Right && doc.cursorPos < doc.charCount)
                    {
                        doc.cursorPos++;
                    }
                    if (event.key.code == sf::Keyboard::Down) // Trecem la linia următoare.
                    {
                        moveCursorDown(doc);
                    }
                    if (event.key.code == sf::Keyboard::Up) // Trecem la linia precedentă.
                    {
                        moveCursorUp(doc);
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
                    if (event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                    {
                        saveFile(doc, (char*)"test.txt");
                    }
                    if (event.key.code == sf::Keyboard::PageDown || cursorVisual.getPosition().y > Window.getSize().y - 50)
                    {
                        scrollDown(doc, Window, text);
                    }
                    if (event.key.code == sf::Keyboard::PageUp || cursorVisual.getPosition().y < 50)
                    {
                        scrollUp(doc, Window, text);
                    }

                    // updateTextObject(&doc, Window, text);
                    updateWholeTextObject(&doc, Window, text);
                    updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
                }
            }
        }

        // Se animează cursorul.
        cursorAnimate(cursorVisual, cursorClock, cursorVisible);
        ScrollBar(event, Window, Bar, Slider, isDragged, scrollPos);

        // Actualizăm window-ul.
        Window.clear(sf::Color(COLOR_BG.r, COLOR_BG.g, COLOR_BG.b));
        Window.draw(text);
        Window.draw(cursorVisual);
        Window.draw(Bar);
        Window.draw(Slider);
        Window.draw(bottomBorder);
        Window.draw(bottomBarText);
        if (menuActive)
        {
			menu.draw(Window, font);
        }
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

    /*
    sf::Font font;
    sf::Text text;
    setFont(text, font, 24, COLOR_TEXT, (char*)"Fonts/CascadiaMono.ttf");

    fullscreenMenu menu;
    menu.init();
    menu.addButton("New");
	menu.addButton("Open");
	menu.addButton("Save");
	menu.addButton("Exit");
    menu.addButton("Info");
    menu.update(EditorText, font);

    while (EditorText.isOpen())
    {
        EditorText.clear(sf::Color(COLOR_BG.r, COLOR_BG.g, COLOR_BG.b));
        menu.draw(EditorText, font);
		EditorText.display();
    }
    */
    
    handleKeyboardInput(EditorText, doc);

    return 0;
}