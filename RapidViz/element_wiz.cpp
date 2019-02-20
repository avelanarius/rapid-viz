#include "element_wiz.h"

ElementWiz ElementWiz::ksztalt(Ksztalt tekstura, std::pair<float, float> pozycja,
                               std::pair<float, float> rozmiar, sf::Color kolor,
                               std::string opis) {

    ElementWiz ew;
    ew.opis = opis;
    ew.boundingBox = sf::FloatRect(pozycja.first - rozmiar.first * 0.5f, pozycja.second - rozmiar.second * 0.5f, rozmiar.first, rozmiar.second);
    ew.zaznaczalny = true;

    ew.typGrafiki = 1;
    ew.pozycja = { pozycja.first - rozmiar.first * 0.5f, pozycja.second - rozmiar.second * 0.5f };
    ew.skala = rozmiar;
    ew.kolor = kolor;
    ew.tekstura = tekstura;
    return ew;
}

ElementWiz ElementWiz::liczba(int liczba, std::pair<float, float> pozycja, float rozmiar, sf::Color kolor, std::string opis) {
    return ElementWiz::liczba(liczba, pozycja, { rozmiar, rozmiar }, kolor, opis);
}

ElementWiz ElementWiz::liczba(int liczba, std::pair<float, float> pozycja, std::pair<float, float> rozmiar, sf::Color kolor, std::string opis) {

    ElementWiz ew;
    ew.opis = opis;
    ew.zaznaczalny = false;

    ew.typGrafiki = 3;
    ew.pozycja = { pozycja.first - rozmiar.first * 0.5f, pozycja.second - rozmiar.second * 0.5f };
    ew.skala = rozmiar;
    ew.kolor = kolor;
    ew.dodatkoweParametry = liczba;

    return ew;
}

ElementWiz ElementWiz::ksztalt(Ksztalt tekstura, std::pair<float, float> pozycja,
                               float rozmiar, sf::Color kolor,
                               std::string opis) {
    return ElementWiz::ksztalt(tekstura, pozycja, { rozmiar, rozmiar }, kolor, opis);
}

ElementWiz ElementWiz::linia(std::pair<float, float> pozycjaOd, std::pair<float, float> pozycjaDo, float grubosc, sf::Color kolor,
                             std::string opis) {
    return ElementWiz::linia(pozycjaOd, pozycjaDo, grubosc, StylLinii::STYL_BRAK_BRAK, kolor, opis);
}

ElementWiz ElementWiz::linia(std::pair<float, float> pozycjaOd, std::pair<float, float> pozycjaDo, float grubosc, StylLinii stylLinii, sf::Color kolor, std::string opis) {

    ElementWiz ew;
    ew.opis = opis;
    ew.zaznaczalny = false;

    ew.typGrafiki = 2;
    ew.pozycja = pozycjaOd;
    ew.pozycjaDo = pozycjaDo;
    ew.skala = { grubosc, grubosc };
    ew.kolor = kolor;
    ew.tekstura = Ksztalt::KWADRAT;

    ew.dodatkoweParametry = stylLinii.getId();
    return ew;
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6) {
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

std::string ElementWiz::get_opis() {
    return "x = " + to_string_with_precision(this->pozycja.first + this->skala.first * 0.5f, 2) +
        "; y = " + to_string_with_precision(this->pozycja.second + this->skala.second * 0.5f, 2)
        + "; " + this->opis;
}

void ElementWiz::generujDaneTrojkatow(std::vector<float>& daneTrojkatow) const {
    if (this->typGrafiki == 1) {
        generujDaneTrojkatowDlaKsztaltu(daneTrojkatow);
    } else if (this->typGrafiki == 2) {
        generujDaneTrojkatowDlaLinii(daneTrojkatow);
    } else if (this->typGrafiki == 3) {
        generujDaneTrojkatowDlaLiczby(daneTrojkatow);
    }
}


void ElementWiz::generujDaneTrojkatowDlaKsztaltu(std::vector<float>& daneTrojkatow) const {
    auto x1 = this->pozycja.first + this->skala.first;
    auto y1 = this->pozycja.second + this->skala.second;

    auto x2 = this->pozycja.first;
    auto y2 = y1;

    auto x3 = x2;
    auto y3 = this->pozycja.second;

    auto x4 = x1;
    auto y4 = y3;

    this->generujDaneTrojkatowDlaCzworokata(daneTrojkatow, x1, y1, x2, y2, x3, y3, x4, y4);
}

void ElementWiz::generujDaneTrojkatowDlaLinii(std::vector<float>& daneTrojkatow) const {
    auto x1 = this->pozycja.first;
    auto y1 = this->pozycja.second;
    auto x2 = this->pozycjaDo.first;
    auto y2 = this->pozycjaDo.second;
    auto g = this->skala.first;

    auto dlX = x1 - x2, dlY = y1 - y2;
    dlX *= dlX;
    dlY *= dlY;

    auto dl2_inv = 0.5f / sqrt(dlX + dlY);
    auto gdl2 = g * dl2_inv;

    auto gdl2x1x2 = gdl2 * (x1 - x2);
    auto gdl2y1y2 = gdl2 * (y1 - y2);

    auto x3 = x1 - gdl2y1y2;
    auto y3 = y1 + gdl2x1x2;

    auto x4 = x1 + gdl2y1y2;
    auto y4 = y1 - gdl2x1x2;

    auto x5 = x2 - gdl2y1y2;
    auto y5 = y2 + gdl2x1x2;

    auto x6 = x2 + gdl2y1y2;
    auto y6 = y2 - gdl2x1x2;

    this->generujDaneTrojkatowDlaCzworokata(daneTrojkatow, x5, y5, x6, y6, x4, y4, x3, y3);

    auto aktualnyStyl = StylLinii::fromId(this->dodatkoweParametry);

    if (aktualnyStyl != StylLinii::STYL_BRAK_BRAK) {

        auto dlugosc12_inv = dl2_inv * 2.0f;

        auto skala = 2.5f / 2.0f;
        auto dlugosc12_inv_g_skala = dlugosc12_inv * g * skala;

        auto deltaX1 = (x1 - x2) * dlugosc12_inv_g_skala;
        auto deltaY1 = (y1 - y2) * dlugosc12_inv_g_skala;

        auto deltaX2 = (x3 - x4) * skala;
        auto deltaY2 = (y3 - y4) * skala;

        auto ksztaltPoczatkowy = aktualnyStyl.ksztaltPoczatkowy();
        auto ksztaltKoncowy = aktualnyStyl.ksztaltKoncowy();

        if (ksztaltPoczatkowy != Ksztalt::BRAK) {

            auto xA = x1 + deltaX1 + deltaX2;
            auto yA = y1 + deltaY1 + deltaY2;

            auto xB = x1 + deltaX1 - deltaX2;
            auto yB = y1 + deltaY1 - deltaY2;

            auto xC = x1 - deltaX1 - deltaX2;
            auto yC = y1 - deltaY1 - deltaY2;

            auto xD = x1 - deltaX1 + deltaX2;
            auto yD = y1 - deltaY1 + deltaY2;

            this->generujDaneTrojkatowDlaCzworokata(daneTrojkatow, xB, yB, xA, yA, xD, yD, xC, yC, ksztaltPoczatkowy.getTekstura());
        }

        if (ksztaltKoncowy != Ksztalt::BRAK) {
            auto xA = x2 + deltaX1 + deltaX2;
            auto yA = y2 + deltaY1 + deltaY2;

            auto xB = x2 + deltaX1 - deltaX2;
            auto yB = y2 + deltaY1 - deltaY2;

            auto xC = x2 - deltaX1 - deltaX2;
            auto yC = y2 - deltaY1 - deltaY2;

            auto xD = x2 - deltaX1 + deltaX2;
            auto yD = y2 - deltaY1 + deltaY2;

            this->generujDaneTrojkatowDlaCzworokata(daneTrojkatow, xB, yB, xA, yA, xD, yD, xC, yC, ksztaltKoncowy.getTekstura());
        }
    }

}

void ElementWiz::generujDaneTrojkatowDlaLiczby(std::vector<float>& daneTrojkatow) const {
    auto liczbaCyfr = 0;
    auto liczba = this->dodatkoweParametry;
    auto ujemna = false;
    if (this->dodatkoweParametry < 0) {
        ujemna = true;
        liczbaCyfr++;
        liczba = -liczba;
    }
    do {
        liczba /= 10;
        liczbaCyfr++;
    } while (liczba != 0);

    auto wysokosc = 1.0f;
    auto szerokosc = static_cast<float>(liczbaCyfr);

    auto nowaWysokosc = this->skala.first / szerokosc * wysokosc;
    auto nowaSzerokosc = this->skala.second / wysokosc * szerokosc;

    if (nowaWysokosc > this->skala.second) {
        nowaWysokosc = this->skala.second;
    } else {
        nowaSzerokosc = this->skala.first;
    }

    auto srodekX = this->skala.first * 0.5f + this->pozycja.first;
    auto srodekY = this->skala.second * 0.5f + this->pozycja.second;

    auto rogX = srodekX - nowaSzerokosc * 0.5f;
    auto rogY = srodekY - nowaWysokosc * 0.5f;

    liczba = (ujemna ? -this->dodatkoweParametry : this->dodatkoweParametry);
    auto szerokoscCyfry = nowaSzerokosc / liczbaCyfr;

    auto aktX = rogX + (liczbaCyfr - 1) * szerokoscCyfry;
    do {
        auto aktualnaCyfra = liczba % 10;
        liczba /= 10;
        liczbaCyfr++;

        sf::FloatRect tekstura;
        switch (aktualnaCyfra) {
            case 0:
                tekstura = Ksztalt::CYFRA_0.getTekstura();
                break;
            case 1:
                tekstura = Ksztalt::CYFRA_1.getTekstura();
                break;
            case 2:
                tekstura = Ksztalt::CYFRA_2.getTekstura();
                break;
            case 3:
                tekstura = Ksztalt::CYFRA_3.getTekstura();
                break;
            case 4:
                tekstura = Ksztalt::CYFRA_4.getTekstura();
                break;
            case 5:
                tekstura = Ksztalt::CYFRA_5.getTekstura();
                break;
            case 6:
                tekstura = Ksztalt::CYFRA_6.getTekstura();
                break;
            case 7:
                tekstura = Ksztalt::CYFRA_7.getTekstura();
                break;
            case 8:
                tekstura = Ksztalt::CYFRA_8.getTekstura();
                break;
            case 9:
                tekstura = Ksztalt::CYFRA_9.getTekstura();
                break;
        }

        auto x1 = aktX + szerokoscCyfry;
        auto y1 = rogY + nowaWysokosc;

        auto x2 = aktX;
        auto y2 = y1;

        auto x3 = x2;
        auto y3 = rogY;

        auto x4 = x1;
        auto y4 = y3;

        this->generujDaneTrojkatowDlaCzworokata(daneTrojkatow, x1, y1, x2, y2, x3, y3, x4, y4, tekstura);

        aktX -= szerokoscCyfry;

    } while (liczba != 0);

    if (ujemna) {
        auto x1 = aktX + szerokoscCyfry;
        auto y1 = rogY + nowaWysokosc;

        auto x2 = aktX;
        auto y2 = y1;

        auto x3 = x2;
        auto y3 = rogY;

        auto x4 = x1;
        auto y4 = y3;

        this->generujDaneTrojkatowDlaCzworokata(daneTrojkatow, x1, y1, x2, y2, x3, y3, x4, y4, Ksztalt::MINUS.getTekstura());
    }
}

inline void ElementWiz::generujDaneTrojkatowDlaCzworokata(std::vector<float>& daneTrojkatow, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const {
    this->generujDaneTrojkatowDlaCzworokata(daneTrojkatow, x1, y1, x2, y2, x3, y3, x4, y4, this->tekstura.getTekstura());
}

void ElementWiz::generujDaneTrojkatowDlaCzworokata(std::vector<float>& daneTrojkatow, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, sf::FloatRect teksturaKoordynaty) const {
    auto floatR = static_cast<float>(this->kolor.r) / 255.0f;
    auto floatG = static_cast<float>(this->kolor.g) / 255.0f;
    auto floatB = static_cast<float>(this->kolor.b) / 255.0f;
    auto floatA = static_cast<float>(this->kolor.a) / 255.0f;

    daneTrojkatow.push_back(x3);
    daneTrojkatow.push_back(y3);

    daneTrojkatow.push_back(floatR);
    daneTrojkatow.push_back(floatG);
    daneTrojkatow.push_back(floatB);
    daneTrojkatow.push_back(floatA);

    daneTrojkatow.push_back(teksturaKoordynaty.left);
    daneTrojkatow.push_back(teksturaKoordynaty.top);

    daneTrojkatow.push_back(x4);
    daneTrojkatow.push_back(y4);

    daneTrojkatow.push_back(floatR);
    daneTrojkatow.push_back(floatG);
    daneTrojkatow.push_back(floatB);
    daneTrojkatow.push_back(floatA);

    daneTrojkatow.push_back(teksturaKoordynaty.left + teksturaKoordynaty.width);
    daneTrojkatow.push_back(teksturaKoordynaty.top);

    daneTrojkatow.push_back(x2);
    daneTrojkatow.push_back(y2);

    daneTrojkatow.push_back(floatR);
    daneTrojkatow.push_back(floatG);
    daneTrojkatow.push_back(floatB);
    daneTrojkatow.push_back(floatA);

    daneTrojkatow.push_back(teksturaKoordynaty.left);
    daneTrojkatow.push_back(teksturaKoordynaty.top + teksturaKoordynaty.height);

    daneTrojkatow.push_back(x1);
    daneTrojkatow.push_back(y1);

    daneTrojkatow.push_back(floatR);
    daneTrojkatow.push_back(floatG);
    daneTrojkatow.push_back(floatB);
    daneTrojkatow.push_back(floatA);

    daneTrojkatow.push_back(teksturaKoordynaty.left + teksturaKoordynaty.width);
    daneTrojkatow.push_back(teksturaKoordynaty.top + teksturaKoordynaty.height);

    daneTrojkatow.push_back(x2);
    daneTrojkatow.push_back(y2);

    daneTrojkatow.push_back(floatR);
    daneTrojkatow.push_back(floatG);
    daneTrojkatow.push_back(floatB);
    daneTrojkatow.push_back(floatA);

    daneTrojkatow.push_back(teksturaKoordynaty.left);
    daneTrojkatow.push_back(teksturaKoordynaty.top + teksturaKoordynaty.height);

    daneTrojkatow.push_back(x4);
    daneTrojkatow.push_back(y4);

    daneTrojkatow.push_back(floatR);
    daneTrojkatow.push_back(floatG);
    daneTrojkatow.push_back(floatB);
    daneTrojkatow.push_back(floatA);

    daneTrojkatow.push_back(teksturaKoordynaty.left + teksturaKoordynaty.width);
    daneTrojkatow.push_back(teksturaKoordynaty.top);
}
