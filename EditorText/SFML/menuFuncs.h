#pragma once
#include <SFML/Graphics.hpp>

#define MAX_BUTTONS 32
#define MAX_BUTTON_TEXT 64

// Meniu plasat în centrul ecranului, dar care întunecă fundalul.
// Culoarea meniului și a butoanelor va fi COLOR_BG, iar textul butoanelor va lua culoarea COLOR_TEXT.
// Meniul va avea butoanele dispuse vertical, sub o formă de o listă.
// Butoanele vor avea o margine de 5 pixeli față de meniu și un contur de 2 pixeli de culoare COLOR_TEXT.
struct fullscreenMenu
{
	sf::RectangleShape background;
	sf::RectangleShape menu;
	sf::RectangleShape button[32];
	sf::Text buttonText[32];
	char buttonLabel[32][64];
	unsigned int buttonCount;
	float menuWidth;
	float menuHeight;
	float margin;
	float border;
	float buttonHeight;
	float buttonWidth;
	float menuX;
	float menuY;

	// Inițializează meniul, setând numărul de butoane la 0 și conținutul textului lor la nul.
	void init();

	// Adaugă un buton la meniu.
	void addButton(const char* label);

	// Actualizează conținutul meniului.
	void update(sf::RenderWindow&, sf::Font& font);

	// Desenează meniul pe ecran.
	// Dimensiunea meniului va fi minimă pentru a încăpea textul de pe butoane.
	void draw(sf::RenderWindow& window, sf::Font& font);

	// Returnează indicele butonului de pe care s-a dat click.
	// Dacă nu s-a dat click pe niciun buton, returnează -1.
	int getClickedButton(sf::Vector2i mousePos);
};

// Deschide meniul și așteaptă input de la utilizator.
// Momentan, detectarea deschiderii se face în funcția handleKeyboardInput (CTRL+M), iar ieșirea din meniu
// se face cu ESC, lucru detectat în această funcție.
void handleMenuInput(sf::RenderWindow& Window, sf::Font& font, fullscreenMenu& menu);