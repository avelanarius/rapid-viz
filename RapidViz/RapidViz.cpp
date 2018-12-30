#include "wizualizacja.h"
#include <fstream>
#include <queue>
#include "element_wiz.h"
#include <ctime>
#include "wizualizacja_writer.h"

using namespace std;

Wizualizacja wiz(1300, 800);
WizualizacjaWriter writer("test");

int frameCount = 0;

void rysuj() {
    vector<ElementWiz> noweElemWiz;

    for (int i = 0; i < 70; i++) {
        for (int j = 0; j < 70; j++) {
            auto kolor = (i * frameCount) % 256;
            auto kolor2 = (i ^ j + frameCount * kolor) % 256;
            noweElemWiz.push_back(ElementWiz::ksztalt(Ksztalt::KWADRAT, { i, j }, 1.0, sf::Color((kolor | kolor2) % 256, kolor2, kolor), "POZYCJA TO: " + to_string(i)));
            noweElemWiz.push_back(ElementWiz::ksztalt(Ksztalt::KOLO, { i, j }, 0.5, sf::Color::White, "tralalala"));
            //noweElemWiz.push_back(ElementWiz::liczba(i * 500 + j, { i, j }, 0.5, sf::Color::White, ""));
        }
    }

    wiz.setNoweElementyWiz(noweElemWiz);
    writer.dodajStan(noweElemWiz);
    frameCount++;
}

int main() {
    while (true) {
        rysuj();
        std::this_thread::sleep_for(300ms);
    }
    //outfile.close();
    //cout << "TUTAJ KONIEC!" << endl;
    return 0;
}
