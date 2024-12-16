#include "menuFuncs.h"
#include "textStructs.h"
#include <iostream>
#include <string.h>

void fullscreenMenu::init()
{
	this->buttonCount = 0;
	this->buttonLabel[0][0] = '\0';
}

void fullscreenMenu::addButton(const char* label)
{
	strcpy_s(buttonLabel[buttonCount], label);
	buttonCount++;
}

void fullscreenMenu::update(sf::RenderWindow& window, sf::Font& font)
{
	background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	background.setPosition(0, 0);
	background.setFillColor(sf::Color(0, 0, 0, 128));

	margin = 5;
	border = 2;

	buttonHeight = font.getGlyph('A', 30, false).bounds.height * 2 + 2 * margin;
	
	// Calculăm lățimea maximă a butoanelor.
	float maxLabel = 0;
	// Luăm lățimea lui 'A' ca fiind lățimea unui caracter, și adăugăm 1 pixel pentru siguranță.
	float characterWidth = font.getGlyph('A', 30, false).bounds.width + 1;
	for (unsigned int i = 0; i < buttonCount; i++)
	{
		// Vedem lungimea maximă a unui text de buton.
		if (strlen(buttonLabel[i]) > maxLabel)
		{
			maxLabel = strlen(buttonLabel[i]);
		}
	}

	buttonWidth = maxLabel * characterWidth + 2 * margin;

	// Calculăm dimensiunea meniului.
	menuWidth = buttonWidth + 2 * border;
	menuHeight = buttonHeight * buttonCount + 2 * border;

	// Calculăm poziția și dimensiunea meniului.
	menuX = (window.getSize().x - menuWidth) / 2;
	menuY = (window.getSize().y - menuHeight) / 2;
	menu.setSize(sf::Vector2f(menuWidth, menuHeight));
	menu.setPosition(menuX, menuY);
	menu.setFillColor(sf::Color(COLOR_BG.r, COLOR_BG.b, COLOR_BG.b));

	// Calculăm poziția și dimensiunea butoanelor.
    for (unsigned int i = 0; i < buttonCount; i++)
    {
        button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
        button[i].setPosition(menuX + border, menuY + border + i * buttonHeight);
        button[i].setFillColor(sf::Color(COLOR_BG.r, COLOR_BG.b, COLOR_BG.b));
        button[i].setOutlineColor(sf::Color(COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b));
        button[i].setOutlineThickness(border);
        
        buttonText[i].setFont(font);
        buttonText[i].setString(buttonLabel[i]);
        buttonText[i].setCharacterSize(30);
        buttonText[i].setPosition(menuX + border + margin, menuY + border + i * buttonHeight + margin);
        buttonText[i].setFillColor(sf::Color(COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b));
    }
}

void fullscreenMenu::draw(sf::RenderWindow& window, sf::Font& font)
{
	// Desenăm meniul.
	window.draw(background);
	window.draw(menu);

	// Desenăm butoanele.
	for (unsigned int i = 0; i < buttonCount; i++)
	{
		window.draw(button[i]);
		window.draw(buttonText[i]);
	}
}

int fullscreenMenu::getClickedButton(sf::Vector2i mousePos)
{
	for (int i = 0; i < buttonCount; i++)
	{
		if (button[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
		{
			return i;
		}
	}
	return -1;
}

void handleMenuInput(sf::RenderWindow& Window, sf::Font& font, fullscreenMenu& menu)
{
	sf::Event event;
	std::cout << "s-a ajuns pe aici" << std::endl;
	while (Window.pollEvent(event))
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			int clickedButton = menu.getClickedButton(sf::Mouse::getPosition(Window));
			if (clickedButton != -1)
			{
				switch (clickedButton)
				{
				case 0:
					// New
					break;
				case 1:
					// Open
					break;
				case 2:
					// Save
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
