#include "styl_linii.h"

const StylLinii StylLinii::STYL_BRAK_BRAK = StylLinii(0);
const StylLinii StylLinii::STYL_BRAK_KOLO = StylLinii(1);
const StylLinii StylLinii::STYL_BRAK_TROJKAT = StylLinii(2);
const StylLinii StylLinii::STYL_BRAK_KWADRAT = StylLinii(3);
const StylLinii StylLinii::STYL_KOLO_BRAK = StylLinii(4);
const StylLinii StylLinii::STYL_KOLO_KOLO = StylLinii(5);
const StylLinii StylLinii::STYL_KOLO_TROJKAT = StylLinii(6);
const StylLinii StylLinii::STYL_KOLO_KWADRAT = StylLinii(7);
const StylLinii StylLinii::STYL_TROJKAT_BRAK = StylLinii(8);
const StylLinii StylLinii::STYL_TROJKAT_KOLO = StylLinii(9);
const StylLinii StylLinii::STYL_TROJKAT_TROJKAT = StylLinii(10);
const StylLinii StylLinii::STYL_TROJKAT_KWADRAT = StylLinii(11);
const StylLinii StylLinii::STYL_KWADRAT_BRAK = StylLinii(12);
const StylLinii StylLinii::STYL_KWADRAT_KOLO = StylLinii(13);
const StylLinii StylLinii::STYL_KWADRAT_TROJKAT = StylLinii(14);
const StylLinii StylLinii::STYL_KWADRAT_KWADRAT = StylLinii(15);

StylLinii StylLinii::fromId(int idStylu) {
	return StylLinii(idStylu);
}

int StylLinii::getId() const {
	return idStylu;
}

bool StylLinii::operator==(const StylLinii& innyStyl) const {
	return idStylu == innyStyl.getId();
}

bool StylLinii::operator!=(const StylLinii& innyStyl) const {
	return !(*this == innyStyl);
}

Ksztalt StylLinii::ksztaltPoczatkowy() const {
	auto idKsztaltu = idStylu / 4;
	switch (idKsztaltu) {
	default:
	case 0:
		return Ksztalt::BRAK;
	case 1:
		return Ksztalt::KOLO;
	case 2:
		return Ksztalt::TROJKAT;
	case 3:
		return Ksztalt::KWADRAT;
	}
}

Ksztalt StylLinii::ksztaltKoncowy() const {
	auto idKsztaltu = idStylu % 4;
	switch (idKsztaltu) {
	default:
	case 0:
		return Ksztalt::BRAK;
	case 1:
		return Ksztalt::KOLO;
	case 2:
		return Ksztalt::TROJKAT;
	case 3:
		return Ksztalt::KWADRAT;
	}
}