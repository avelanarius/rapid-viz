#pragma once
#include "ksztalt.h"
#include "styl_linii.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <iomanip>
	
/**
 * \brief Klasa opisuje obiekt widoczny w oknie wizualizacji, taki jak kszta³t lub linia.
 * 
 * Dostêpne s¹ trzy typy ElementWiz:
 * - ksztalt: jedna z figur geometrycznych dostêpnych w klasie Ksztalt (m.in. kwadrat, ko³o, trójk¹t, drzewo)
 * - linia: linia pomiêdzy dwoma punktami, z mo¿liwoœci¹ ustawienia grotów (StylLinii)
 * - liczba: ca³kowita liczba wype³niaj¹ca zadany prostok¹t.
 */
class ElementWiz {
public:
	/**
	 * \brief Tworzy obiekt linii (bez grotów na obu koñcach).
	 * \param pozycjaOd wspó³rzêdna pocz¹tku linii
	 * \param pozycjaDo wspó³rzêdna koñca linii
	 * \param grubosc gruboœæ linii
	 * \param kolor kolor wype³nienia
	 * \param opis opis elementu
	 * \return obiekt linii typu ElementWiz
	 */
	static ElementWiz linia(std::pair<float, float> pozycjaOd, std::pair<float, float> pozycjaDo, float grubosc, sf::Color kolor,
	                        std::string opis);

	/**
	 * \brief Tworzy obiekt linii z podanym stylem grotów.
	 * \param pozycjaOd wspó³rzêdna pocz¹tku linii
	 * \param pozycjaDo wspó³rzêdna koñca linii
	 * \param grubosc gruboœæ linii
	 * \param stylLinii styl linii: opis grotów na koñcach linii
	 * \param kolor kolor wype³nienia
	 * \param opis opis elementu
	 * \return obiekt linii typu ElementWiz
	 */
	static ElementWiz linia(std::pair<float, float> pozycjaOd, std::pair<float, float> pozycjaDo, float grubosc, StylLinii stylLinii, sf::Color kolor,
		std::string opis);

	/**
	 * \brief Tworzy obiekt o podanym kszta³cie i kwadratowych wymiarach (klasa Ksztalt).
	 * \param tekstura typ kszta³tu (Ksztalt)
	 * \param pozycja wspó³rzêdna lewego górnego rogu obiektu
	 * \param rozmiar wymiary szerokoœci i wysokoœci
	 * \param kolor kolor wype³nienia
	 * \param opis opis elementu
	 * \return obiekt kszta³tu typu ElementWiz
	 */
	static ElementWiz ksztalt(Ksztalt tekstura, std::pair<float, float> pozycja,
	                          float rozmiar, sf::Color kolor,
	                          std::string opis);

	/**
	* \brief Tworzy obiekt o podanym kszta³cie i wymiarach (klasa Ksztalt).
	* \param tekstura typ kszta³tu (Ksztalt)
	* \param pozycja wspó³rzêdna lewego górnego rogu obiektu
	* \param rozmiar para liczb okreœlaj¹ca wymiary szerokoœci i wysokoœci
	* \param kolor kolor wype³nienia
	* \param opis opis elementu
	* \return obiekt kszta³tu typu ElementWiz
	*/
	static ElementWiz ksztalt(Ksztalt tekstura, std::pair<float, float> pozycja,
	                          std::pair<float, float> rozmiar, sf::Color kolor,
	                          std::string opis);

	/**
	* \brief Tworzy liczbê o podanym kszta³cie i kwadratowych wymiarach.
	* \param liczba wyœwietlona liczba
	* \param pozycja wspó³rzêdna lewego górnego rogu liczby
	* \param rozmiar wymiary szerokoœci i wysokoœci
	* \param kolor kolor wype³nienia
	* \param opis opis elementu
	* \return obiekt kszta³tu typu ElementWiz
	*/
	static ElementWiz liczba(int liczba, std::pair<float, float> pozycja,
		float rozmiar, sf::Color kolor,
		std::string opis);

	/**
	* \brief Tworzy liczbê o podanym kszta³cie i wymiarach.
	* \param liczba wyœwietlona liczba
	* \param pozycja wspó³rzêdna lewego górnego rogu liczby
	* \param rozmiar para liczb okreœlaj¹ca wymiary szerokoœci i wysokoœci
	* \param kolor kolor wype³nienia
	* \param opis opis elementu
	* \return obiekt kszta³tu typu ElementWiz
	*/
	static ElementWiz liczba(int liczba, std::pair<float, float> pozycja,
		std::pair<float, float> rozmiar, sf::Color kolor,
		std::string opis);

	/**
	 * \brief Zwraca opis elementu sk³adaj¹cy siê z pola opis, wlasciwosciString i wlasciwosciLongLong,
	 * oddzielony przecinkami i œrednikami: "opis; wlasciowsc1: wartosc, wlasciwosc2: wartosc; wlasciwosc3: wartosc;"
	 * \return opis tekstowy
	 */
	std::string get_opis();

	/**
	 * \brief Dodaje wierzcho³ki i ich opis do vectora, w nastêpuj¹cej postaci:
	 * - 6 punktów, okreœlaj¹cych 2 trójk¹ty, w ka¿dym:
	 *    - koordynaty X, Y
	 *    - kolor: R, G, B, A
	 *    - koordynaty tekstury: X, Y
	 * \param daneTrojkatow vector do którego zostan¹ dodane w³aœciwoœci wierzcho³ków
	 */
	void generujDaneTrojkatow(std::vector<float>& daneTrojkatow) const;

	/**
	 * \brief Opis s³owny danego elementu, widoczny dla u¿ytkownika wizualizacji.
	 */
	std::string opis = "";

	/**
	 * \brief Minimalny prostok¹t, który zawiera element.
	 */
	sf::FloatRect boundingBox = {0.0f, 0.0f, 0.0f, 0.0f};

	/**
	 * \brief Czy u¿ytkownik mo¿e wchodziæ w interakcjê z danym elementem z poziomu interfejsu graficznego.
	 */
	bool zaznaczalny = false;

	/**
	 * \brief Pozycja elementu. Przyk³adowo w przypadku kszta³tu jest to lewy górny róg.
	 */
	std::pair<float, float> pozycja = {0.0f, 0.0f};

	/**
	 * \brief Pozycja koñcowa. Przyk³adowo w przypadku linii jest to pozycja koñca linii.
	 */
	std::pair<float, float> pozycjaDo = {0.0f, 0.0f};

	/**
	 * \brief Rozmiar elementu. Przyk³adowo w przypadku kszta³tu jest to szerokoœæ i wysokoœæ wyœwietalnej grafiki.
	 */
	std::pair<float, float> skala = {0.0f, 0.0f};

	/**
	 * \brief Kolor w jakim element jest wyœwietlany.
	 */
	sf::Color kolor = sf::Color::White;

	/**
	 * \brief Okreœla wygl¹d obiektu, w przypadku m.in. kszta³tu.
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
