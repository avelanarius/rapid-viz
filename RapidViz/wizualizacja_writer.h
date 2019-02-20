#pragma once
#include "element_wiz.h"
#include "lz4.h"

#include <vector>
#include <array>
#include <chrono>
#include <array>
#include <future>
#include <fstream>

class WizualizacjaWriter {
public:
    WizualizacjaWriter(std::string nazwa);

    void dodajStan(std::vector<ElementWiz> stan);
private:
    void wstawStan(std::vector<ElementWiz> stan);
    bool czyZapisac() const;
    void asyncZapisz(const int aktualnyNumer, const std::vector<std::vector<ElementWiz>>& _daneDoZapisu, const std::vector<unsigned long long>& daneDoZapisuCzasy);
    void zapiszCzasy(const std::string& nazwaPliku, const std::vector<unsigned long long>& czasyDoZapisu) const;
    std::vector<char> serializujDane(const std::vector<std::vector<ElementWiz>>& daneDoZapisu) const;
    void writeToVector(std::vector<char>& vec, const char* dane, unsigned long long length) const;
    std::vector<char> kompresujDane(const std::vector<char>& dane) const;
    void zapiszDane(const std::string& nazwaPliku, const std::vector<std::vector<ElementWiz>>& daneDoZapisu) const;
    void zapisz();
    void usunTmp(std::string nazwaPliku);

    std::string aktualnyCzas();

    std::vector<std::vector<ElementWiz>> zapisaneStany;
    int liczbaElementWiz = 0;
    std::chrono::time_point<std::chrono::system_clock> ostatniZapis = std::chrono::system_clock::now();

    std::vector<unsigned long long> czasy;
    std::string nazwa;

    int numerBloku;
};
