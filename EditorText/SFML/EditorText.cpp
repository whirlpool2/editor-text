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
    FIND,
    FIND_REPLACE,
    NEXT,
    PREVIOUS,
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
    menu.addButton("Find");
    menu.addButton("Find and Replace");
    menu.addButton("Next");
    menu.addButton("Previous");
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
    initializeBottomBar(bottomBarText, font,Window.getSize().x, Window.getSize().y, bottomBorder, fontSize);

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


    //Bool pentru mouse
    bool wasMousePressed = false;
	bool isMousePressed = false;

    //Pentru find and replace
    bool findReplaceActive = false;
    bool findInputActive = false; 
    bool replaceInputActive = false; 
    inputBox findInput;
    inputBox replaceInput;
    std::string message;
    sf::Clock messageClock;
	bool showMessage = false;//In cazul in care nu gasim textul de cautat afisam un mesaj
    

	//Avem un vectr in care memoram pozitiile la care se afla textul cautat
    std::vector<unsigned long long> matchPositions;
    int currentMatchIndex = -1; //-1 in cazul in care nu avem matches ca sa putem da trigger la cazul respectiv

    // Actualizăm conținutul textului (pentru a prelua datele încărcate).
    updateTextObject(&doc, Window, text);

    while (Window.isOpen()) // Cât timp fereastra este deschisă, tot codul rulează la infinit.
    {
        sf::Event event;
        while (Window.pollEvent(event)) // Verificăm event-ul curent.
        {
            if (findReplaceActive)
            {
                if (findInputActive)
                {

                    //Luam input-ul de la user
                    char* findText = findInput.handleInput(event, findInputActive);
                    if (!findInputActive)
                    {
						// Daca am terminat de introdus textul de cautat, aidca daca s-a apasat enter dupa ce s-a introdus textul cautat, trecem la replace
                        replaceInputActive = true;
                        replaceInput.init(Window, font, 600, 32, "Enter text to replace:");
                    }
                }
                else if (replaceInputActive)
                {
					//Luam input-ul de la user
                    char* findTextCStr = findInput.getText();
                    char* replaceTextCStr = replaceInput.getText();
                    if (findTextCStr != nullptr && replaceTextCStr != nullptr)
                    {
                        std::string findText(findTextCStr);
                        std::string replaceText(replaceTextCStr);

						//Verificam daca textul cautat exista in document
                        std::vector<unsigned long long> positions = doc.findText(findText);
                        if (positions.empty())
                        {
                            // Daca nu gasim atunci afisam mesaj
                            message = "Nu au fost gasite aparitii pentru '" + findText;
                            messageClock.restart(); //avem un clock pentru mesaj de 3 secunde
                            showMessage = true;
                        }
                        else
                        {
							//Aici inlocuim textul gasit
                            doc.replaceText(findText, replaceText);
                            updateWholeTextObject(&doc, Window, text);
                        }
                    }
                    findReplaceActive = false;
                }
            }
            else if (findInputActive)
            {
                //Luam inputul de la user
                char* findText = findInput.handleInput(event, findInputActive);
                if (!findInputActive)
                {
					// Dupa ce tastam enter, cautam textul in document
                    char* findTextCStr = findInput.getText();
                    if (findTextCStr != nullptr)
                    {
                        std::string findText(findTextCStr);

						// Verificam daca exista textul cautat in document
                        matchPositions = doc.findText(findText);
                        if (matchPositions.empty())
                        {
                            // Daca nu am gasit atunci afisam mesajul
                            message = "Nu au fost gasite aparitii pentru '" + findText;
                            messageClock.restart();
                            showMessage = true;
                        }
                        else
                        {
							// Dam highlight la textul gasit, daca sunt mai multe dam highlight la primul
							//Ca sa navigam printre match uri folosim Next si Previous
                            currentMatchIndex = 0;
                            textSelection.SelStart = matchPositions[currentMatchIndex];
                            textSelection.SelEnd = matchPositions[currentMatchIndex] + findText.length();
                            textSelection.isSelected = true;
                            doc.cursorPos = textSelection.SelEnd;
                            updateWholeTextObject(&doc, Window, text);
                        }
                    }
                    findInputActive = false;
                }
            }
            //Cazurile pentru celelalte butoane din meniu
            else if (inputBoxActive)
            {
                char* userInput = input.handleInput(event, inputBoxActive);
                if (!inputBoxActive)
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
			//Cazurile pentru butoanele din meniu
            else if (menuActive)
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
                        //Aflam pe ce buton s-a dat click
                        int clickedButton = menu.getClickedButton(sf::Mouse::getPosition(Window));
                        if (clickedButton != -1)
                        {
                            switch (clickedButton)
                            {
                            case 0:
                                // New
                                std::cout << "New" << std::endl;
                                menuOption = NEW;
                                break;
                            case 1:
                                // Open
                                std::cout << "Open" << std::endl;
                                menuOption = OPEN;
                                input.init(Window, font, 600, 32, "Enter file path:");
                                inputBoxActive = true;
                                break;
                            case 2:
                                // Save
                                std::cout << "Save" << std::endl;
                                menuOption = SAVE;
                                saveFile(doc, path);
                                break;
                            case 3:
                                // Save as
                                std::cout << "Save as" << std::endl;
                                menuOption = SAVE_AS;
                                input.init(Window, font, 600, 32, "Enter file save path:");
                                inputBoxActive = true;
                                break;
                            case 4:
                                // Info
                                std::cout << "Info" << std::endl;
                                menuOption = INFO;
                                break;
                            case 5:
                                // Find
                                std::cout << "Find" << std::endl;
                                menuOption = FIND;
                                findInputActive = true;
                                findInput.init(Window, font, 600, 32, "Enter text to find:");
                                break;
                            case 6:
                                // Find and Replace
                                std::cout << "Find and Replace" << std::endl;
                                menuOption = FIND_REPLACE;
                                findReplaceActive = true;
                                findInputActive = true; 
                                findInput.init(Window, font, 600, 32, "Enter text to find:");
                                break;
                            case 7:
                                // Next
                                std::cout << "Next" << std::endl;
								if (!matchPositions.empty())//Daca avem match uri
                                {
                                    currentMatchIndex = (currentMatchIndex + 1) % matchPositions.size();
									//avem modulo pentru a putea face cicluri printre match uri
									std::string findText(findInput.getText()); //Folosim un string pentru a tine minte textul cautat
                                    textSelection.SelStart = matchPositions[currentMatchIndex];
                                    textSelection.SelEnd = matchPositions[currentMatchIndex] + findText.length();
									textSelection.isSelected = true;//Facem True ca sa dam highlight la textul gasit
									doc.cursorPos = textSelection.SelEnd;//mutam cursorul la finalul textului gasit
                                    updateWholeTextObject(&doc, Window, text);
                                }
                                break;
                            case 8:
                                // Previous
                                std::cout << "Previous" << std::endl;
                                if (!matchPositions.empty())
                                {
                                    currentMatchIndex = (currentMatchIndex - 1 + matchPositions.size()) % matchPositions.size();
									//Adunam matchPositions.size ca sa nu puste programul in aer, ca sa ramana indexul pozitiv
									//Uneori se poate bugui si vrem sa evitam crash uri
                                    std::string findText(findInput.getText()); // Define findText here
                                    textSelection.SelStart = matchPositions[currentMatchIndex];
                                    textSelection.SelEnd = matchPositions[currentMatchIndex] + findText.length();
                                    textSelection.isSelected = true;
                                    doc.cursorPos = textSelection.SelEnd;
                                    updateWholeTextObject(&doc, Window, text);
                                }
                                break;
                            case 9:
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
                if (event.type == sf::Event::Resized)
                {
					// memoram noile dimensiuni ale ferestrei
                    sf::Vector2u newSize(event.size.width, event.size.height);
					// actualizam dimensiunea textului ca sa arate la fel pe ecran
                    fontSize = fontSize * newSize.y / Window.getSize().y;

                    Window.setView(sf::View(sf::FloatRect(0, 0, newSize.x, newSize.y)));
                    initEscMenu(Window, font, menu);
                    initializeBottomBar(bottomBarText, font, newSize.x, newSize.y, bottomBorder, fontSize);
					ScrollBar(event, Window, Bar, Slider, isDragged, scrollPos, newSize.x, newSize.y);
                    updateWholeTextObject(&doc, Window, text);
                }
                if (findReplaceActive)
                {
                    if (findInputActive)
                    {
                        char* findText = findInput.handleInput(event, findInputActive);
                        if (!findInputActive)
                        {
							// Afisam interfata pentru replace
                            replaceInputActive = true;
                            replaceInput.init(Window, font, 600, 32, "Enter text to replace:");
                        }
                    }
					// Am afisat promptul, acum trecem al replace
                    else if (replaceInputActive)
                    {
                        char* replaceText = replaceInput.handleInput(event, replaceInputActive);
                        if (!replaceInputActive)
                        {
                            // Acum trecem la inlocuire in sine
                            char* findTextCStr = findInput.getText();//memoram textul cautat
							char* replaceTextCStr = replaceInput.getText();//memoram textul cu care inlocuim
                            if (findTextCStr != nullptr && replaceTextCStr != nullptr)
                            {
                                std::string findText(findTextCStr);
                                std::string replaceText(replaceTextCStr);

								//Verificam daca textul cautat exista in document
                                std::vector<unsigned long long> positions = doc.findText(findText);
                                if (positions.empty())
                                {
                                    // Nu exista, afisam mesaj
                                    message = "Nu au fost gasite aparitii pentru '" + findText;
                                    messageClock.restart();
                                    showMessage = true;
                                }
                                else
                                {
                                    doc.replaceText(findText, replaceText);
                                    updateWholeTextObject(&doc, Window, text);
                                }
                            }
                            findReplaceActive = false;
                        }
                    }
                }
                updateBottomBar(bottomBarText, doc);
                // Daca este apăsat click stânga.
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
						//La selectia cu mouse, pentru implementarea curenta
                        //avem un caz care trebuie tratat
                        //Daca facem o selectie cu mouse ul, dupa ce dam release la click stanga
                        //Daca dam click din nou va trebui sa resetam selectia, asa ca aceste 2 if uri
                        //au rolul de a trata cazul acesta
                        //Daca mouse ul nu este apasat acum dar a fos apasat inainte inseamna ca resetam selectia
                        if(!isMousePressed && !wasMousePressed)
							textSelection.isSelected = false;
                        else if (!isMousePressed && wasMousePressed) {
                            textSelection.isSelected = false;
							wasMousePressed = false;
                        }
						//Facem update la textul selectat
                        isMousePressed = true;
                        sf::Vector2i mousePos = sf::Mouse::getPosition(Window);
						cursorClickPos(mousePos, doc, text);
						textSelection.updateSelectedTextMouse(doc, text, mousePos, isMousePressed,
                            Window, cursorVisual, cursorClock, cursorVisible);
                        updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
                    }
                }
				//Daca butonul mouse-ului este eliberat atunci indicam asta algoritmului ca sa nu mai faca selectie
                   
                if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        isMousePressed = false;
                        wasMousePressed = true;
                    }
					
                }
        
                if (event.type == sf::Event::MouseMoved) {
                    if (isMousePressed) {
                        
                        sf::Vector2i mousePos = sf::Mouse::getPosition(Window);
						textSelection.updateSelectedTextMouse(doc, text, mousePos, isMousePressed,
							Window, cursorVisual, cursorClock, cursorVisible);
						updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
                    }

                }
                
                // Dacă event-ul curent presupune caractere date de la tastatură...
                if (event.type == sf::Event::TextEntered)
                {
                    //Pentru selectie, resetam pentru fiecare caz isSelected pentru a putea efectua stergerea si pentru a evita
					//sa se reseteze selectia cand nu trebuie

                    //cazul cand apasam o tasta
                    if (event.text.unicode >= 32 and event.text.unicode <= 126) {
                        // Convertim tasta apăsată într-un char.
                        char key = static_cast<char>(event.text.unicode);
                        // Daca exista o selecție activa, stergem textul selectat.
                        if (textSelection.isSelected)
                        {
                            textSelection.deleteSelectedText(doc, text, cursorVisual, cursorClock, cursorVisible, Window);
                        }
                        insertCharInTextObject(&doc, text, key);
                        textSelection.isSelected = false;

                    }
                    //backspace
                    else if (event.text.unicode == 8) {
                        if (textSelection.isSelected)
                        {
                            textSelection.deleteSelectedText(doc, text, cursorVisual, cursorClock, cursorVisible, Window);
                        }
                        else
                        {
                            deleteCharInTextObject(&doc, text);
                        }
                        textSelection.isSelected = false;
                    }
                    //enter
                    else if (event.text.unicode == 13) {
                        if (textSelection.isSelected)
                        {
                            textSelection.deleteSelectedText(doc, text, cursorVisual, cursorClock, cursorVisible, Window);
                        }
                        insertCharInTextObject(&doc, text, '\n');
                        textSelection.isSelected = false;
                    }
                    updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
                    
                }
                //implementare makeScrollBarWork
                float scrollPosCurrent = scrollPos;
                makeScrollBarWork(event, doc, Window, Bar, Slider, isDragged, scrollPos, scrollPosCurrent, text);
                if (event.type == sf::Event::KeyPressed) // Acest caz tratează tastele ce nu produc caractere.
                {
                    bool isControlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
                    bool isShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

                    if (event.key.code == sf::Keyboard::Escape)
					{
						menuActive = !menuActive;
					}
                    //Avem cazurile pentru selectie cu shift
                    if (isShiftPressed) {
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
                    //Cazurile cu ctrl
                    else if (isControlPressed)
                    {

                        //CTRL + A comanda de select all
                        if (event.key.code == sf::Keyboard::A) {
                            textSelection.SelStart = 0;
                            textSelection.SelEnd = doc.charCount;
                            doc.cursorPos = doc.charCount;
                            textSelection.isSelected = true;
                            isControlPressed = true;
                        }

                        //CTRL + C comanda de copiere
                        else if (event.key.code == sf::Keyboard::C)
                        {
							std::cout << "Selectia a fost Copiata" << std::endl;
                            textSelection.copyText(doc);
                        }
                        //CTRL + X, comanda de cut
                        else if (event.key.code == sf::Keyboard::X)
                        {
                            textSelection.copyText(doc);
                            textSelection.deleteSelectedText(doc, text, cursorVisual, cursorClock, cursorVisible, Window);
                        }
                        //CTRL + V comanda de paste
                        else if (event.key.code == sf::Keyboard::V)
                        {
                            textSelection.pasteText(doc, text, Window, cursorVisual, cursorClock, cursorVisible);
                        }
                    }
                    else {
                        //Vector2i(0, 0) inseamna ca nu se selecteaza nimic
                        if (event.key.code == sf::Keyboard::Left && doc.cursorPos > 0) {
                            doc.cursorPos--;
                            textSelection.isSelected = false;
                        }
                        if (event.key.code == sf::Keyboard::Right && doc.cursorPos < doc.charCount){
                            doc.cursorPos++;
							textSelection.isSelected = false;
                        }
                        if (event.key.code == sf::Keyboard::Down) // Trecem la linia următoare.
                        {
                            moveCursorDown(doc);
							textSelection.isSelected = false;
                        }
                        if (event.key.code == sf::Keyboard::Up) // Trecem la linia precedentă.
                        {
                            moveCursorUp(doc);
							textSelection.isSelected = false;
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
		ScrollBar(event, Window, Bar, Slider, isDragged, scrollPos, Window.getSize().x, Window.getSize().y);

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
        if (findReplaceActive)
        {
            if (findInputActive)
            {
                findInput.draw(Window, font);
            }
            else if (replaceInputActive)
            {
                replaceInput.draw(Window, font);
            }
        }
        if (findInputActive)
        {
            findInput.draw(Window, font);
        }
        if (showMessage)
        {
            sf::Text messageText;
            messageText.setFont(font);
            messageText.setString(message);
            messageText.setCharacterSize(24);
            messageText.setFillColor(sf::Color::Red);

            // Center the text on the window
            sf::FloatRect textRect = messageText.getLocalBounds();
            messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            messageText.setPosition(Window.getSize().x / 2.0f, Window.getSize().y / 2.0f);

            Window.draw(messageText);

            // Hide the message after 3 seconds
            if (messageClock.getElapsedTime().asSeconds() > 3)
            {
                showMessage = false;
            }
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