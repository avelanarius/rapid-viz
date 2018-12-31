#include "wizualizacja.h"
#include <fstream>
#include <queue>
#include "element_wiz.h"
#include <ctime>
#include "wizualizacja_writer.h"

using namespace std;

Wizualizacja wiz(1300, 800);
//WizualizacjaWriter writer("test");

int frameCount = 0;

std::string plansza[] = {
 "###########O###",
 "#####..########",
 "##.......###.##",
 "###.....#O##.##",
 "####.#.########",
 "#########D#####",
 "#O##############.......",
 "###O##OO#######..#......",
 "#O###O######O##..........",
 "##O#####O#########....#O##",
 "###################.######"
};

int pozX = 10;
int pozY = 10;

void rysuj() {
    vector<ElementWiz> noweElemWiz;

    noweElemWiz.push_back(ElementWiz::ksztalt(Ksztalt::KWADRAT, { -50, -50 }, 200, sf::Color(0xff0062ff), ""));

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < plansza[i].size(); j++) {
            if (plansza[i][j] == '#') {
                noweElemWiz.push_back(ElementWiz::ksztalt(Ksztalt::KWADRAT, { j, i }, 1, sf::Color(0xff0062ff), ""));
            } else if (plansza[i][j] == '.') {
                noweElemWiz.push_back(ElementWiz::ksztalt(Ksztalt::KWADRAT, { j, i }, 1, sf::Color(0xff0062ff), ""));
                noweElemWiz.push_back(ElementWiz::ksztalt(Ksztalt::KWADRAT, { j, i }, 1, sf::Color(255, 255, 255, (i * 92 + j * 382) % 79 + 130), "Woda."));
            } else if (plansza[i][j] == 'D') {
                noweElemWiz.push_back(ElementWiz::ksztalt(Ksztalt::KWADRAT, { j, i }, 1, sf::Color(0xff0062ff), ""));
                noweElemWiz.push_back(ElementWiz::ksztalt(Ksztalt::DOM, { j, i }, 1, sf::Color(255, 255, 255, 210), "Punkt docelowy. 0 / 100 zajete. Budzet: 523 zl."));
            } 
        }
    }

    for (auto klawisz : wiz.getKlawisze()) {
        if (klawisz == 'w') pozY--;
        else if (klawisz == 's') pozY++;
        else if (klawisz == 'a') pozX--;
        else if (klawisz == 'd') pozX++;
    }

    wiz.setNoweElementyWiz(noweElemWiz);
    wiz.setStatus("Trwa rozgrywka. Pozostale jednostki: 14. Pozostaly czas: 1h. ");
    //writer.dodajStan(noweElemWiz);
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
