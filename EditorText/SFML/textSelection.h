#pragma once

#include "textStructs.h"
#include <SFML/Graphics.hpp>

struct TextSelection
{
	unsigned long long SelStart = 0;
	unsigned long long SelEnd = 0;

	//numar cate caractere sunt selectate, ca atunci
	unsigned long long charCountSelected = 0;
	unsigned long long selectionAnchor;
	//unsigned long long cursorPosSelected;
	//verific daca e seelctat

	bool isSelected = false;

	//Functie pentru a selecta textul
	void selectText(textDocument& doc, sf::Text& text, sf::Vector2i mousePos);

	//Functie pentru a deselecta textul
	void deselectText(textDocument& doc, sf::Text& text);

	//Functie pentru a copia textul selectat
	void copyText(textDocument& doc);

	//Functie pentru a sterge textul selectat
	void deleteText(textDocument& doc, sf::Text& text);

	//Functie pentru a lipi textul copiat
	void pasteText(textDocument& doc, sf::Text& text);


	//Functie pentru a actualiza textul selectat dupa shift + arrow keys
	void updateSelectedTextKeys(textDocument& doc, sf::Text& text, sf::Vector2i direction);

};
