#include "wizualizacja_writer.h"
#include <iostream>

WizualizacjaWriter::WizualizacjaWriter(const std::string nazwa) {
    this->nazwa = nazwa + this->aktualnyCzas();
}

void WizualizacjaWriter::dodajStan(const std::vector<ElementWiz> stan) {
    this->wstawStan(stan);

    if (this->czyZapisac()) {
        this->zapisz();
    }
}

void WizualizacjaWriter::wstawStan(std::vector<ElementWiz> stan) {
    this->zapisaneStany.push_back(stan);
    const auto aktualnyCzas = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    this->czasy.push_back(aktualnyCzas);
    this->liczbaElementWiz += stan.size();
}

bool WizualizacjaWriter::czyZapisac() const {
    if (this->liczbaElementWiz == 0) return false;

    if (this->liczbaElementWiz > 600000) return true;

    const auto aktualnyCzas = std::chrono::system_clock::now();
    const auto czasOdZapisuMs = std::chrono::duration_cast<std::chrono::milliseconds>(aktualnyCzas - this->ostatniZapis).count();
    if (czasOdZapisuMs > 30 * 1000) return true;

    return false;
}

void WizualizacjaWriter::zapisz() {
    this->ostatniZapis = std::chrono::system_clock::now();
    const auto aktualnyNumer = this->numerBloku;
    this->numerBloku++;

    this->asyncZapisz(aktualnyNumer, this->zapisaneStany, this->czasy);

    this->zapisaneStany.clear();
    this->czasy.clear();
    this->liczbaElementWiz = 0;
}

void WizualizacjaWriter::usunTmp(std::string nazwaPliku) {
    std::rename((nazwaPliku + ".dat.tmp").c_str(), (nazwaPliku + ".dat").c_str());
    std::rename((nazwaPliku + ".list.tmp").c_str(), (nazwaPliku + ".list").c_str());
}

void WizualizacjaWriter::asyncZapisz(const int aktualnyNumer, const std::vector<std::vector<ElementWiz>>&_daneDoZapisu, const std::vector<unsigned long long>& _daneDoZapisuCzasy) {
    std::thread([&](const int aktualnyNumer, const std::vector<std::vector<ElementWiz>> daneDoZapisu, const std::vector<unsigned long long> daneDoZapisuCzasy) {
        std::string nazwaPliku = this->nazwa + std::to_string(aktualnyNumer);

        this->zapiszCzasy(nazwaPliku, daneDoZapisuCzasy);
        this->zapiszDane(nazwaPliku, daneDoZapisu);
        this->usunTmp(nazwaPliku);
    }, aktualnyNumer, _daneDoZapisu, _daneDoZapisuCzasy).detach();
}

void WizualizacjaWriter::zapiszCzasy(const std::string& nazwaPliku, const std::vector<unsigned long long>& czasyDoZapisu) const {
    std::ofstream output(nazwaPliku + ".list.tmp", std::ios::binary);

    unsigned long long liczbaRekordow = czasyDoZapisu.size();
    output.write(reinterpret_cast<const char *>(&liczbaRekordow), sizeof(liczbaRekordow));

    for (auto czas : czasyDoZapisu) {
        output.write(reinterpret_cast<const char *>(&czas), sizeof(czas));
    }

    output.close();
}

void WizualizacjaWriter::zapiszDane(const std::string& nazwaPliku, const std::vector<std::vector<ElementWiz>>& daneDoZapisu) const {
    std::ofstream output(nazwaPliku + ".dat.tmp", std::ios::binary);

    auto dane = this->serializujDane(daneDoZapisu);
    unsigned long long wielkoscDanychPrzedKompresja = dane.size();
    dane = this->kompresujDane(dane);
    unsigned long long wielkoscDanychPoKompresji = dane.size();

    output.write(reinterpret_cast<const char *>(&wielkoscDanychPrzedKompresja), sizeof(wielkoscDanychPrzedKompresja));
    output.write(reinterpret_cast<const char *>(&wielkoscDanychPoKompresji), sizeof(wielkoscDanychPoKompresji));

    output.write(dane.data(), wielkoscDanychPoKompresji);

    output.close();
}

std::vector<char> WizualizacjaWriter::serializujDane(const std::vector<std::vector<ElementWiz>>& daneDoZapisu) const {
    std::vector<char> dane;

    unsigned long long liczbaZbiorow = daneDoZapisu.size();

    this->writeToVector(dane, reinterpret_cast<char *>(&liczbaZbiorow), sizeof(liczbaZbiorow));

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        unsigned long long wielkoscZbioru = daneDoZapisu[zbior].size();

        this->writeToVector(dane, reinterpret_cast<char *>(&wielkoscZbioru), sizeof(wielkoscZbioru));
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            int dlugoscOpisu = daneDoZapisu[zbior][rekord].opis.size();

            this->writeToVector(dane, reinterpret_cast<char *>(&dlugoscOpisu), sizeof(dlugoscOpisu));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            int dlugoscOpisu = daneDoZapisu[zbior][rekord].opis.size();

            this->writeToVector(dane, daneDoZapisu[zbior][rekord].opis.c_str(), dlugoscOpisu);
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].boundingBox.left), sizeof(daneDoZapisu[zbior][rekord].boundingBox.left));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].boundingBox.top), sizeof(daneDoZapisu[zbior][rekord].boundingBox.top));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].boundingBox.height), sizeof(daneDoZapisu[zbior][rekord].boundingBox.height));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].boundingBox.width), sizeof(daneDoZapisu[zbior][rekord].boundingBox.width));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].zaznaczalny), sizeof(daneDoZapisu[zbior][rekord].zaznaczalny));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].pozycja.first), sizeof(daneDoZapisu[zbior][rekord].pozycja.first));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].pozycja.second), sizeof(daneDoZapisu[zbior][rekord].pozycja.second));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].pozycjaDo.first), sizeof(daneDoZapisu[zbior][rekord].pozycjaDo.first));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].pozycjaDo.second), sizeof(daneDoZapisu[zbior][rekord].pozycjaDo.second));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].skala.first), sizeof(daneDoZapisu[zbior][rekord].skala.first));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].skala.second), sizeof(daneDoZapisu[zbior][rekord].skala.second));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].kolor.r), sizeof(daneDoZapisu[zbior][rekord].kolor.r));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].kolor.g), sizeof(daneDoZapisu[zbior][rekord].kolor.g));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].kolor.b), sizeof(daneDoZapisu[zbior][rekord].kolor.b));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].kolor.a), sizeof(daneDoZapisu[zbior][rekord].kolor.a));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            auto teksturaId = daneDoZapisu[zbior][rekord].tekstura.getId();

            this->writeToVector(dane, reinterpret_cast<const char *>(&teksturaId), sizeof(teksturaId));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].typGrafiki), sizeof(daneDoZapisu[zbior][rekord].typGrafiki));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < daneDoZapisu[zbior].size(); rekord++) {
            this->writeToVector(dane, reinterpret_cast<const char *>(&daneDoZapisu[zbior][rekord].dodatkoweParametry), sizeof(daneDoZapisu[zbior][rekord].dodatkoweParametry));
        }
    }

    return dane;
}

void WizualizacjaWriter::writeToVector(std::vector<char>& vec, const char* dane, unsigned long long length) const {
    vec.insert(vec.end(), dane, dane + length);
}

std::vector<char> WizualizacjaWriter::kompresujDane(const std::vector<char>& dane) const {
    const int srcSize = dane.size();
    const int maxKompresowanySize = LZ4_compressBound(srcSize);

    std::vector<char> skompresowany(maxKompresowanySize);

    const int skompresowanySize = LZ4_compress_default(dane.data(), skompresowany.data(), dane.size(), maxKompresowanySize);

    if (skompresowanySize <= 0) {
        throw std::runtime_error("Blad kompresowania!");
    }

    skompresowany.resize(skompresowanySize);
    std::cout << "KOMPRESJA Z: " << srcSize / 1024.0 / 1024.0 << " DO " << skompresowanySize / 1024.0 / 1024.0 << std::endl;
    return skompresowany;
}

std::string WizualizacjaWriter::aktualnyCzas() {
    std::array<char, 64> buffer;
    buffer.fill(0);
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), ".%H-%M-%S.%F.", timeinfo);
    std::string timeStr(buffer.data());
    return timeStr;
}
