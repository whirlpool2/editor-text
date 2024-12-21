#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "textStructs.h"
#include "textFuncs.h"
#include "menuFuncs.h"
#include <stdlib.h>
#include "textSelection.h"
using namespace std;

enum menuOptions
{
	NEW,
	OPEN,
	SAVE,
	SAVE_AS,
	INFO,
	EXIT
};

void initEscMenu(sf::RenderWindow& window, sf::Font& font, fullscreenMenu& menu)
{
	menu.init();
	menu.addButton("New");
	menu.addButton("Open");
	menu.addButton("Save");
    menu.addButton("Save as");
    menu.addButton("Info");
	menu.addButton("Exit");
    menu.update(window, font);
}

// Primește informații de la tastatură și modifică documentul corespunzător.
void handleKeyboardInput(sf::RenderWindow& Window, textDocument& doc, TextSelection& textSelection, char* path)
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
    initEscMenu(Window, font, menu);
    unsigned int menuOption;

	// Declarăm input box-ul.
    // Inițializarea se va face după caz, în funcție de opțiunea aleasă în meniu.
	bool inputBoxActive = false;
	inputBox input;

    // Actualizăm conținutul textului (pentru a prelua datele încărcate).
    updateTextObject(&doc, Window, text);

    while (Window.isOpen()) // Cât timp fereastra este deschisă, tot codul rulează la infinit.
    {
        sf::Event event;
        while (Window.pollEvent(event)) // Verificăm event-ul curent.
        {
            if (inputBoxActive)
            {
                char* userInput = input.handleInput(event, inputBoxActive);
                if (inputBoxActive == false)
                {
                    switch (menuOption)
                    {
                    case OPEN:
                        loadFile(doc, userInput);
                        updateWholeTextObject(&doc, Window, text);
                        break;
                    case SAVE_AS:
                        saveFile(doc, userInput);
                        break;
                    }
					path = userInput;
                    menuActive = false;
                }
            }
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
                                menuOption = NEW;
                                break;
                            case 1:
                                // Open
                                cout << "Open" << endl;
                                menuOption = OPEN;
								input.init(Window, font, 600, 32, "Enter file path:");
								inputBoxActive = true;
                                break;
                            case 2:
                                // Save
                                cout << "Save" << endl;
                                menuOption = SAVE;
                                break;
							case 3:
								// Save as
								cout << "Save as" << endl;
								menuOption = SAVE_AS;
                                input.init(Window, font, 600, 32, "Enter file save path:");
                                inputBoxActive = true;
								break;
                            case 4:
								// Info
								cout << "Info" << endl;
                                menuOption = INFO;
								break;
                            case 5:
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
                        if (key == 8 && doc.charCount != 0 && doc.cursorPos > 0) {
                                //daca am shift apasat si dau delete se sterge tot continutul selectat
                                textSelection.deleteSelectedText(doc, text, textSelection, cursorVisual, cursorClock, cursorVisible, Window);
                            
                            
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
                    bool isControlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
					if (event.key.code == sf::Keyboard::Escape)
					{
						menuActive = !menuActive;
					}
                    //Avem cazurile pentru selectie cu shift
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                        if (doc.cursorPos > 0 && event.key.code == sf::Keyboard::Left) {

                            textSelection.updateSelectedTextKeys(doc, text, sf::Vector2i(-1, 0), isControlPressed);
                        }
                        if (doc.cursorPos < doc.charCount && event.key.code == sf::Keyboard::Right) {
                            textSelection.updateSelectedTextKeys(doc, text, sf::Vector2i(1, 0), isControlPressed);
                        }
                        if (event.key.code == sf::Keyboard::Up) {
                            textSelection.updateSelectedTextKeys(doc, text, sf::Vector2i(0, -1), isControlPressed);
                        }
                        if (event.key.code == sf::Keyboard::Down) {
                            textSelection.updateSelectedTextKeys(doc, text, sf::Vector2i(0, 1), isControlPressed);
                        }
                    }
                    //Daca nu e apasat shift, atunci doar mutam cursorul
                    else {
                        //Vector2i(0, 0) inseamna ca nu se selecteaza nimic
                        textSelection.updateSelectedTextKeys(doc, text, sf::Vector2i(0, 0), isControlPressed);
                        if (event.key.code == sf::Keyboard::Left && doc.cursorPos > 0)
                            doc.cursorPos--;
                        if (event.key.code == sf::Keyboard::Right && doc.cursorPos < doc.charCount)
                            doc.cursorPos++;
                        if (event.key.code == sf::Keyboard::Down) // Trecem la linia următoare.
                            moveCursorDown(doc);
                        if (event.key.code == sf::Keyboard::Up) // Trecem la linia precedentă.
                            moveCursorUp(doc);
                    }
					
					//Cazurile de copiere, cut si paste
                    if (isControlPressed)
					{
						std::cout << "SIGGGMA" << std::endl;
						
                        //CTRL + C comanda de copiere
                        if (event.key.code == sf::Keyboard::C)
						{
							textSelection.copyText(doc);
						}
                        //CTRL + X, comanda de cut
						if (event.key.code == sf::Keyboard::X)
						{
							textSelection.copyText(doc);
							textSelection.deleteSelectedText(doc, text, textSelection, cursorVisual, cursorClock, cursorVisible, Window);
						}
						//CTRL + V comanda de paste
						if (event.key.code == sf::Keyboard::V)
						{
							textSelection.pasteText(doc, text, Window, cursorVisual, cursorClock, cursorVisible);
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
                    if (event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                    {
                        saveFile(doc, path);
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
        textSelection.drawHighLight(Window, text, doc);
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
        if (inputBoxActive)
        {
            input.draw(Window, font);
        }
        Window.display();
    }
}

int main()
{
    sf::RenderWindow EditorText(sf::VideoMode(800, 800), "Editor Text", sf::Style::Default);
    EditorText.setFramerateLimit(60);

    textDocument doc;
    TextSelection textSelection;

    doc.init();

    // Schimbă asta dacă vrei să încarci un fișier.
    char* path = nullptr;
    
    // loadFile(doc, path);

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
    
    /*
    sf::Font font;
    sf::Text text;
    setFont(text, font, 24, COLOR_TEXT, (char*)"Fonts/CascadiaMono.ttf");

    inputBox input;
	input.init(EditorText, font, 600, 100, "Introduceti textul:");
	bool inputBoxActive = true;

    char* inputString = nullptr;

    while (EditorText.isOpen())
    {
        sf::Event event;
        while (EditorText.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                EditorText.close();
            }
            if (inputBoxActive)
            {
				inputString = input.handleInput(event, inputBoxActive);
                input.update();
            }
        }
        EditorText.clear(sf::Color(COLOR_BG.r, COLOR_BG.g, COLOR_BG.b));
        if (inputBoxActive)
        {
            input.draw(EditorText, font);
        }
        
        EditorText.display();
    }
    */
    
    handleKeyboardInput(EditorText, doc, textSelection, path);

    return 0;
}