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
 * \brief G³ówna klasa obs³uguj¹ca wizualiacjê.
 * 
 * Obiekt wizualizacji dzia³a na osobnym w¹tku. Klasa jest bezpieczna wielow¹tkowo.
 * 
 * ## Interakcja z wizualizacj¹
 * 
 * ### 1. Wyœwietlanie ElementWiz:
 * 
 * Poprzez metodê setNoweElementyWiz() mo¿liwe jest ustawienie nowych elementów,
 * które bêd¹ widoczne na ekranie. Poszczególny ElementWiz zawiera pozycjê,
 * wygl¹d, kolor, jak równie¿ opis widoczny dla u¿ytkownika.
 * 
 * ### 2. Status:
 * 
 * Metoda setStatus pozwala na ustawienie tekstu widocznego w górnej belce interfejsu.
 * 
 * ### 3. Klikniêcia, komendy, klawisze wpisane przez u¿ytkownika wizualizacji:
 * 
 * Poprzez wywo³anie metod getKlikniete(), getKomendy(), getKlawisze() mo¿liwe jest 
 * przechwycenie wszystkich zdarzeñ, które nast¹pi³y od poprzedniego wywo³ania
 * danej funkcji. 
 * Pozwala to na reakcjê na wystêpuj¹ce interakcje u¿ytkownika.
 */
class Wizualizacja {
public:
	/**
	 * \brief Tworzy obiekt wizualizacji, otwieraj¹c jedno okno o podanych wymiarach.
	 * \param szerokosc szerokoœæ pocz¹tkowego okna
	 * \param wysokosc wysokoœæ pocz¹tkowego okna
	 */
	Wizualizacja(int szerokosc, int wysokosc);

	/**
	 * \brief Zastêpuje aktualnie wyœwietlane ElementWiz nowymi elementami. 
	 * Nowe dane bêd¹ wyœwietlane w najbli¿szej klatce.
	 * \param noweElementyWiz vector przechowuj¹cy ElementWiz, które bêd¹ od tej pory wyœwietlane w wizualizacji
	 */
	void setNoweElementyWiz(std::vector<ElementWiz>& noweElementyWiz);
	/**
	 * \brief Zmienia aktualny status. 
	 * Status jest widoczny w górnej belce, stanowi informacjê dla u¿ytkownika wizualizacji.
	 * \param status nowy status
	 */
	void setStatus(std::string status);

	/**
	 * \brief Zwraca elementy, które zosta³y klikniête prawym przyciskiem myszki od 
	 * czasu ostatniego wywo³ania funkcji (lub startu wizualizacji). 
	 * 
	 * Uwaga: funkcja mo¿e zwróciæ
	 * ElementWiz, który jest w aktualnej klatce ju¿ niewidoczny.
	 * \return klikniête elementy od ostatniego wywo³ania funkcji
	 */
	std::vector<ElementWiz> getKlikniete();
	/**
	 * \brief Zwraca wpisane komendy do wizualizacji od 
	 * czasu ostatniego wywo³ania funkcji (lub startu wizualizacji).
	 * \return wpisane komendy od ostatniego wywo³ania funkcji
	 */
	std::vector<std::string> getKomendy();
	/**
	* \brief Zwraca naciœniete klawisze od
	* czasu ostatniego wywo³ania funkcji (lub startu wizualizacji).
	*  
	* Uwzglêdniane s¹ jedynie te klawisze, które nie s¹ u¿ywane przez interfejs wizualizacji.
	* \return naciœniête klawisze od ostatniego wywo³ania funkcji
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
