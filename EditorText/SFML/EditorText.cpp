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

// Funcție ce returnează un font. (Mai mult pentru a nu tasta font.loadFromFile() de fiecare dată.)
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

// Funcție ce setează dimensiunea, fontul și culoarea unui text.
void setFont(Text& text, Font& font, int size, RGBColor color, string path)
{
    font = loadFont(path);

    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(Color(color.r, color.g, color.b));
    text.setPosition(10, 10);
}

// Funcție care evidențiază cuvântul curent.
void CurrentWord(string& currentText, int cursorPos, Text& text, Text& currWord) {

    int start = cursorPos;
    while (start > 0 && currentText != ' ')
    {
        start--;
    }

    int end = cursorPos;
    while (end < currentText.length() && currentText[end] != ' ')
    {
        end++;
    }

    string before = currentText.substr(0, start);
    string current = currentText.substr(start, end - start);
    string after = currentText.substr(end);

    text.setString(before + current + after);
    currWord.setString(current);
    Vector2f startPos = text.findCharacterPos(start);
    currWord.setPosition(startPos);
}

// Funcție care primește informații de la tastatură și modifică documentul corespunzător.
void handleKeyboardInput(RenderWindow& Window)
{
    string currentText = "";
    int cursorPos = 0;
    Font font;
    Text text, CuvCurent;
    setFont(text, font, 24, { 255, 128, 64 }, "Fonts/CascadiaMono.ttf");
    setFont(CuvCurent, font, 24, { 255, 255 , 255 }, "Fonts/CascadiaMono.ttf");
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
            if (event.type == Event::TextEntered) {
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
                }
                text.setString(currentText); // După ce se efectuează operațiile, actualizăm textul vizibil.
            }
            if (event.type == Event::KeyPressed) // Acest caz tratează tastele ce nu produc caractere.
            {
                if (event.key.code == Keyboard::Left && cursorPos > 0)
                {
                    cursorPos--;
                }
                else if (event.key.code == Keyboard::Right && cursorPos < currentText.size())
                {
                    cursorPos++;
                }
            }
        }

        CurrentWord(currentText, cursorPos, text, CuvCurent);

        // Clear and draw updated text
        Window.clear(Color::Black);
        Window.draw(text);
        Window.display();
    }
}

int main()
{
    RenderWindow EditorText(VideoMode(800, 800), "Editor Text", sf::Style::Default);
    EditorText.setFramerateLimit(60);

    handleKeyboardInput(EditorText);

    return 0;
}