#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

struct RGBColor
{
    int r;
    int g;
    int b;
};

struct character
{
    char c;
    character* prev;
    character* next;
};

struct line
{
    character* first;
    line* prev;
    line* next;
};

struct textDocument
{
    line* first;
    int totalLines;
    int cursorLine;
    int cursorChar;
};

const RGBColor             COLOR_BG = {  32,  32,  32 };
const RGBColor COLOR_TEXT_HIGHLIGHT = { 255, 255, 128 };
const RGBColor           COLOR_TEXT = { 255, 255, 255 };


// Returnează un font. (Mai mult pentru a nu tasta font.loadFromFile() de fiecare dată.)
Font loadFont(string path)
{
    Font font;

    if (!(font.loadFromFile(path))) // Dacă nu găsim fontul, incărcăm fontul inclus cu proiectul.
    {
        cerr << "Font " << path << " could not be found." << '\n';
        font.loadFromFile("Fonts/CascadiaMono.ttf");
        return font;
    }

    return font;
}

// Setează dimensiunea, fontul și culoarea unui text.
void setFont(Text& text, Font& font, int size, RGBColor color, string path)
{
    font = loadFont(path);

    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(Color(color.r, color.g, color.b));
    text.setPosition(10, 10);
}

// Evidențiază cuvântul curent.
void CurrentWord(string& currentText, int cursorPos, Text& text, Text& currWord) {

    int start = cursorPos;
    int end   = cursorPos;

    while (start > 0 && currentText != ' ')
    {
        start--;
    }

    while (end < currentText.length() && currentText[end] != ' ')
    {
        end++;
    }

    string before = currentText.substr(0, start);
    string current = currentText.substr(start, end - start);
    string after = currentText.substr(end);

    currWord.setString(current);
    Vector2f startPos = text.findCharacterPos(start);
    currWord.setPosition(startPos);
    text.setString(before + current + after);
}

// Returnează primele k caractere dintr-un string.
string prefix(string& str, int k)
{
    string res = "";

    if (k > str.size())
    {
        k = str.size();
    }

    for (int i = 0; i < k; i++)
    {
        res = res + str[i];
    }

    return res;
}

// Returnează ultimele k caractere dintr-un string.
string suffix(string& str, int k)
{
    string res = "";

    for (int i = k; i < str.size(); i++)
    {
        res = res + str[i];
    }

    return res;
}

// Animează vizualul cursorului (animația de blink).
void cursorAnimate(RectangleShape& cursor, Clock& timpAlternanta, bool& esteVizibil)
{
    //in esteVizibil retin daca afisam cursor care are forma de dreptunghi , daca estevizibil dau culoare alb , altfel fac transparent
    if (timpAlternanta.getElapsedTime().asMilliseconds() > 500)
    {
        esteVizibil = 1 - esteVizibil; // Alternăm vizibilitatea după 500 milisecunde.
        timpAlternanta.restart();
    }

    if (esteVizibil)
    {
        cursor.setFillColor(Color(COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b));
    }
    else
    {
        cursor.setFillColor(Color::Transparent);
    }
}
//Functie care updateaza pozitia cursorului in functie de mouse 
void updateCursorForMouse(Vector2i &mousepos, string currentText, int &cursorPos, Text text) {
    
    //Iteram prin tot textul pentru a gasi pozitia mouseului
    for (int i = 0; i < currentText.size(); i++) {
        Vector2f pozitieCaracterCurent = text.findCharacterPos(i);

        //Verificam daca mouse ul este pe caracterul  curent
        //Daca incercam sa verifica mousepos.x == pozitieCaractercurent.x  si   mousepos.y == pozitieCaracterCurent.y
        //nu va functiona pentru ca mouse-ul ar trebui sa fie FIX pe aceeasi pozitie cu caracterul la care vrem sa ajungem, asa ca
        //folosim intervale de 1 caracter
        if (mousepos.x >= pozitieCaracterCurent.x and mousepos.x <= pozitieCaracterCurent.x + text.getCharacterSize()
            and mousepos.y >= pozitieCaracterCurent.y and mousepos.y <= pozitieCaracterCurent.y + text.getCharacterSize()) {
            cursorPos = i + 1;
            break;
        }
        }


}

// Primește informații de la tastatură și modifică documentul corespunzător.
void handleKeyboardInput(RenderWindow& Window)
{
    string currentText = "";
    int cursorPos = 0;
    Font font;
    Text text;

    // Creez bara pt animatie cursor
    
    int fontSize = 24;

    RectangleShape cursor;
    Clock timpAlternanta;
    bool esteVizibil = true;
    cursor.setFillColor(Color::White);
    cursor.setSize(Vector2f(2, text.getCharacterSize()));

    //pun cursorul pe pozitia initiala

    cursor.setPosition(text.getPosition().x, text.getPosition().y);

    setFont(text, font, 24, COLOR_TEXT, "Fonts/CascadiaMono.ttf");

    while (Window.isOpen()) // Cât timp fereastra este deschisă, tot codul rulează la infinit.
    {
        Event event;
        while (Window.pollEvent(event)) // Verificăm event-ul curent.
        {
            if (event.type == Event::Closed)
            {
                Window.close();
            }
            
            //Daca este apasat click stanga
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousepos = Mouse::getPosition(Window);
                    updateCursorForMouse(mousepos, currentText, cursorPos, text);
                }
            }
            // Dacă event-ul curent presupune caractere date de la tastatură...
            if (event.type == Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    char key = static_cast<char>(event.text.unicode);

                    // Dacă se apasă backspace și se poate șterge...
                    if (key == 8 && !currentText.empty() && cursorPos > 0)
                    {
                        currentText.erase(cursorPos - 1, 1);
                        cursorPos--; // Devansăm cu o poziție după ștergere.
                    }
                    else if (key >= 32 && key <= 126)
                    {
                        // key in [32, 126] implică caracterele alfanumerice, punctuații și paranteze, etc.
                        // După inserarea caracterului, se incrementează poziția cursorului.
                        currentText.insert(cursorPos, 1, key);
                        cursorPos++;
                    }
                    else if (key == 13)
                    {
                        // 13 corespude CR, adică ENTER.
                        cursorPos++;
                        currentText = prefix(currentText, cursorPos) + "\n" + suffix(currentText, cursorPos);
                    }
                }
                text.setString(currentText); // După ce se efectuează operațiile, actualizăm textul vizibil.
            }
            if (event.type == Event::KeyPressed) // Acest caz tratează tastele ce nu produc caractere.
            {
                if (event.key.code == Keyboard::Left && cursorPos > 0)
                {
                    cursorPos--;
                }
                if (event.key.code == Keyboard::Right && cursorPos < currentText.size())
                {
                    cursorPos++;
                }
				if (event.key.code == Keyboard::Up && cursorPos > 0) // Trecem la linia precedentă.
				{
					do
					{
						cursorPos--;
					} while (currentText[cursorPos] != '\n' && cursorPos - 1 > 0);
				}
				if (event.key.code == Keyboard::Down && cursorPos < currentText.size()) // Trecem la linia următoare.
				{
					do
					{
						cursorPos++;
                    } while (currentText[cursorPos] != '\n' && cursorPos < currentText.size());
                    // BUG: Dacă ne aflăm pe penultima linie și incercăm să trecem la următoarea, dă crash.
                    //      Trebuie o condiție de oprire mai bună.
				}
                if (event.key.code == Keyboard::Equal && Keyboard::isKeyPressed(Keyboard::LControl)) // Zoom-in (CTRL + '=')
                {
                    fontSize = fontSize + 4;
                    text.setCharacterSize(fontSize);
                    cursor.setSize(Vector2f(2, text.getCharacterSize()));
                }
                if (event.key.code == Keyboard::Dash && Keyboard::isKeyPressed(Keyboard::LControl)) // Zoom-out (CTRL + '-')
                {
                    fontSize = (fontSize > 6) ? fontSize - 4 : fontSize;
                    text.setCharacterSize(fontSize);
                    cursor.setSize(Vector2f(2, text.getCharacterSize()));
                }
            }
        }

        // Se actualizează poziția cursorului.
        if (currentText.empty() == 0 && cursorPos > 0)
        {
            Vector2f pozitieNouaCursor = text.findCharacterPos(cursorPos); // Funcția findCharacterPos returnează un vector de
            cursor.setPosition(pozitieNouaCursor.x, pozitieNouaCursor.y);  // 2 floaturi care conține poziția caracterului de la cursorPos.
        }
        else // Dacă currentText este gol, documentul este gol. Atunci, cursorul este poziționat unde ar veni primul caracter.
        {
            cursor.setPosition(text.getPosition().x, text.getPosition().y);
        }
        
        // Se animează cursorul.
        cursorAnimate(cursor, timpAlternanta, esteVizibil);

        // Actualizăm window-ul.
        Window.clear(Color(COLOR_BG.r, COLOR_BG.g, COLOR_BG.b));
        Window.draw(text);
        Window.draw(cursor);
        Window.display();
    }
}

int main()
{
    RenderWindow EditorText(VideoMode(800, 800), "Editor Text", Style::Default);
    EditorText.setFramerateLimit(60);

    handleKeyboardInput(EditorText);

    return 0;
}