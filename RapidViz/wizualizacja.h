#pragma once
#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>
#include <memory>
#include <map>
#include <mutex>
#include <queue>
#include <iostream>
#include <future>

#include "okno_wizualizacji.h"
#include "element_wiz.h"


/**
 * \brief G��wna klasa obs�uguj�ca wizualiacj�.
 * 
 * Obiekt wizualizacji dzia�a na osobnym w�tku. Klasa jest bezpieczna wielow�tkowo.
 * 
 * ## Interakcja z wizualizacj�
 * 
 * ### 1. Wy�wietlanie ElementWiz:
 * 
 * Poprzez metod� setNoweElementyWiz() mo�liwe jest ustawienie nowych element�w,
 * kt�re b�d� widoczne na ekranie. Poszczeg�lny ElementWiz zawiera pozycj�,
 * wygl�d, kolor, jak r�wnie� opis widoczny dla u�ytkownika.
 * 
 * ### 2. Status:
 * 
 * Metoda setStatus pozwala na ustawienie tekstu widocznego w g�rnej belce interfejsu.
 * 
 * ### 3. Klikni�cia, komendy, klawisze wpisane przez u�ytkownika wizualizacji:
 * 
 * Poprzez wywo�anie metod getKlikniete(), getKomendy(), getKlawisze() mo�liwe jest 
 * przechwycenie wszystkich zdarze�, kt�re nast�pi�y od poprzedniego wywo�ania
 * danej funkcji. 
 * Pozwala to na reakcj� na wyst�puj�ce interakcje u�ytkownika.
 */
class Wizualizacja {
public:
	/**
	 * \brief Tworzy obiekt wizualizacji, otwieraj�c jedno okno o podanych wymiarach.
	 * \param szerokosc szeroko�� pocz�tkowego okna
	 * \param wysokosc wysoko�� pocz�tkowego okna
	 */
	Wizualizacja(int szerokosc, int wysokosc);

	/**
	 * \brief Zast�puje aktualnie wy�wietlane ElementWiz nowymi elementami. 
	 * Nowe dane b�d� wy�wietlane w najbli�szej klatce.
	 * \param noweElementyWiz vector przechowuj�cy ElementWiz, kt�re b�d� od tej pory wy�wietlane w wizualizacji
	 */
	void setNoweElementyWiz(std::vector<ElementWiz>& noweElementyWiz);
	/**
	 * \brief Zmienia aktualny status. 
	 * Status jest widoczny w g�rnej belce, stanowi informacj� dla u�ytkownika wizualizacji.
	 * \param status nowy status
	 */
	void setStatus(std::string status);

	/**
	 * \brief Zwraca elementy, kt�re zosta�y klikni�te prawym przyciskiem myszki od 
	 * czasu ostatniego wywo�ania funkcji (lub startu wizualizacji). 
	 * 
	 * Uwaga: funkcja mo�e zwr�ci�
	 * ElementWiz, kt�ry jest w aktualnej klatce ju� niewidoczny.
	 * \return klikni�te elementy od ostatniego wywo�ania funkcji
	 */
	std::vector<ElementWiz> getKlikniete();
	/**
	 * \brief Zwraca wpisane komendy do wizualizacji od 
	 * czasu ostatniego wywo�ania funkcji (lub startu wizualizacji).
	 * \return wpisane komendy od ostatniego wywo�ania funkcji
	 */
	std::vector<std::string> getKomendy();
	/**
	* \brief Zwraca naci�niete klawisze od
	* czasu ostatniego wywo�ania funkcji (lub startu wizualizacji).
	*  
	* Uwzgl�dniane s� jedynie te klawisze, kt�re nie s� u�ywane przez interfejs wizualizacji.
	* \return naci�ni�te klawisze od ostatniego wywo�ania funkcji
	*/
	std::vector<char> getKlawisze();
private:
	std::vector<ElementWiz> elementyWiz;
	std::queue<ElementWiz> klikniete;
	std::queue<std::string> komendy;
	std::queue<char> klawisze;
	std::mutex block;
	std::string status = "Brak statusu";
	bool czyZmienioneObiekty = false;

	std::vector<std::unique_ptr<OknoWizualizacji>> aktywneOkna;

	sf::ContextSettings settings;

	sf::Clock clock;
	sf::Clock fpsCounter;
	sf::Clock migajacaKomendaCounter;
	bool migajacaKomenda = false;

	std::string vendorName;
	std::string openglVersion;

	GLuint vertexBuffer;
	std::vector<float> daneTrojkatow;

	void obslugaOkna();

	static void inicjujGLEW();

	bool wczytajAssety();

	static bool wczytajImage(sf::Image& image, std::string filename);
	static bool wczytajFont(sf::Font& font, std::string filename);
	static bool wczytajTexture(sf::Texture& texture, std::string filename);

	void przekazAssetyDoOkna(OknoWizualizacji* oknoWizualizacji);
	void przekazDaneDoOknaKlatka(OknoWizualizacji* oknoWizualizacji);

	void aktualizujVertexBuffer();
	void stworzNoweOkno(OknoWizualizacji* okno);
	void obsluzZdarzeniaOkien();
	void obsluzPozycjeMyszkiOkien();
	void rysujOkna();
	void obsluzTimery();

	int szerokoscPoczatkowa;
	int wysokoscPoczatkowa;
	std::thread watek;

	sf::Image icon;
	sf::Font font;
	sf::Font fontBold;
	sf::Texture backgroundTexture;
	sf::Texture statusBarTexture;
	sf::Texture statusTexture;
	sf::Texture selectedTexture;
	sf::Texture minimapButtonTexture;
	sf::Texture setMinimapButtonTexture;
	sf::Texture commandModeButtonTexture;
	sf::Texture shapesTexture;
	sf::Texture newWindowButtonTexture;
};
