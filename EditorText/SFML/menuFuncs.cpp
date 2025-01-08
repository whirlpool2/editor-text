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

void popup::init(sf::RenderWindow& window, sf::Font& font, unsigned int fontSize, const char* text)
{
	// Întunecăm fundalul.
	background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	background.setPosition(0, 0);
	background.setFillColor(sf::Color(0, 0, 0, 128));

	// Formăm textul și îl centrăm într-un box de dimensiune minimă.
	// Marginea va fi de 5 pixeli.
	strcpy_s(popupTextString, text);
	int margin = 5;
	popupText.setFont(font);
	popupText.setCharacterSize(fontSize);
	popupText.setString(popupTextString);
	popupText.setPosition((window.getSize().x - popupText.getGlobalBounds().width) / 2, (window.getSize().y - popupText.getGlobalBounds().height) / 2);

	// Dacă textul e mai lat decât ecranul, micșorăm fontul.
	while (popupText.getGlobalBounds().width > window.getSize().x)
	{
		fontSize = fontSize * 0.75;
		popupText.setCharacterSize(fontSize);
		popupText.setPosition((window.getSize().x - popupText.getGlobalBounds().width) / 2, (window.getSize().y - popupText.getGlobalBounds().height) / 2);
	}

	// Calculăm dimensiunea și poziția cutiei.
	width = popupText.getGlobalBounds().width + 2 * margin;
	height = popupText.getGlobalBounds().height + 2 * margin + fontSize / 2; // Aparent textul nu prea încape dacă nu adaug asta.
	x = (window.getSize().x - width) / 2;
	y = (window.getSize().y - height) / 2 + fontSize / 4; // Compensăm adăugarea aia de la height...
	popupArea.setSize(sf::Vector2f(width, height));
	popupArea.setPosition(x, y);
	popupArea.setFillColor(sf::Color(COLOR_BG.r, COLOR_BG.g, COLOR_BG.b));
	popupArea.setOutlineColor(sf::Color(COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b));
	popupArea.setOutlineThickness(1);
}

void popup::draw(sf::RenderWindow& window)
{
	window.draw(background);
	window.draw(popupArea);
	window.draw(popupText);
}

void inputBox::init(sf::RenderWindow& window, sf::Font& font, float width, float height, const char* desc)
{
	this->width = width;
	this->height = height;
	strcpy_s(descriptionText, desc);

	for (int i = 0; i < 128; i++)
	{
		inputText[i] = '\0';
	}

	inputTextLength = 0;

	background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	background.setPosition(0, 0);
	background.setFillColor(sf::Color(0, 0, 0, 128));

	inputArea.setSize(sf::Vector2f(width, height));
	inputArea.setPosition((window.getSize().x - width) / 2, (window.getSize().y - height) / 2);
	inputArea.setFillColor(sf::Color(COLOR_BG.r * 0.75, COLOR_BG.g * 0.75, COLOR_BG.b * 0.75));
	inputArea.setOutlineColor(sf::Color(COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b));
	inputArea.setOutlineThickness(2);

	description.setFont(font);
	description.setString(descriptionText);
	description.setPosition(inputArea.getPosition().x, inputArea.getPosition().y - description.getGlobalBounds().height - 20);

	input.setFont(font);
	input.setCharacterSize(height * 0.75);
	input.setString(inputText);
	input.setPosition(inputArea.getPosition().x + height / 8, inputArea.getPosition().y + height / 8);
}


// void (*function)(const char*)) e un pointer la o funcție este de tip void și ia ca argument un const char*.
// Aici folosim ca să știm cărei funcții trimitem textul introdus.
char* inputBox::handleInput(sf::Event event, bool& inputBoxActive, void (*function)(const char*))
{
	// std::cout << inputText << " (" << inputTextLength << ")" << std::endl;
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			inputBoxActive = false;
			return inputText;
		}
	}

	if (event.type == sf::Event::TextEntered)
	{
		std::cout << "Event text entered" << std::endl;
		update();
		if (event.text.unicode < 128)
		{
			char key = static_cast<char>(event.text.unicode);
			std::cout << key << std::endl;
			if (key == 8 && inputTextLength) // 8 = backspace
			{
				inputText[strlen(inputText) - 1] = '\0';
				inputTextLength--;
			}
			else if (key >= 32 && key <= 126) // 32-126 = caractere de la tastatura
			{
				inputText[inputTextLength] = key;
				inputTextLength++;
			}
			else if (key == 13) // 13 = enter
			{
				inputBoxActive = false;
				if (function != nullptr)
				{
					function(inputText);
				}
				return inputText;
			}
			update();
		}
	}
	return inputText;
}

void inputBox::update()
{
	input.setString(inputText);
	// input.setPosition(inputArea.getPosition().x + 10, inputArea.getPosition().y + 10);
}

void inputBox::draw(sf::RenderWindow& window, sf::Font& font)
{
	window.draw(background);
	window.draw(inputArea);
	window.draw(description);
	window.draw(input);
}