#pragma once
#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <queue>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

class OknoWizualizacji {
public:
	OknoWizualizacji(int szerokosc, int wysokosc, sf::ContextSettings settings);

	sf::RenderWindow window;
	int szerokosc;
	int wysokosc;

	sf::View widok;
	sf::View widokMini;
	float skala;

	bool czyMinimapa;

	bool trybKomend;
	std::string komenda;

	bool czyDrag;
	bool ignorujKlikniecie;

	sf::Vector2i ostatniaPozycjaMyszki;
	bool myszka;
	int zadaneNoweOkna = 0;

	int rysowaneObiekty = 0;
	int rysowaneTrojkaty = 0;

	unsigned long long aktualnyCzasMs;

	std::vector<std::pair<unsigned long long, std::string>> wpisaneKomendyAnimacja;

	std::string statusObiektu;

	sf::Texture* backgroundTexture;
	sf::Texture* statusBarTexture;
	sf::Texture* statusTexture;
	sf::Texture* selectedTexture;
	sf::Texture* minimapButtonTexture;
	sf::Texture* setMinimapButtonTexture;
	sf::Texture* commandModeButtonTexture;
	sf::Texture* shapesTexture;
	sf::Texture* newWindowButtonTexture;

	sf::Font font;
	sf::Font fontBold;

	GLuint vertexBuffer;
	size_t liczbaTrojkatow;

	const sf::IntRect minimapButton = {7, 6, 36, 36};
	const sf::IntRect setMinimapButton = {50, 6, 36, 36};
	const sf::IntRect commandModeButton = {93, 6, 36, 36};
	const sf::IntRect newWindowButton = {136, 6, 36, 36};

	std::string status;
	bool migajacaKomenda;

	std::queue<std::string> komendy;
	std::queue<char> klawisze;

	void ustawDomyslneWidoki();
	void ustawIkone(sf::Image icon);

	void obsluzZdarzenia(sf::Event event);

	void obsluzKomendyKlawiaturowe(sf::Event::KeyEvent keyEvent);
	void obsluzWpisanieTekstu(sf::Event::TextEvent textEvent);
	void obsluzWcisniecieMyszki(sf::Event::MouseButtonEvent mouseButtonEvent);
	void obsluzKolkoMyszki(sf::Event::MouseWheelScrollEvent mouseWheelScrollEvent);
	void obsluzZmianeRozmiaruOkna(sf::Event::SizeEvent sizeEvent);

	void obsluzLewyPrzyciskMyszy();

	void rysujTlo();
	void rysujGlownyWidok();
	
	void rysujHUD();
	void rysujMinimape();
private:
	void rysujPrzycisk(sf::IntRect position, sf::Texture* texture);
	void rysujAnimacjeHUD();
};
