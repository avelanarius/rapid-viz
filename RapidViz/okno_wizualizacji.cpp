#include "okno_wizualizacji.h"

OknoWizualizacji::OknoWizualizacji(int szerokosc, int wysokosc, sf::ContextSettings settings)
    : window(sf::VideoMode(szerokosc, wysokosc), "Wizualizacja", sf::Style::Default, settings) {

    this->szerokosc = szerokosc;
    this->wysokosc = wysokosc;

    this->trybKomend = false;
    this->czyMinimapa = true;
    this->komenda = "";
    this->rysowaneObiekty = 0;
    this->rysowaneTrojkaty = 0;

    this->ostatniaPozycjaMyszki = sf::Mouse::getPosition(this->window);
    this->czyDrag = false;
    this->ignorujKlikniecie = false;

    this->aktualnyCzasMs = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

    this->ustawDomyslneWidoki();
}

void OknoWizualizacji::obsluzJoystick() {
    if (!this->window.hasFocus()) return;
    for (auto joystick = 0; joystick < sf::Joystick::Count; joystick++) {
        if (!sf::Joystick::isConnected(joystick)) continue;

        auto supportsPan = sf::Joystick::hasAxis(joystick, sf::Joystick::U) &&
            sf::Joystick::hasAxis(joystick, sf::Joystick::V);
        if (supportsPan) {
            auto moveU = sf::Joystick::getAxisPosition(joystick, sf::Joystick::U);
            auto moveV = sf::Joystick::getAxisPosition(joystick, sf::Joystick::V);
            if (fabs(moveU) > 15 || fabs(moveV) > 15) {
                moveU = copysign(pow(fabs(moveU), 1.6), moveU);
                moveV = copysign(pow(fabs(moveV), 1.6), moveV);
                this->widok.move(0.0125 * this->skala * moveU, 0.0125 * this->skala * moveV);
            }
        }

        auto supportsTriggers = sf::Joystick::hasAxis(joystick, sf::Joystick::Z);
        if (supportsTriggers) {
            auto moveZ = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Z);
            if (fabs(moveZ) > 15) {
                moveZ = copysign(pow(fabs(moveZ), 1.6), moveZ);
                this->skala *= 1 + moveZ / 59000.0;
                this->widok.zoom(1 + moveZ / 59000.0);
            }
        }

        auto supportsZoom = sf::Joystick::hasAxis(joystick, sf::Joystick::Y);
        if (supportsZoom) {
            auto moveY = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Y);
            if (fabs(moveY) > 15) {
                moveY = copysign(pow(fabs(moveY), 1.6), moveY);
                this->skala *= 1 + moveY / 59000.0;
                this->widok.zoom(1 + moveY / 59000.0);
            }
        }

        auto movePovX = sf::Joystick::getAxisPosition(joystick, sf::Joystick::PovX);
        auto movePovY = sf::Joystick::getAxisPosition(joystick, sf::Joystick::PovY);
    }
}

void OknoWizualizacji::zarejestrujZnak(char klawisz) {
    if (!this->trybKomend) {
        this->klawisze.push(klawisz);
    } else {
        this->komenda += klawisz;
    }
}

void OknoWizualizacji::obsluzPrzyciskJoystick(unsigned button) {
    if (!this->window.hasFocus()) return;
    if (button == 6) {
        this->widokMini.setSize(this->widok.getSize());
        this->widokMini.setCenter(this->widok.getCenter());
    } else if (button == 0) {
        this->klawisze.push('A');
    } else if (button == 1) {
        this->klawisze.push('B');
    } else if (button == 2) {
        this->klawisze.push('X');
    } else if (button == 3) {
        this->klawisze.push('Y');
    } else if (button == 7) {
        this->czyMinimapa = !this->czyMinimapa;
    }
}

void OknoWizualizacji::ustawDomyslneWidoki() {
    this->widok = sf::View(sf::FloatRect(-this->szerokosc / 32.0f, -this->wysokosc / 32.0f, this->szerokosc / 16.0f, this->wysokosc / 16.0f));
    this->widokMini = sf::View(sf::FloatRect(-this->szerokosc / 32.0f, -this->wysokosc / 32.0f, this->szerokosc / 16.0f, this->wysokosc / 16.0f));
    this->widokMini.setViewport(sf::FloatRect(0.75f, 0.0f, 0.25f, 0.25f));
    this->skala = 1.0f / 16.0f;
}

void OknoWizualizacji::ustawIkone(sf::Image icon) {
    this->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void OknoWizualizacji::obsluzZdarzenia(sf::Event event) {
    if (event.type == sf::Event::TextEntered) {
        this->obsluzWpisanieTekstu(event.text);
    }
    if (event.type == sf::Event::KeyPressed) {
        this->obsluzKomendyKlawiaturowe(event.key);
    }
    if (event.type == sf::Event::JoystickButtonPressed) {
        this->obsluzPrzyciskJoystick(event.joystickButton.button);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        this->obsluzWcisniecieMyszki(event.mouseButton);
    }
    if (event.type == sf::Event::MouseWheelScrolled) {
        this->obsluzKolkoMyszki(event.mouseWheelScroll);
    }
    if (event.type == sf::Event::Resized) {
        this->obsluzZmianeRozmiaruOkna(event.size);
    }
}

void OknoWizualizacji::obsluzKomendyKlawiaturowe(sf::Event::KeyEvent keyEvent) {
    if (!this->trybKomend) {
        if (keyEvent.code == sf::Keyboard::Left) {
            this->widok.move(-30 * this->skala, 0);
        } else if (keyEvent.code == sf::Keyboard::Right) {
            this->widok.move(30 * this->skala, 0);
        } else if (keyEvent.code == sf::Keyboard::Up) {
            this->widok.move(0, -30 * this->skala);
        } else if (keyEvent.code == sf::Keyboard::Down) {
            this->widok.move(0, 30 * this->skala);
        } else if (keyEvent.code == sf::Keyboard::LBracket) {
            this->skala *= 1.0f / 1.5f;
            this->widok.zoom(1.0f / 1.5f);
        } else if (keyEvent.code == sf::Keyboard::RBracket) {
            this->skala *= 1.5f;
            this->widok.zoom(1.5f);
        } else if (keyEvent.code == sf::Keyboard::Return) {
            this->trybKomend = true;
        } else if (keyEvent.code == sf::Keyboard::BackSlash) {
            this->widokMini.setSize(this->widok.getSize());
            this->widokMini.setCenter(this->widok.getCenter());
        } else if (keyEvent.code == sf::Keyboard::Quote) {
            this->czyMinimapa = !this->czyMinimapa;
        }
    } else {
        if (keyEvent.code == sf::Keyboard::Return) {
            this->trybKomend = false;
            this->komendy.push(this->komenda);
            this->wpisaneKomendyAnimacja.push_back({ this->aktualnyCzasMs, this->komenda });
            this->komenda = "";
        }
    }
}

void OknoWizualizacji::obsluzWpisanieTekstu(sf::Event::TextEvent textEvent) {
    if (!this->trybKomend) {
        if (textEvent.unicode < 128 && textEvent.unicode != '\n' && textEvent.unicode != 13) {
            this->zarejestrujZnak(static_cast<char>(textEvent.unicode));
        }
    } else {
        if (textEvent.unicode == '\b') {
            if (!this->komenda.empty()) this->komenda.erase(this->komenda.size() - 1, 1);
        } else if (textEvent.unicode < 128 && textEvent.unicode != '\n' && textEvent.unicode != 13) {
            this->zarejestrujZnak(static_cast<char>(textEvent.unicode));
        }
    }
}

void OknoWizualizacji::obsluzWcisniecieMyszki(sf::Event::MouseButtonEvent mouseButtonEvent) {
    if (mouseButtonEvent.button == sf::Mouse::Right) {
        this->myszka = true;
    }
}

void OknoWizualizacji::obsluzKolkoMyszki(sf::Event::MouseWheelScrollEvent mouseWheelScrollEvent) {
    if (mouseWheelScrollEvent.delta > 0) {
        this->skala *= 1.0f / 1.5f;
        this->widok.zoom(1.0f / 1.5f);
    } else {
        this->skala *= 1.5f;
        this->widok.zoom(1.5f);
    }
}

void OknoWizualizacji::obsluzZmianeRozmiaruOkna(sf::Event::SizeEvent sizeEvent) {
    auto szerokoscRatio = static_cast<float>(sizeEvent.width) / static_cast<float>(this->szerokosc);
    auto wysokoscRatio = static_cast<float>(sizeEvent.height) / static_cast<float>(this->wysokosc);
    this->szerokosc = sizeEvent.width;
    this->wysokosc = sizeEvent.height;
    this->widok.setSize(sf::Vector2f(this->widok.getSize().x * szerokoscRatio, this->widok.getSize().y * wysokoscRatio));
    this->widokMini.setSize(sf::Vector2f(this->widokMini.getSize().x * szerokoscRatio, this->widokMini.getSize().y * wysokoscRatio));
    this->window.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(this->szerokosc), static_cast<float>(this->wysokosc))));
}

void OknoWizualizacji::obsluzLewyPrzyciskMyszy() {
    if (this->window.hasFocus()) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto aktualnaPozycjaMyszki = sf::Mouse::getPosition(this->window);
            sf::IntRect statusBar = { 0, 0, this->szerokosc, 50 };
            sf::IntRect minimapRect = { static_cast<int>(0.75f * this->szerokosc), 0,
                static_cast<int>(0.25f * this->szerokosc), static_cast<int>(0.25f * this->wysokosc) };
            if (!this->czyDrag && (statusBar.contains(aktualnaPozycjaMyszki) ||
                (this->czyMinimapa && minimapRect.contains(aktualnaPozycjaMyszki)))) {
                if (!this->ignorujKlikniecie) {
                    this->ignorujKlikniecie = true;
                    if (this->minimapButton.contains(aktualnaPozycjaMyszki)) {
                        this->czyMinimapa = !this->czyMinimapa;
                    } else if (this->setMinimapButton.contains(aktualnaPozycjaMyszki)) {
                        this->widokMini.setSize(this->widok.getSize());
                        this->widokMini.setCenter(this->widok.getCenter());
                        this->czyMinimapa = true;
                    } else if (this->commandModeButton.contains(aktualnaPozycjaMyszki)) {
                        this->trybKomend = !this->trybKomend;
                    } else if (this->newWindowButton.contains(aktualnaPozycjaMyszki)) {
                        this->zadaneNoweOkna++;
                    } else if (this->czyMinimapa && minimapRect.contains(aktualnaPozycjaMyszki)) {
                        auto aktualnyWidok = this->window.getView();
                        this->window.setView(this->widokMini);
                        auto pozycjaMyszkiPixel = sf::Mouse::getPosition(this->window);
                        auto pozycjaMyszki = this->window.mapPixelToCoords(pozycjaMyszkiPixel);
                        this->widok.setCenter(pozycjaMyszki);
                        this->window.setView(aktualnyWidok);
                        this->ignorujKlikniecie = false;
                    }
                }
            } else {
                if (!this->ignorujKlikniecie) {
                    if (this->czyDrag) {
                        auto przesX = aktualnaPozycjaMyszki.x - this->ostatniaPozycjaMyszki.x;
                        auto przesY = aktualnaPozycjaMyszki.y - this->ostatniaPozycjaMyszki.y;
                        this->widok.move(-przesX * this->skala, -przesY * this->skala);
                    }
                    this->czyDrag = true;
                }
            }
            this->ostatniaPozycjaMyszki = aktualnaPozycjaMyszki;
        } else {
            this->czyDrag = false;
            this->ignorujKlikniecie = false;
        }
    } else {
        this->czyDrag = false;
        this->ignorujKlikniecie = false;
    }
}

void OknoWizualizacji::rysujTlo() {
    this->window.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(this->szerokosc), static_cast<float>(this->wysokosc))));
    sf::RectangleShape background(sf::Vector2f(static_cast<float>(this->szerokosc), static_cast<float>(this->wysokosc)));
    background.setPosition(0, 0);
    background.setTexture(this->backgroundTexture);
    background.setTextureRect({ 0, 0, this->szerokosc, this->wysokosc });
    this->window.draw(background);
}

void OknoWizualizacji::rysujGlownyWidok() {
    this->window.setView(this->widok);
    this->window.pushGLStates();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(this->window.getView().getTransform().getMatrix());

    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glVertexPointer(2, GL_FLOAT, 8 * sizeof(float), nullptr);

    glColorPointer(4, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(2 * sizeof(float)));

    glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));

    sf::Texture::bind(this->shapesTexture);

    glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(this->liczbaTrojkatow));
    this->rysowaneTrojkaty += this->liczbaTrojkatow;

    this->window.popGLStates();
}

void OknoWizualizacji::rysujHUD() {
    this->window.setView(sf::View(sf::FloatRect(0, 0, this->szerokosc, this->wysokosc)));

    sf::RectangleShape statusbar(sf::Vector2f(this->szerokosc, 50));
    statusbar.setPosition(0, 0);
    statusbar.setTexture(this->statusBarTexture);
    statusbar.setTextureRect({ 0, 0, this->szerokosc, 50 });
    this->window.draw(statusbar);

    sf::RectangleShape icon(sf::Vector2f(16, 16));
    icon.setPosition(180, 5);
    icon.setTexture(this->statusTexture);
    this->window.draw(icon);

    icon.setPosition(180, 27);
    icon.setTexture(this->selectedTexture);
    this->window.draw(icon);

    this->rysujPrzycisk(minimapButton, minimapButtonTexture);
    this->rysujPrzycisk(setMinimapButton, setMinimapButtonTexture);
    this->rysujPrzycisk(commandModeButton, commandModeButtonTexture);
    this->rysujPrzycisk(newWindowButton, newWindowButtonTexture);

    sf::Text text;
    text.setFont(this->fontBold);
    text.setString(this->status);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color(50, 50, 50));
    text.setPosition(204, 3);
    this->window.draw(text);

    text.setFont(this->font);
    if (!this->trybKomend) text.setString(this->statusObiektu);
    else {
        if (this->migajacaKomenda) text.setString("Podaj komende: " + this->komenda);
        else text.setString("Podaj komende: " + this->komenda + "|");
    }
    text.setPosition(204, 25);
    this->window.draw(text);

    this->rysujAnimacjeHUD();
}

void OknoWizualizacji::rysujMinimape() {
    if (this->czyMinimapa) {
        sf::RectangleShape minimap(sf::Vector2f(0.25f * this->szerokosc, 0.25f * this->wysokosc));
        minimap.setPosition(0.75 * this->szerokosc, 0);
        minimap.setTexture(this->backgroundTexture);
        minimap.setTextureRect({ 0, 0, static_cast<int>(0.25f * this->szerokosc), static_cast<int>(0.25f * this->wysokosc) });
        minimap.setOutlineThickness(5);
        minimap.setOutlineColor(sf::Color(255, 255, 255, 243));
        this->window.draw(minimap);

        this->window.setView(this->widokMini);
        sf::FloatRect widokMiniRect
        (sf::Vector2f(this->widokMini.getCenter().x - (this->widokMini.getSize().x) / 2, this->widokMini.getCenter().y - (this->widokMini.getSize().y) / 2),
         this->widokMini.getSize());
        this->window.pushGLStates();

        GLint x = this->window.getView().getViewport().left * this->szerokosc;
        GLint y = this->wysokosc - this->window.getView().getViewport().height * this->wysokosc;
        GLsizei width = this->window.getView().getViewport().width * this->szerokosc;
        GLsizei height = this->window.getView().getViewport().height * this->wysokosc;
        glViewport(x, y, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(this->window.getView().getTransform().getMatrix());

        glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
        glVertexPointer(2, GL_FLOAT, 8 * sizeof(float), nullptr);

        glColorPointer(4, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(2 * sizeof(float)));

        glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));

        sf::Texture::bind(this->shapesTexture);

        glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glDrawArrays(GL_TRIANGLES, 0, this->liczbaTrojkatow);

        this->window.popGLStates();

        this->rysowaneTrojkaty += this->liczbaTrojkatow;

        sf::FloatRect widokRect
        (sf::Vector2f(this->widok.getCenter().x - (this->widok.getSize().x) / 2, this->widok.getCenter().y - (this->widok.getSize().y) / 2),
         this->widok.getSize());
        sf::RectangleShape obramowanieWidokuOgolnego;
        obramowanieWidokuOgolnego.setPosition(widokRect.left, widokRect.top);
        obramowanieWidokuOgolnego.setSize(sf::Vector2f(widokRect.width, widokRect.height));
        auto gruboscLinii = std::max(widokMiniRect.height, widokMiniRect.width);
        gruboscLinii *= 0.02f;
        obramowanieWidokuOgolnego.setOutlineThickness(gruboscLinii);
        obramowanieWidokuOgolnego.setOutlineColor(sf::Color(255, 255, 255, 180));
        obramowanieWidokuOgolnego.setFillColor(sf::Color(255, 255, 255, 0));
        this->window.draw(obramowanieWidokuOgolnego);
    }
}

void OknoWizualizacji::rysujPrzycisk(sf::IntRect position, sf::Texture* texture) {
    auto aktualnaPozycjaMyszki = sf::Mouse::getPosition(this->window);

    sf::RectangleShape button;

    if (position.contains(aktualnaPozycjaMyszki) && !this->ignorujKlikniecie) {
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

void OknoWizualizacji::rysujAnimacjeHUD() {
    this->wpisaneKomendyAnimacja.erase(std::remove_if(this->wpisaneKomendyAnimacja.begin(),
                                                      this->wpisaneKomendyAnimacja.end(),
                                                      [&](std::pair<unsigned long long, std::string> elem) { return this->aktualnyCzasMs - elem.first >= 400; }),
                                       this->wpisaneKomendyAnimacja.end());

    for (auto animacja : this->wpisaneKomendyAnimacja) {
        auto timeDelta = static_cast<float>(this->aktualnyCzasMs - animacja.first) / 400.0f;
        sf::Text animatedText;
        animatedText.setFont(this->font);
        animatedText.setString(animacja.second);
        animatedText.setCharacterSize(15);
        animatedText.setFillColor(sf::Color(150, 150, 150, 255.0f - timeDelta * timeDelta * 255.0f));
        animatedText.setPosition(320, 25 + timeDelta * 100.0f);
        animatedText.setScale(1 + 1.5f * timeDelta, 1 + 1.5f * timeDelta);
        this->window.draw(animatedText);
    }
}
