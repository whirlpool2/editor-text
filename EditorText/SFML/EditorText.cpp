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

//Vreau sa fac o functie pentru animatia unde se afla cursorul pe locul unde tastez

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

// Primește informații de la tastatură și modifică documentul corespunzător.
void handleKeyboardInput(RenderWindow& Window)
{
    string currentText = "";
    int cursorPos = 0;
    Font font;
    Text text;

    //Creez bara pt animatie cursor

    RectangleShape cursor;
    Clock timpAlternanta;
    bool esteVizibil = true;
    cursor.setFillColor(Color::White);
    cursor.setSize(Vector2f(2, text.getCharacterSize()));

    //pun cursorul pe pozitia initiala

    cursor.setPosition(text.getPosition().x, text.getPosition().y);

    int size = 24;

    setFont(text, font, 24, COLOR_TEXT, "Fonts/CascadiaMono.ttf");

    while (Window.isOpen())
    {
        Event event;
        while (Window.pollEvent(event)) // Verificăm event-ul curent.
        {
            if (event.type == Event::Closed)
            {
                Window.close();
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
				if (event.key.code == Keyboard::Up && cursorPos > 0)
				{
					do
					{
						cursorPos--;
					} while (currentText[cursorPos] != '\n' && cursorPos - 1 > 0);
				}
				if (event.key.code == Keyboard::Down && cursorPos < currentText.size())
				{
					do
					{
						cursorPos++;
                    } while (currentText[cursorPos] != '\n' && cursorPos < currentText.size());
				}
                if (event.key.code == Keyboard::Equal && Keyboard::isKeyPressed(Keyboard::LControl))
                {
                    size = size + 4;
                    text.setCharacterSize(size);
                    cursor.setSize(Vector2f(2, text.getCharacterSize()));
                }
                if (event.key.code == Keyboard::Dash && Keyboard::isKeyPressed(Keyboard::LControl))
                {
                    size = (size > 6) ? size - 4 : size;
                    text.setCharacterSize(size);
                    cursor.setSize(Vector2f(2, text.getCharacterSize()));
                }
            }
        }

        //Actualizez pozitie cursor
        if (currentText.empty() == 0 && cursorPos > 0) {
            Vector2f pozitieNouaCursor = text.findCharacterPos(cursorPos);
            cursor.setPosition(pozitieNouaCursor.x, pozitieNouaCursor.y);
        }
        else
            cursor.setPosition(text.getPosition().x, text.getPosition().y);
        //aici trebuie rezolvat o problema importanta , daca incerc sa apas repetat tasta left arrow de multe ori , ajung pe prima pozitie
        // dupa pe cele negative si da crash din cauza asta(nu este alocat in memorie pe pozitiile de dincolo, trb conditie de oprire
        //Punem animatia la cursor 
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