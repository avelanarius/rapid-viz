# RapidViz
**RapidViz** to wieloplatformowa biblioteka graficzna napisana w C++, zorientowana na prostotę użycia, zaprojektowana pod kątem prototypowania i szybkiej iteracji. 

RapidViz pozwala na wyświetlanie **figur** z szerokiej biblioteki kształtów, jak również różnego rodzaju linii oraz liczb. Proces wizualizacji odbywa się na **osobnym wątku** i nie wymaga obsługi ze strony użytkownika biblioteki. 

RapidViz umożliwia w prosty sposób na **interakcję z interfejsem**: kliknięcie elementów widocznych na ekranie, klawisze klawiaturowe oraz wpisywane komendy. Cały przebieg wizualizacji może być **zapisywany do pliku** i interaktywnie odtwarzany w późniejszym czasie.

---

<p align="center">
<b>Przykładowy program, wykorzystujący RapidViz:</b><br/><br/>
<img src="https://github.com/avelanarius/rapid-viz/raw/master/Documentation/main_screenshot.gif" width="70%" />
</p>

## Spis treści
- [:page_with_curl: Funkcjonalność](#Funkcjonalność)
- [:question: Przykład użycia](#Przykład-użycia)

## Funkcjonalność
- **Figury** (na obrazku część):

  <img src="https://github.com/avelanarius/rapid-viz/raw/master/Documentation/ksztalty.png" width="40%" />
  
  - Kształty z dostępnej biblioteki kształtów
  - Linie, z możliwością dostosowania grotów strzałek
  - Liczby
- **Pasek ze statusem**, wyświetlający modyfikowalny status i pole opisu elementu pod kursorem myszki. 

  <img src="https://github.com/avelanarius/rapid-viz/raw/master/Documentation/status_bar.png" width="50%" />
  
- **Minimapa**, pozwalająca na szybkie przemieszczanie się po obszarze roboczym.

  <img src="https://github.com/avelanarius/rapid-viz/raw/master/Documentation/minimap.gif" width="30%" />

- **Wiele okien**, ułatwiające obserwowanie obszaru roboczego w wielu miejscach na raz.

  <img src="https://github.com/avelanarius/rapid-viz/raw/master/Documentation/wiele_okien.gif" width="70%" />
  
- **Interakcja z interfejsem**, umożliwiająca przechwytywanie zdarzeń kliknięcia widocznych elementów, wciśnięcia klawisza lub wpisania komendy.
- **Zapis przebiegu wizualizacji**, który może być później interaktywnie odtwarzany.
- **Akceleracja sprzętowa**: biblioteka korzysta z OpenGL, dzięki czemu wyświetlanie jest wspomagane sprzętowo przez kartę graficzną.
- **Wielowątkowość**: proces wizualizacji działa na osobnym wątku, a wszystkie metody wizualizacji są bezpieczne wielowątkowo. Pozwala to na aktualizowanie UI, nawet gdy główny wątek jest zajęty.

## Przykład użycia

<img src="https://github.com/avelanarius/rapid-viz/raw/master/Documentation/przyklad.PNG" width="50%" />

Przykład dostępny w pliku [RapidViz.cpp](https://github.com/avelanarius/rapid-viz/blob/master/RapidViz/RapidViz.cpp).

```c++
#include "wizualizacja.h"
#include "element_wiz.h"

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
```
