#include "okno_player.h"
#include <iostream>
#include <iomanip>
#include <sstream>

OknoPlayer::OknoPlayer() {
    this->szerokosc = 600;
    this->wysokosc = 96;

    this->aktualnyCzas = 0;
    this->dlugosc = 0;

    this->watek = std::thread(&OknoPlayer::obslugaOkna, this);
}

void OknoPlayer::setIcon() {
    sf::Image icon;
    this->wczytajImage(icon, "player_icon.png");
    this->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void OknoPlayer::createWindow() {
    this->window.create(sf::VideoMode(this->szerokosc, this->wysokosc),
                        "Odtwarzacz", sf::Style::Titlebar | sf::Style::Close);
    this->window.setFramerateLimit(60);

    this->loadAssets();
}

void OknoPlayer::drawBackground() {
    sf::RectangleShape background(sf::Vector2f(static_cast<float>(this->szerokosc), static_cast<float>(this->wysokosc)));
    background.setPosition(0, 0);
    background.setTexture(&this->backgroundTexture);
    background.setTextureRect({ 0, 0, this->szerokosc, this->wysokosc });
    this->window.draw(background);
}

std::string czasToString(unsigned long long c) {
    std::chrono::milliseconds dur(c);
    std::chrono::time_point<std::chrono::system_clock> dt(dur);

    std::array<char, 64> buffer;
    buffer.fill(0);
    std::time_t rawtime = std::chrono::system_clock::to_time_t(dt);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), "%H:%M:%S", timeinfo);
    std::string timeStr(buffer.data());
    return timeStr;
}

void OknoPlayer::drawPostep() {
    sf::RectangleShape postep(sf::Vector2f(this->szerokosc - 20, 30));
    postep.setPosition(10, 10);
    postep.setFillColor(sf::Color(100, 100, 100, 100));
    postep.setOutlineThickness(1);
    postep.setOutlineColor(sf::Color(0, 0, 0, 100));
    this->window.draw(postep);

    auto procent = (this->dlugosc == 0 ?
                    0 : static_cast<float>(this->aktualnyCzas) / static_cast<float>(this->dlugosc));

    postep.setSize(sf::Vector2f(procent * (this->szerokosc - 20), 30));
    postep.setOutlineThickness(0);
    postep.setFillColor(sf::Color::White);
    postep.setTexture(&this->progressTexture);
    this->window.draw(postep);

    std::string postepString = msToString(this->aktualnyCzas) + " / " + msToString(this->dlugosc) + " (" + czasToString(this->aktualnyCzas + this->pierwszyCzas) + ")";

    sf::Text text;
    text.setFont(this->fontBold);
    text.setCharacterSize(20);
    text.setString(postepString);

    auto width = text.getLocalBounds().width;

    text.setPosition(sf::Vector2f(this->szerokosc - 10 - width, playButton.top + 7));
    text.setFillColor(sf::Color::Black);
    this->window.draw(text);

    postep.setTexture(&this->clockTexture);
    postep.setTextureRect({ 0, 0, 16, 16 });
    postep.setSize(sf::Vector2f(16, 16));
    postep.setPosition(sf::Vector2f(text.getPosition().x - 20, text.getPosition().y + 5));
    this->window.draw(postep);
}

void OknoPlayer::drawButtons() {
    if (this->czyOdtwarza) {
        this->rysujPrzycisk(this->playButton, &this->pauseTexture);
    } else {
        this->rysujPrzycisk(this->playButton, &this->playTexture);
    }

    this->rysujPrzycisk(this->speedButton, &this->emptyButtonTexture);

    sf::Text text;
    text.setFont(this->fontBold);
    text.setCharacterSize(15);
    text.setString(this->predkosciString[this->aktualnaPredkosc]);

    const auto width = text.getLocalBounds().width;
    text.setPosition(0.5 * (this->speedButton.width - width) + this->speedButton.left - 1, this->speedButton.top + 9);
    text.setFillColor(sf::Color::White);
    this->window.draw(text);

}

void OknoPlayer::drawUI() {
    this->drawBackground();
    this->drawPostep();
    this->drawButtons();
}

void OknoPlayer::obsluzWcisniecieMyszki(const sf::Event::MouseButtonEvent& event) {
    if (event.button == sf::Mouse::Button::Left) {
        auto aktualnaPozycjaMyszki = sf::Mouse::getPosition(this->window);

        sf::IntRect postep(10, 10, this->szerokosc - 20, 30);

        if (this->playButton.contains(aktualnaPozycjaMyszki)) {
            this->czyOdtwarza = !this->czyOdtwarza;
        } else if (this->speedButton.contains(aktualnaPozycjaMyszki)) {
            this->aktualnaPredkosc++;
            this->aktualnaPredkosc %= this->liczbaPredkosci;
        } else if (postep.contains(aktualnaPozycjaMyszki)) {
            const auto procent = static_cast<float>(aktualnaPozycjaMyszki.x - postep.left) / static_cast<float>(postep.width);
            this->aktualnyCzas = procent * this->dlugosc;
        }
    }
}

void OknoPlayer::obsluzZdarzenie(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        this->obsluzWcisniecieMyszki(event.mouseButton);
    }
}

void OknoPlayer::loadAssets() {
    this->setIcon();

    this->wczytajTexture(backgroundTexture, "white_background.png");
    this->backgroundTexture.setRepeated(true);

    this->wczytajTexture(playTexture, "play_button.png");
    this->wczytajTexture(pauseTexture, "pause_button.png");
    this->wczytajTexture(emptyButtonTexture, "empty_button.png");

    this->wczytajTexture(progressTexture, "progress_bar.png");
    this->progressTexture.setRepeated(true);

    this->wczytajTexture(clockTexture, "clock.png");

    this->wczytajFont(fontBold, "ui_font_bold.ttf");
}

void OknoPlayer::obslugaOkna() {
    this->createWindow();

    auto ostatniCzas = std::chrono::steady_clock::now();

    while (this->window.isOpen()) {
        auto aktualnyCzas = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration <double, std::milli>(aktualnyCzas - ostatniCzas).count();
        delta *= this->predkosci[this->aktualnaPredkosc];

        if (this->czyOdtwarza) {
            this->aktualnyCzas += delta;
            if (this->aktualnyCzas > this->dlugosc) {
                this->aktualnyCzas = this->dlugosc;
                this->czyOdtwarza = false;
            }
        }

        ostatniCzas = aktualnyCzas;

        this->window.clear(sf::Color::Black);

        sf::Event event;
        while (this->window.pollEvent(event)) {
            this->obsluzZdarzenie(event);
        }

        this->drawUI();

        this->window.display();
    }
}

bool OknoPlayer::wczytajImage(sf::Image& image, std::string filename) {
    if (!image.loadFromFile(filename)) {
        std::cerr << "BLAD WCZYTYWANIA OBRAZKA: " << filename << std::endl;
        return false;
    }
    return true;
}

bool OknoPlayer::wczytajFont(sf::Font& font, std::string filename) {
    if (!font.loadFromFile(filename)) {
        std::cerr << "BLAD WCZYTYWANIA FONTU: " << filename << std::endl;
        return false;
    }
    return true;
}

bool OknoPlayer::wczytajTexture(sf::Texture& texture, std::string filename) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "BLAD WCZYTYWANIA TEXTURE: " << filename << std::endl;
        return false;
    }
    return true;
}

void OknoPlayer::rysujPrzycisk(sf::IntRect position, sf::Texture* texture) {
    auto aktualnaPozycjaMyszki = sf::Mouse::getPosition(this->window);

    sf::RectangleShape button;

    if (position.contains(aktualnaPozycjaMyszki)) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            button.setFillColor(sf::Color(255, 255, 255, 150));

            auto heightChange = position.height / 20;
            auto widthChange = position.width / 20;

            position.height -= heightChange * 2;
            position.width -= widthChange * 2;

            position.top += heightChange;
            position.left += widthChange;
        } else {
            button.setFillColor(sf::Color(255, 255, 255, 220));
        }
    } else {
        button.setFillColor(sf::Color(255, 255, 255, 255));
    }

    button.setTexture(texture);
    button.setSize(sf::Vector2f(position.width, position.height));
    button.setPosition(position.left, position.top);

    this->window.draw(button);
}

std::string to_zero_lead(const int value, const unsigned precision) {
    std::ostringstream oss;
    oss << std::setw(precision) << std::setfill('0') << value;
    return oss.str();
}

std::string OknoPlayer::msToString(const unsigned long long time) {
    const auto minutes = time / (60 * 1000);

    const auto seconds = (time / 1000) % 60;

    const auto miliseconds = time % 1000;

    return to_zero_lead(minutes, 2) + ":" + to_zero_lead(seconds, 2) + ":" + to_zero_lead(miliseconds / 10, 2);
}
