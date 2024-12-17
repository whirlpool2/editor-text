#pragma once

#include "textStructs.h"
#include <SFML/Graphics.hpp>

struct TextSelection
{
	unsigned long long SelStart = 0;
	unsigned long long SelEnd = 0;

	//numar cate caractere sunt selectate
	unsigned long long charCountSelected = 0;
	unsigned long long selectionAnchor;
	//unsigned long long cursorPosSelected;
	//verific daca e seelctat

	bool isSelected = false;


	//Functie pentru a copia textul selectat
	void copyText(textDocument& doc);

	//Functie pentru a sterge textul selectat
	void deleteSelectedText(textDocument& doc, sf::Text& text, TextSelection& selection,
		sf::RectangleShape& cursorVisual, sf::Clock& cursorClock, bool& cursorVisible, sf::RenderWindow& window);

	//Functie pentru a lipi textul copiat
	void pasteText(textDocument& doc, sf::Text& text);


	//Functie pentru a actualiza textul selectat dupa shift + arrow keys
	void updateSelectedTextKeys(textDocument& doc, sf::Text& text, sf::Vector2i direction);

	//Functie pentru a da highlight la text
	void drawHighLight(sf::RenderWindow& window, sf::Text& text, textDocument& doc);

};
