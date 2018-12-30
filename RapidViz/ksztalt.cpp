#include "ksztalt.h"

const Ksztalt Ksztalt::BRAK = Ksztalt(-1);
const Ksztalt Ksztalt::KWADRAT = Ksztalt(0);
const Ksztalt Ksztalt::KOLO = Ksztalt(1);
const Ksztalt Ksztalt::TROJKAT = Ksztalt(2);
const Ksztalt Ksztalt::ROMB = Ksztalt(3);
const Ksztalt Ksztalt::PIECIOKAT = Ksztalt(4);
const Ksztalt Ksztalt::SZESCIOKAT = Ksztalt(5);
const Ksztalt Ksztalt::SIEDMIOKAT = Ksztalt(6);
const Ksztalt Ksztalt::OSMIOKAT = Ksztalt(7);
const Ksztalt Ksztalt::X = Ksztalt(8);
const Ksztalt Ksztalt::A = Ksztalt(9);
const Ksztalt Ksztalt::B = Ksztalt(10);
const Ksztalt Ksztalt::CYFRA_1 = Ksztalt(11);
const Ksztalt Ksztalt::CYFRA_2 = Ksztalt(12);
const Ksztalt Ksztalt::HASH = Ksztalt(13);
const Ksztalt Ksztalt::DRZEWO = Ksztalt(14);
const Ksztalt Ksztalt::GWIAZDA_5 = Ksztalt(15);
const Ksztalt Ksztalt::GWIAZDA_8 = Ksztalt(16);
const Ksztalt Ksztalt::GWIAZDA_3 = Ksztalt(17);
const Ksztalt Ksztalt::KWADRAT_OBRAMOWANIE = Ksztalt(18);
const Ksztalt Ksztalt::TROJKAT_OBRAMOWANIE = Ksztalt(19);
const Ksztalt Ksztalt::LUDZIK = Ksztalt(20);
const Ksztalt Ksztalt::ZNAK_ZAPYTANIA = Ksztalt(21);
const Ksztalt Ksztalt::WYKRZYKNIK = Ksztalt(22);
const Ksztalt Ksztalt::DOM = Ksztalt(23);
const Ksztalt Ksztalt::PLOT = Ksztalt(24);
const Ksztalt Ksztalt::TARCZA = Ksztalt(25);
const Ksztalt Ksztalt::PLUS = Ksztalt(26);
const Ksztalt Ksztalt::DOLAR = Ksztalt(27);
const Ksztalt Ksztalt::GORY = Ksztalt(28);
const Ksztalt Ksztalt::GRADIENT1 = Ksztalt(29);
const Ksztalt Ksztalt::GRADIENT2 = Ksztalt(30);
const Ksztalt Ksztalt::KOLO_GRADIENT = Ksztalt(31);
const Ksztalt Ksztalt::CYFRA_3 = Ksztalt(32);
const Ksztalt Ksztalt::CYFRA_4 = Ksztalt(33);
const Ksztalt Ksztalt::CYFRA_5 = Ksztalt(34);
const Ksztalt Ksztalt::CYFRA_6 = Ksztalt(35);
const Ksztalt Ksztalt::CYFRA_7 = Ksztalt(36);
const Ksztalt Ksztalt::CYFRA_8 = Ksztalt(37);
const Ksztalt Ksztalt::CYFRA_9 = Ksztalt(38);
const Ksztalt Ksztalt::CYFRA_0 = Ksztalt(39);
const Ksztalt Ksztalt::MINUS = Ksztalt(40);

int Ksztalt::getId() const {
	return idKsztaltu;
}

sf::FloatRect Ksztalt::getTekstura() const {
	return TEKSTURY[this->getId()];
}

bool Ksztalt::operator==(const Ksztalt& innyKsztalt) const {
	return idKsztaltu == innyKsztalt.getId();
}

bool Ksztalt::operator!=(const Ksztalt& innyKsztalt) const {
	return !(*this == innyKsztalt);
}