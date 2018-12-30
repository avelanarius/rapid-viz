#pragma once
#include <SFML/Graphics/Rect.hpp>

/**
 * \brief Klasa opisuj¹ca dostêpne kszta³ty, takie jak KOLO, KWADRAT.
 * 
 * Poszczególne typy dostêpne s¹ jako w³aœnoœæ statyczna, przyk³adowo: Ksztalt::KOLO, Ksztalt::KWADRAT. 
 * Istnieje kszta³t Ksztalt::BRAK okreœlaj¹cy nieprawid³owy kszta³t, brak kszta³tu.
 */
class Ksztalt {
public:
	static const Ksztalt BRAK;

	static const Ksztalt KWADRAT;
	static const Ksztalt KOLO;
	static const Ksztalt TROJKAT;
	static const Ksztalt ROMB;
	static const Ksztalt PIECIOKAT;
	static const Ksztalt SZESCIOKAT;
	static const Ksztalt SIEDMIOKAT;
	static const Ksztalt OSMIOKAT;
	static const Ksztalt X;
	static const Ksztalt A;
	static const Ksztalt B;
	static const Ksztalt CYFRA_1;
	static const Ksztalt CYFRA_2;
	static const Ksztalt HASH;
	static const Ksztalt DRZEWO;
	static const Ksztalt GWIAZDA_5;
	static const Ksztalt GWIAZDA_8;
	static const Ksztalt GWIAZDA_3;
	static const Ksztalt KWADRAT_OBRAMOWANIE;
	static const Ksztalt TROJKAT_OBRAMOWANIE;
	static const Ksztalt LUDZIK;
	static const Ksztalt ZNAK_ZAPYTANIA;
	static const Ksztalt WYKRZYKNIK;
	static const Ksztalt DOM;
	static const Ksztalt PLOT;
	static const Ksztalt TARCZA;
	static const Ksztalt PLUS;
	static const Ksztalt DOLAR;
	static const Ksztalt GORY;
	static const Ksztalt GRADIENT1;
	static const Ksztalt GRADIENT2;
	static const Ksztalt KOLO_GRADIENT;
	static const Ksztalt CYFRA_3;
	static const Ksztalt CYFRA_4;
	static const Ksztalt CYFRA_5;
	static const Ksztalt CYFRA_6;
	static const Ksztalt CYFRA_7;
	static const Ksztalt CYFRA_8;
	static const Ksztalt CYFRA_9;
	static const Ksztalt CYFRA_0;
	static const Ksztalt MINUS;

	/**
	 * \brief Zwraca unikalny numer okreœlaj¹cy typ kszta³tu.
	 * \return id kszta³tu
	 */
	int getId() const;
	/**
	 * \brief Zwraca pozycjê tekstury umo¿liwiaj¹cej wyrenderowanie kszta³tu.
	 * \return prostok¹t okreœlaj¹cy pozycjê w g³ównej teksturze kszta³tów
	 */
	sf::FloatRect getTekstura() const;

	bool operator==(const Ksztalt& innyKsztalt) const;
	bool operator!=(const Ksztalt& innyKsztalt) const;

private:
	int idKsztaltu = 0;
	Ksztalt(int idKsztaltu) : idKsztaltu(idKsztaltu) {}
};

const sf::FloatRect TEKSTURY[] = {
	{ 1.0f / 32.0f, 1.0f / 32.0f, 1.0f / 8.0f - 2.0f / 32.0f, 1.0f / 8.0f - 2.0f / 32.0f },
	{ 1.0f / 8.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 2.0f / 8.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 3.0f / 8.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 4.0f / 8.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 5.0f / 8.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 6.0f / 8.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 7.0f / 8.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 0.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 3.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 5.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 6.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 7.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 0.0f / 8.0f, 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 1.0f / 8.0f, 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 2.0f / 8.0f, 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 3.0f / 8.0f, 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 4.0f / 8.0f, 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 5.0f / 8.0f, 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 6.0f / 8.0f, 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 7.0f / 8.0f, 2.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 0.0f / 8.0f, 3.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 1.0f / 8.0f, 3.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 2.0f / 8.0f, 3.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 3.0f / 8.0f, 3.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 4.0f / 8.0f, 3.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 5.0f / 8.0f + 1.0f / 160.0f, 3.0f / 8.0f + 1.0f / 160.0f, 1.0f / 8.0f - 2.0f / 160.0f, 1.0f / 8.0f - 2.0f / 160.0f },
	{ 6.0f / 8.0f + 1.0f / 160.0f, 3.0f / 8.0f + 1.0f / 160.0f, 1.0f / 8.0f - 2.0f / 160.0f, 1.0f / 8.0f - 2.0f / 160.0f },
	{ 7.0f / 8.0f, 3.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 0.0f / 8.0f, 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 1.0f / 8.0f, 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 2.0f / 8.0f, 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 3.0f / 8.0f, 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 4.0f / 8.0f, 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 5.0f / 8.0f, 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 6.0f / 8.0f, 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 7.0f / 8.0f, 4.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f },
	{ 0.0f / 8.0f, 5.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f }
};