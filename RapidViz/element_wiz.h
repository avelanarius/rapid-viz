#pragma once
#include "ksztalt.h"
#include "styl_linii.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <iomanip>
	
/**
 * \brief Klasa opisuje obiekt widoczny w oknie wizualizacji, taki jak kszta�t lub linia.
 * 
 * Dost�pne s� trzy typy ElementWiz:
 * - ksztalt: jedna z figur geometrycznych dost�pnych w klasie Ksztalt (m.in. kwadrat, ko�o, tr�jk�t, drzewo)
 * - linia: linia pomi�dzy dwoma punktami, z mo�liwo�ci� ustawienia grot�w (StylLinii)
 * - liczba: ca�kowita liczba wype�niaj�ca zadany prostok�t.
 */
class ElementWiz {
public:
	/**
	 * \brief Tworzy obiekt linii (bez grot�w na obu ko�cach).
	 * \param pozycjaOd wsp�rz�dna pocz�tku linii
	 * \param pozycjaDo wsp�rz�dna ko�ca linii
	 * \param grubosc grubo�� linii
	 * \param kolor kolor wype�nienia
	 * \param opis opis elementu
	 * \return obiekt linii typu ElementWiz
	 */
	static ElementWiz linia(std::pair<float, float> pozycjaOd, std::pair<float, float> pozycjaDo, float grubosc, sf::Color kolor,
	                        std::string opis);

	/**
	 * \brief Tworzy obiekt linii z podanym stylem grot�w.
	 * \param pozycjaOd wsp�rz�dna pocz�tku linii
	 * \param pozycjaDo wsp�rz�dna ko�ca linii
	 * \param grubosc grubo�� linii
	 * \param stylLinii styl linii: opis grot�w na ko�cach linii
	 * \param kolor kolor wype�nienia
	 * \param opis opis elementu
	 * \return obiekt linii typu ElementWiz
	 */
	static ElementWiz linia(std::pair<float, float> pozycjaOd, std::pair<float, float> pozycjaDo, float grubosc, StylLinii stylLinii, sf::Color kolor,
		std::string opis);

	/**
	 * \brief Tworzy obiekt o podanym kszta�cie i kwadratowych wymiarach (klasa Ksztalt).
	 * \param tekstura typ kszta�tu (Ksztalt)
	 * \param pozycja wsp�rz�dna lewego g�rnego rogu obiektu
	 * \param rozmiar wymiary szeroko�ci i wysoko�ci
	 * \param kolor kolor wype�nienia
	 * \param opis opis elementu
	 * \return obiekt kszta�tu typu ElementWiz
	 */
	static ElementWiz ksztalt(Ksztalt tekstura, std::pair<float, float> pozycja,
	                          float rozmiar, sf::Color kolor,
	                          std::string opis);

	/**
	* \brief Tworzy obiekt o podanym kszta�cie i wymiarach (klasa Ksztalt).
	* \param tekstura typ kszta�tu (Ksztalt)
	* \param pozycja wsp�rz�dna lewego g�rnego rogu obiektu
	* \param rozmiar para liczb okre�laj�ca wymiary szeroko�ci i wysoko�ci
	* \param kolor kolor wype�nienia
	* \param opis opis elementu
	* \return obiekt kszta�tu typu ElementWiz
	*/
	static ElementWiz ksztalt(Ksztalt tekstura, std::pair<float, float> pozycja,
	                          std::pair<float, float> rozmiar, sf::Color kolor,
	                          std::string opis);

	/**
	* \brief Tworzy liczb� o podanym kszta�cie i kwadratowych wymiarach.
	* \param liczba wy�wietlona liczba
	* \param pozycja wsp�rz�dna lewego g�rnego rogu liczby
	* \param rozmiar wymiary szeroko�ci i wysoko�ci
	* \param kolor kolor wype�nienia
	* \param opis opis elementu
	* \return obiekt kszta�tu typu ElementWiz
	*/
	static ElementWiz liczba(int liczba, std::pair<float, float> pozycja,
		float rozmiar, sf::Color kolor,
		std::string opis);

	/**
	* \brief Tworzy liczb� o podanym kszta�cie i wymiarach.
	* \param liczba wy�wietlona liczba
	* \param pozycja wsp�rz�dna lewego g�rnego rogu liczby
	* \param rozmiar para liczb okre�laj�ca wymiary szeroko�ci i wysoko�ci
	* \param kolor kolor wype�nienia
	* \param opis opis elementu
	* \return obiekt kszta�tu typu ElementWiz
	*/
	static ElementWiz liczba(int liczba, std::pair<float, float> pozycja,
		std::pair<float, float> rozmiar, sf::Color kolor,
		std::string opis);

	/**
	 * \brief Zwraca opis elementu sk�adaj�cy si� z pola opis, wlasciwosciString i wlasciwosciLongLong,
	 * oddzielony przecinkami i �rednikami: "opis; wlasciowsc1: wartosc, wlasciwosc2: wartosc; wlasciwosc3: wartosc;"
	 * \return opis tekstowy
	 */
	std::string get_opis();

	/**
	 * \brief Dodaje wierzcho�ki i ich opis do vectora, w nast�puj�cej postaci:
	 * - 6 punkt�w, okre�laj�cych 2 tr�jk�ty, w ka�dym:
	 *    - koordynaty X, Y
	 *    - kolor: R, G, B, A
	 *    - koordynaty tekstury: X, Y
	 * \param daneTrojkatow vector do kt�rego zostan� dodane w�a�ciwo�ci wierzcho�k�w
	 */
	void generujDaneTrojkatow(std::vector<float>& daneTrojkatow) const;

	/**
	 * \brief Opis s�owny danego elementu, widoczny dla u�ytkownika wizualizacji.
	 */
	std::string opis = "";

	/**
	 * \brief Minimalny prostok�t, kt�ry zawiera element.
	 */
	sf::FloatRect boundingBox = {0.0f, 0.0f, 0.0f, 0.0f};

	/**
	 * \brief Czy u�ytkownik mo�e wchodzi� w interakcj� z danym elementem z poziomu interfejsu graficznego.
	 */
	bool zaznaczalny = false;

	/**
	 * \brief Pozycja elementu. Przyk�adowo w przypadku kszta�tu jest to lewy g�rny r�g.
	 */
	std::pair<float, float> pozycja = {0.0f, 0.0f};

	/**
	 * \brief Pozycja ko�cowa. Przyk�adowo w przypadku linii jest to pozycja ko�ca linii.
	 */
	std::pair<float, float> pozycjaDo = {0.0f, 0.0f};

	/**
	 * \brief Rozmiar elementu. Przyk�adowo w przypadku kszta�tu jest to szeroko�� i wysoko�� wy�wietalnej grafiki.
	 */
	std::pair<float, float> skala = {0.0f, 0.0f};

	/**
	 * \brief Kolor w jakim element jest wy�wietlany.
	 */
	sf::Color kolor = sf::Color::White;

	/**
	 * \brief Okre�la wygl�d obiektu, w przypadku m.in. kszta�tu.
	 */
	Ksztalt tekstura = Ksztalt::KWADRAT;

	int typGrafiki = 0;
	int dodatkoweParametry = 0;
private:
	void generujDaneTrojkatowDlaKsztaltu(std::vector<float>& daneTrojkatow) const;
	void generujDaneTrojkatowDlaLinii(std::vector<float>& daneTrojkatow) const;
	inline void generujDaneTrojkatowDlaCzworokata(std::vector<float>& daneTrojkatow, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const;
	void generujDaneTrojkatowDlaCzworokata(std::vector<float>& daneTrojkatow, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, sf::FloatRect teksturaKoordynaty) const;
	void generujDaneTrojkatowDlaLiczby(std::vector<float>& daneTrojkatow) const;
};
