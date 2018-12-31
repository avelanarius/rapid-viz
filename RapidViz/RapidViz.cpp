#include "wizualizacja.h"
#include "element_wiz.h"
#include "wizualizacja_writer.h"

int main() {
    /* Stwórz obiekt wizualizacji z początkowym oknem
     * o wymiarach 600 x 600. */
    Wizualizacja wiz(600, 600);

    while (true) {
        /* Jeżeli chcemy zmienić aktualny stan wizualizacji,
         * należy przekazać vector elementów. */
        std::vector<ElementWiz> wyswietlaneElementy;

        /* Stwórz kształt typu KWADRAT o środku w punkcie
         * (5, 5), wymiarach 10 x 10, kolorze White i opisie
         * "Tlo" (opis widoczny po najechaniu na element). */
        wyswietlaneElementy.push_back(
            ElementWiz::ksztalt(Ksztalt::KWADRAT,
                                { 5, 5 }, 10, sf::Color::White, "Tlo"));

        wyswietlaneElementy.push_back(
            ElementWiz::ksztalt(Ksztalt::DOM,
                                { 2, 2 }, 1, sf::Color::Red, "Pozycja startowa"));
        wyswietlaneElementy.push_back(
            ElementWiz::ksztalt(Ksztalt::GWIAZDA_8,
                                { 7, 7 }, 1, sf::Color::Red, "Pozycja docelowa"));

        wyswietlaneElementy.push_back(
            ElementWiz::linia({ 3, 3 }, { 6, 6 }, 0.15, StylLinii::STYL_KOLO_TROJKAT,
                              sf::Color::Blue, "Trasa"));

        /* Zaktualizuj wyświetlane elementy. */
        wiz.setNoweElementyWiz(wyswietlaneElementy);

        /* Ustaw status, widoczny w górnej belce. */
        wiz.setStatus("Przydatne informacje.");

        /* W tym przykładzie aktualizujemy stan wizualizacji
         * co sekundę, jednak wystarczy robić to tylko gdy
         * rzeczywiście chcemy zmienić wyświetlany stan. */
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}