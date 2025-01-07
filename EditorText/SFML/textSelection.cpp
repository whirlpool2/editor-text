#include "textSelection.h"
#include "textFuncs.h"
#include <iostream>
#include "textSelection.h"
#include "textFuncs.h"

//Varianta cu ancora, ar trebui sa functioneze cel mai ok

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}
void TextSelection::updateSelectedTextKeys(textDocument& doc, sf::Text& text, sf::Vector2i direction, bool isControlPressed) {
    //Daca nu e apasat niciun buton, selectia se intrerupe


    //Folosim o ancora pentru a calcula corect intervalele
    //Prin folosirea ancorei evit sa fac fel si fel de conditii pentru a calcula corect intervalele
    //pentru ca anchoreaza inceputul selectiei, trebuie doar sa fac minimul pentru a obtine inceputul selectiei si maximul pentru a obtine sfarsitul selectiei


    if (direction.x == 0 && direction.y == 0 && isControlPressed == 0)
    {
        std::cout << "SE RESETEAZA< DA" << std::endl;
        SelStart = doc.cursorPos;
        SelEnd = doc.cursorPos;
        isSelected = false;
        return;
    }

    //Daca e apasat shift, atunci continuam sau incepem selectia

   // Daca nu e selectat nimic(isSelected este 0 la fiecare inceput de selectie
   // in cazul acesta incepem selectia
    if (!isSelected)
    {
        //Ancora  retine inceputul selectiei
        selectionAnchor = doc.cursorPos;
        isSelected = true;//Marcam facptul ca a inceput selectia prin isSelected = true
    }

    // Mutam cursorul in functie de directie
    if (direction.x == -1 && doc.cursorPos > 0)
    {
        doc.cursorPos--;
    }
    else if (direction.x == 1 && doc.cursorPos < doc.charCount)
    {
        doc.cursorPos++;
    }
    else if (direction.y == -1)
    {
        moveCursorUp(doc);
    }
    else if (direction.y == 1)
    {
        moveCursorDown(doc);
    }

    // Facem update la pozitiile de selectie
    SelStart = min(selectionAnchor, doc.cursorPos);
    SelEnd = max(selectionAnchor, doc.cursorPos);

    // Verificam daca revenim la selectie de lungime 0 si daca putem continua
    isSelected = (SelStart != SelEnd);



    // Verific sa vad ca merge bine ca innebunesc
    std::cout << "CURSOR POS: " << doc.cursorPos << std::endl;
    //Verific daca nu se reseteaza Shift ul cand nu trebuie

    //Verific daca se selecteaza corect
    std::cout << "SELSTART: " << SelStart << " SELEND: " << SelEnd << std::endl << std::endl;
    
}

//De facut Hightlight ul pe 17.12.2024
//De facut copy paste pe 18.12.2024

//functie stergere




void TextSelection::drawHighLight(sf::RenderWindow& window, sf::Text& text, textDocument& doc) {
    //daca avem o selectie, atunci ii dam highlight
    if (isSelected) {
		sf::RectangleShape highlight;//cream un dreptunghi pentru highlight
		highlight.setFillColor(sf::Color(170, 210, 230, 128));//setam culoarea dreptunghiului, albastru transparent
		//parcurgem textul si punem highlight pe textul selectat
        float SelectSize = (text.getCharacterSize() / 2) * 1.2;//Pentru a avea highlightul o linie constanta
        for (unsigned long long i = SelStart; i < SelEnd; ++i) {
            sf::Vector2f position = text.findCharacterPos(i); //luam pozitia caracterului curent
			highlight.setPosition(position);//setam pozitia dreptunghiului de highlight
			highlight.setSize(sf::Vector2f(SelectSize, text.getCharacterSize() * 1.2f));//setam dimensiunile dreptunghiului
			window.draw(highlight);//desenam dreptunghiul, ca sa apara vizual pe ecran
        }
    }

}

void TextSelection::deleteSelectedText(textDocument& doc, sf::Text& text,
    sf::RectangleShape& cursorVisual, sf::Clock& cursorClock, bool& cursorVisible, sf::RenderWindow& window) {
   
    std::cout << "DA INTRU AICI BOULE" << std::endl;
    
    if (!isSelected or SelEnd <= SelStart){
        std::cout << "Nu este selectat text" << std::endl;
	   if (SelStart != SelEnd)
		   doc.deleteText(SelStart, SelEnd);
	   else
		   deleteCharInTextObject(&doc, text);
       return;
   }

   //Stergem Selectia
   doc.deleteText(SelStart, SelEnd);

   character* startChar = doc.getChar(SelStart - 1);
   character* nextChar = doc.getChar(SelStart);
   if (startChar != nullptr && nextChar != nullptr) {
       startChar->next = nextChar;
       nextChar->prev = startChar;
   }
   doc.cursorPos = SelEnd = SelStart;
   isSelected = false;
   updateTextObject(&doc, window, text);
   updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
    
 //      if (!selection.isSelected || selection.SelStart >= selection.SelEnd) {
	//	std::cout << "No text selected.\n" << selection.SelStart << "  " << selection.SelEnd << '\n';

	//if (selection.SelStart != selection.SelEnd)
	//		doc.deleteText(selection.SelStart, selection.SelEnd);
 //       else
 //           deleteCharInTextObject(&doc, text);

 //       return;
 //   }

 //   // Sterg textul selectat
 //   doc.deleteText(selection.SelStart, selection.SelEnd);

 //   // Fac legatura intre caracterul de start si urmatorul dupa cel sters pentru a nu avea crash-uri
 //   character* startChar = doc.getChar(selection.SelStart - 1);
 //   character* nextChar = doc.getChar(selection.SelStart);
 //   if (startChar != nullptr && nextChar != nullptr) {
 //       startChar->next = nextChar;
 //       nextChar->prev = startChar;
 //   }

	////// Actualizez pozitia cursorului cu cea de la startul selectiei
 //   doc.cursorPos = selection.SelStart;

 //   // Resetez parametrul pt verificarea textului selectat
 //   selection.isSelected = false;
 //   selection.SelStart = selection.SelEnd = doc.cursorPos;

 //   // Actualizez textul si cursorul
 //   updateTextObject(&doc, window, text);
 //   updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
}

void TextSelection::copyText(textDocument& doc) {
    //Daca nu e nimic selectat, nu avem ce copia
    if (!isSelected) {
        return;
    }
    //Daca e selectat ceva, atunci copiem textul selectat
    character* start = doc.getChar(SelStart);
    character* end = doc.getChar(SelEnd);
    character* currentchar = start;
    std::string TextToCopy;
	//Parcurgem teextul selectat si il punem intr-un string care va fi la final copiat in clipboard
    while (currentchar != nullptr and currentchar != end) {
        TextToCopy += currentchar->c;
        currentchar = currentchar->next;
    }

    sf::Clipboard::setString(TextToCopy);
}

void TextSelection::pasteText(textDocument& doc, sf::Text& text, sf::RenderWindow& window, sf::RectangleShape& cursorVisual,
    sf::Clock& cursorClock, bool& cursorVisible) {

    //Verificam daca avem un text selectat care trebuie inlocuit	
    if (isSelected) 
		doc.deleteText(SelStart, SelEnd);
    
    std::string textToPaste = sf::Clipboard::getString();


    //Daca nu e nimic in clipboard, nu avem ce lipi
    if (textToPaste.size() == 0)
        return;
	
    
    for (int iterator = 0; iterator < textToPaste.size(); iterator++) {
		insertCharInTextObject(&doc, text, textToPaste[iterator]);
	}
	//Actualizam textul si cursorul
	updateTextObject(&doc, window, text);
	updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);
	//Resetez selectia
	isSelected = false;
	SelStart = SelEnd = doc.cursorPos;
}



void TextSelection::updateSelectedTextMouse(textDocument& doc, sf::Text& text, sf::Vector2i mousePosition, bool isMousePressed,
    sf::RenderWindow& window, sf::RectangleShape& cursorVisual,
    sf::Clock& cursorClock, bool& cursorVisible) {
	if (!isMousePressed) {
		return;
	}
	if (!isSelected) {
		selectionAnchor = doc.cursorPos;
		isSelected = true;
	}
	//Updatez pozitia cursorului
	cursorClickPos(mousePosition, doc, text);
    SelStart = min(selectionAnchor, doc.cursorPos);
	SelEnd = max(selectionAnchor, doc.cursorPos);   
    //Verificam daca selectia e activa
    isSelected = (SelStart != SelEnd);
 
    updateTextObject(&doc, window, text);
    updateCursorVisual(doc, text, cursorVisual, cursorClock, cursorVisible);


}
