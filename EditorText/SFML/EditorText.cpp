#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>

using namespace std;
using namespace sf;
//etrtvervwervwervwervwerv
//test

//test !!!!!!!!!
//sigma on th wall

//test mihai
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

Font loadFont(string path)
{
    Font font;

    if (!(font.loadFromFile(path)))
    {
        //throw("Font " + path + " could not be found.");
        cerr << "Font " << path << " could not be found." << '\n';
    }

    return font;
}

void setFont(Text& text, Font& font, int size, RGBColor color, string path)
{
    font = loadFont(path);

    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(Color(color.r, color.g, color.b));
    text.setPosition(10, 10);
}

//fac o functie pentru a evidentia cuvantul curent pe care suntems
void CurrentWord(string& currentText, int cursorPos, Text& text, Text& currWord) {

    int start = cursorPos;
    while (start > 0 && currentText != ' ')
        start--;

    int end = cursorPos;
    while (end < currentText.length() && currentText[end] != ' ')
        end++;

    string before = currentText.substr(0, start);
    string current = currentText.substr(start, end - start);
    string after = currentText.substr(end);

    text.setString(before + current + after);
    currWord.setString(current);
    Vector2f startPos = text.findCharacterPos(start);
    currWord.setPosition(startPos);
}

// Ia input de la tastatură.
void handleKeyboardInput(RenderWindow& Window)
{
    string currentText = "";
    int cursorPos = 0;
    Font font;
    Text text, CuvCurent;
    setFont(text, font, 24, { 255, 128, 64 }, "Fonts/CascadiaMono.ttf");
    setFont(CuvCurent, font, 24, { 255, 255 , 255 }, "Fonts/CascadiaMono.ttf");
    while (Window.isOpen()) {
        Event event;
        while (Window.pollEvent(event)) {
            if (event.type == Event::Closed)
                Window.close();

            // Handle keyboard input for characters
            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char key = static_cast<char>(event.text.unicode);
                    if (key == 8 && !currentText.empty() && cursorPos > 0) {
                        // daca sterg caracterul scad si pozitia c
                        currentText.erase(cursorPos - 1, 1);
                        cursorPos--;
                    }
                    else if (key >= 32 && key <= 126) {
                        // adaug la pozitia cursorului caracterul de la tasta
                        currentText.insert(cursorPos, 1, key);
                        cursorPos++;
                    }
                }
                text.setString(currentText); // Update the displayed text
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Left && cursorPos > 0)
                    cursorPos--;
                else if (event.key.code == Keyboard::Right && cursorPos < currentText.size())
                    cursorPos++;
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
    EditorText.setFramerateLimit(144);

    handleKeyboardInput(EditorText);

    return 0;
}