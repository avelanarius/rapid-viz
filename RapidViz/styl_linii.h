#pragma once

#include "ksztalt.h"

/**
 * \brief Klasa opisuj�ca dost�pne style linii (grot pocz�tkowy i ko�cowy).
 * 
 * Dost�pne s� 4 typy grot�w:
 * - BRAK
 * - KOLO
 * - TROJKAT
 * - KWADRAT
 * 
 * Ka�dy styl opisany jest w nast�puj�cym formacie: STYL_[Grot pocz�tkowy]_[Grot ko�cowy].
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
	* \brief Zwraca unikalny numer okre�laj�cy styl linii.
	* \return id stylu linii
	*/
	int getId() const;

	bool operator==(const StylLinii& innyStyl) const;

	bool operator!=(const StylLinii& innyStyl) const;

	/**
	 * \brief Zwraca obiekt typu Ksztalt okre�laj�cy grot pocz�tkowy linii.
	 * \return obiekt typu Ksztalt okre�laj�cy grot pocz�tkowy linii.
	 */
	Ksztalt ksztaltPoczatkowy() const;

	/**
	* \brief Zwraca obiekt typu Ksztalt okre�laj�cy grot ko�cowy linii.
	* \return obiekt typu Ksztalt okre�laj�cy grot ko�cowy linii.
	*/
	Ksztalt ksztaltKoncowy() const;
private:
	int idStylu = 0;
	StylLinii(int idStylu) : idStylu(idStylu) {}
};