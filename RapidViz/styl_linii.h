#pragma once

#include "ksztalt.h"

/**
 * \brief Klasa opisuj¹ca dostêpne style linii (grot pocz¹tkowy i koñcowy).
 * 
 * Dostêpne s¹ 4 typy grotów:
 * - BRAK
 * - KOLO
 * - TROJKAT
 * - KWADRAT
 * 
 * Ka¿dy styl opisany jest w nastêpuj¹cym formacie: STYL_[Grot pocz¹tkowy]_[Grot koñcowy].
 */
class StylLinii {
public:
	static const StylLinii STYL_BRAK_BRAK;
	static const StylLinii STYL_BRAK_KOLO;
	static const StylLinii STYL_BRAK_TROJKAT;
	static const StylLinii STYL_BRAK_KWADRAT;
	static const StylLinii STYL_KOLO_BRAK;
	static const StylLinii STYL_KOLO_KOLO;
	static const StylLinii STYL_KOLO_TROJKAT;
	static const StylLinii STYL_KOLO_KWADRAT;
	static const StylLinii STYL_TROJKAT_BRAK;
	static const StylLinii STYL_TROJKAT_KOLO;
	static const StylLinii STYL_TROJKAT_TROJKAT;
	static const StylLinii STYL_TROJKAT_KWADRAT;
	static const StylLinii STYL_KWADRAT_BRAK;
	static const StylLinii STYL_KWADRAT_KOLO;
	static const StylLinii STYL_KWADRAT_TROJKAT;
	static const StylLinii STYL_KWADRAT_KWADRAT;

	/**
	 * \brief Konstruuje styl linii o podanym idStylu
	 * \param idStylu unikalny numer stylu linii
	 * \return styl linii o podanym idStylu
	 */
	static StylLinii fromId(int idStylu);

	/**
	* \brief Zwraca unikalny numer okreœlaj¹cy styl linii.
	* \return id stylu linii
	*/
	int getId() const;

	bool operator==(const StylLinii& innyStyl) const;

	bool operator!=(const StylLinii& innyStyl) const;

	/**
	 * \brief Zwraca obiekt typu Ksztalt okreœlaj¹cy grot pocz¹tkowy linii.
	 * \return obiekt typu Ksztalt okreœlaj¹cy grot pocz¹tkowy linii.
	 */
	Ksztalt ksztaltPoczatkowy() const;

	/**
	* \brief Zwraca obiekt typu Ksztalt okreœlaj¹cy grot koñcowy linii.
	* \return obiekt typu Ksztalt okreœlaj¹cy grot koñcowy linii.
	*/
	Ksztalt ksztaltKoncowy() const;
private:
	int idStylu = 0;
	StylLinii(int idStylu) : idStylu(idStylu) {}
};