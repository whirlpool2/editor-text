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
        throw("Font " + path + " could not be found.");
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

// Ia input de la tastatură.
void handleKeyboardInput(RenderWindow& Window)
{
    string currentText = "";

    Font font;
    Text text;
    setFont(text, font, 24, { 255, 128, 64 }, "Fonts/CascadiaMono.ttf");

    while (Window.isOpen()) {
        Event event;
        while (Window.pollEvent(event)) {
            if (event.type == Event::Closed)
                Window.close();

            // Handle keyboard input for characters
            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char key = static_cast<char>(event.text.unicode);
                    if (key == 8 && !currentText.empty()) {
                        // Handle backspace
                        currentText.pop_back();
                    }
                    else if (key >= 32 && key <= 126) {
                        // Add printable characters
                        currentText += key;
                    }
                }
                text.setString(currentText); // Update the displayed text
            }
        }
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