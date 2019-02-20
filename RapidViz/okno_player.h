#pragma once
#define _USE_MATH_DEFINES
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <thread>
#include <array>

class OknoPlayer {
public:
    OknoPlayer();
    void setIcon();

    unsigned long long aktualnyCzas = 0;
    unsigned long long dlugosc = 0;
    unsigned long long pierwszyCzas = 0;
    bool czyOdtwarza = false;

    const int liczbaPredkosci = 9;
    const std::array<float, 9> predkosci = { 1.0, 2.0, 4.0, 8.0, 16.0, 1.0 / 2.0, 1.0 / 4.0, 1.0 / 8.0, 1.0 / 16.0 };
    const std::array<std::string, 9> predkosciString = { "1x", "2x", "4x", "8x", "16x", "1/2", "1/4", "1/8", "1/16" };

    int aktualnaPredkosc = 0;

    sf::RenderWindow window;
    std::thread watek;

    int szerokosc;
    int wysokosc;

    sf::Texture backgroundTexture;
    sf::Texture playTexture;
    sf::Texture pauseTexture;
    sf::Texture progressTexture;
    sf::Texture clockTexture;
    sf::Texture emptyButtonTexture;

    sf::Font fontBold;

    const sf::IntRect playButton = { 10, 50, 36, 36 };
    const sf::IntRect speedButton = { 10 + 36 + 10, 50, 36, 36 };

    static bool wczytajImage(sf::Image& image, std::string filename);
    static bool wczytajFont(sf::Font& font, std::string filename);
    static bool wczytajTexture(sf::Texture& texture, std::string filename);
private:
    void loadAssets();
    void createWindow();
    void drawBackground();
    void drawPostep();
    void drawButtons();
    void drawUI();
    void obsluzWcisniecieMyszki(const sf::Event::MouseButtonEvent& event);
    void obsluzZdarzenie(const sf::Event& event);
    void obslugaOkna();
    void rysujPrzycisk(sf::IntRect position, sf::Texture* texture);

    std::string msToString(unsigned long long time);
};
