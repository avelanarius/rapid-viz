#include "wizualizacja.h"

Wizualizacja::Wizualizacja(int szerokosc, int wysokosc) {
	this->szerokoscPoczatkowa = szerokosc;
	this->wysokoscPoczatkowa = wysokosc;
	this->watek = std::thread(&Wizualizacja::obslugaOkna, this);
}

void Wizualizacja::setNoweElementyWiz(std::vector<ElementWiz>& noweElementyWiz) {
	auto noweElementyWizCopy = noweElementyWiz;
	this->block.lock();
	this->elementyWiz.swap(noweElementyWizCopy);
	this->czyZmienioneObiekty = true;
	this->block.unlock();
}

void Wizualizacja::setStatus(std::string status) {
	this->block.lock();
	this->status = status;
	this->block.unlock();
}

std::vector<ElementWiz> Wizualizacja::getKlikniete() {
	this->block.lock();
	std::vector<ElementWiz> kliknieteVector;
	while (!this->klikniete.empty()) {
		kliknieteVector.push_back(this->klikniete.front());
		this->klikniete.pop();
	}
	this->block.unlock();
	return kliknieteVector;
}

std::vector<std::string> Wizualizacja::getKomendy() {
	this->block.lock();
	std::vector<std::string> komendyVector;
	while (!this->komendy.empty()) {
		komendyVector.push_back(this->komendy.front());
		this->komendy.pop();
	}
	this->block.unlock();
	return komendyVector;
}

std::vector<char> Wizualizacja::getKlawisze() {
	this->block.lock();
	std::vector<char> klawiszeVector;
	while (!this->klawisze.empty()) {
		klawiszeVector.push_back(this->klawisze.front());
		this->klawisze.pop();
	}
	this->block.unlock();
	return klawiszeVector;
}

bool Wizualizacja::wczytajImage(sf::Image& image, std::string filename) {
	if (!image.loadFromFile(filename)) {
		std::cerr << "BLAD WCZYTYWANIA OBRAZKA: " << filename << std::endl;
		return false;
	}
	return true;
}

bool Wizualizacja::wczytajFont(sf::Font& font, std::string filename) {
	if (!font.loadFromFile(filename)) {
		std::cerr << "BLAD WCZYTYWANIA FONTU: " << filename << std::endl;
		return false;
	}
	return true;
}

bool Wizualizacja::wczytajTexture(sf::Texture& texture, std::string filename) {
	if (!texture.loadFromFile(filename)) {
		std::cerr << "BLAD WCZYTYWANIA TEXTURE: " << filename << std::endl;
		return false;
	}
	return true;
}

bool Wizualizacja::wczytajAssety() {
	if (!this->wczytajImage(icon, "icon.png")) return false;

	if (!this->wczytajFont(font, "ui_font.ttf")) return false;
	if (!this->wczytajFont(fontBold, "ui_font_bold.ttf")) return false;

	statusBarTexture.setRepeated(true);
	if (!this->wczytajTexture(statusBarTexture, "status_bar.png")) return false;

	if (!this->wczytajTexture(selectedTexture, "selected.png")) return false;
	if (!this->wczytajTexture(statusTexture, "status.png")) return false;

	backgroundTexture.setRepeated(true);
	if (!this->wczytajTexture(backgroundTexture, "background.png")) return false;

	if (!this->wczytajTexture(minimapButtonTexture, "minimap_button.png")) return false;
	if (!this->wczytajTexture(setMinimapButtonTexture, "set_minimap_button.png")) return false;
	if (!this->wczytajTexture(commandModeButtonTexture, "command_mode_button.png")) return false;
	if (!this->wczytajTexture(newWindowButtonTexture, "new_window_button.png")) return false;

	shapesTexture.setSmooth(true);
	if (!this->wczytajTexture(shapesTexture, "shapes.png")) return false;
	shapesTexture.generateMipmap();
	return true;
}

void Wizualizacja::przekazAssetyDoOkna(OknoWizualizacji* oknoWizualizacji) {
	oknoWizualizacji->ustawIkone(icon);
	oknoWizualizacji->font = font;
	oknoWizualizacji->fontBold = fontBold;
	oknoWizualizacji->statusBarTexture = &statusBarTexture;
	oknoWizualizacji->selectedTexture = &selectedTexture;
	oknoWizualizacji->statusTexture = &statusTexture;
	oknoWizualizacji->backgroundTexture = &backgroundTexture;
	oknoWizualizacji->minimapButtonTexture = &minimapButtonTexture;
	oknoWizualizacji->setMinimapButtonTexture = &setMinimapButtonTexture;
	oknoWizualizacji->commandModeButtonTexture = &commandModeButtonTexture;
	oknoWizualizacji->shapesTexture = &shapesTexture;
	oknoWizualizacji->newWindowButtonTexture = &newWindowButtonTexture;
}

#include <typeinfo>

void Wizualizacja::przekazDaneDoOknaKlatka(OknoWizualizacji* oknoWizualizacji) {
	oknoWizualizacji->status = this->status;
	oknoWizualizacji->migajacaKomenda = migajacaKomenda;
	oknoWizualizacji->vertexBuffer = vertexBuffer;
	oknoWizualizacji->liczbaTrojkatow = daneTrojkatow.size() / 8;
	oknoWizualizacji->rysowaneObiekty = elementyWiz.size();
	oknoWizualizacji->aktualnyCzasMs = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();

	while (!oknoWizualizacji->komendy.empty()) {
		this->komendy.push(oknoWizualizacji->komendy.front());
		oknoWizualizacji->komendy.pop();
	}

	while (!oknoWizualizacji->klawisze.empty()) {
		this->klawisze.push(oknoWizualizacji->klawisze.front());
		oknoWizualizacji->klawisze.pop();
	}
}

void Wizualizacja::inicjujGLEW() {
	glewExperimental = GL_TRUE;
	glewInit();
}

void Wizualizacja::aktualizujVertexBuffer() {
	if (this->czyZmienioneObiekty) {
		this->aktywneOkna.front()->window.setActive(true);
		this->czyZmienioneObiekty = false;
		daneTrojkatow.clear();
		for (auto& elementWiz : elementyWiz) {
			elementWiz.generujDaneTrojkatow(daneTrojkatow);
		}
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float)) * static_cast<GLsizeiptr>(daneTrojkatow.size()), nullptr, GL_STREAM_DRAW);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float)) * static_cast<GLsizeiptr>(daneTrojkatow.size()), daneTrojkatow.data(), GL_STREAM_DRAW);
		glPopClientAttrib();
	}
}

void Wizualizacja::stworzNoweOkno(OknoWizualizacji* aktywneOkno) {
	std::unique_ptr<OknoWizualizacji> noweOkno(new OknoWizualizacji(aktywneOkno->szerokosc, aktywneOkno->wysokosc, settings));
	noweOkno->widok = aktywneOkno->widok;
	noweOkno->widokMini = aktywneOkno->widokMini;
	noweOkno->czyMinimapa = aktywneOkno->czyMinimapa;
	noweOkno->skala = aktywneOkno->skala;

	auto pozycjaOkna = aktywneOkno->window.getPosition();
	pozycjaOkna.x += 50;
	pozycjaOkna.y += 50;
	noweOkno->window.setPosition(pozycjaOkna);

	this->przekazAssetyDoOkna(noweOkno.get());

	this->aktywneOkna.push_back(std::move(noweOkno));
}

void Wizualizacja::obsluzZdarzeniaOkien() {
	for (int i = this->aktywneOkna.size() - 1; i >= 0; i--) {
		auto okno = this->aktywneOkna[i].get();
		okno->rysowaneTrojkaty = 0;
		okno->myszka = false;
		okno->obsluzLewyPrzyciskMyszy();

		sf::Event event;
		bool zamkniete = false;
		while (okno->window.pollEvent(event)) {
			okno->obsluzZdarzenia(event);
			if (event.type == sf::Event::Closed) {
				okno->window.close();
				zamkniete = true;
				break;
			}
		}

		for (int j = 0; j < okno->zadaneNoweOkna; j++) {
			this->stworzNoweOkno(okno);
		}
		okno->zadaneNoweOkna = 0;

		if (zamkniete) {
			this->aktywneOkna.erase(this->aktywneOkna.begin() + i);
		}
	}
}

void Wizualizacja::obsluzPozycjeMyszkiOkien() {
	for (auto& okno : this->aktywneOkna) {
		okno->window.setView(okno->widok);
		sf::IntRect prostokatOkna = {0, 0, okno->szerokosc, okno->wysokosc};
		sf::Vector2i pozycjaMyszkiPixel = sf::Mouse::getPosition(okno->window);
		sf::Vector2f pozycjaMyszki = okno->window.mapPixelToCoords(pozycjaMyszkiPixel);
		okno->statusObiektu = "Brak obiektu w tym miejscu";

		if (prostokatOkna.contains(pozycjaMyszkiPixel)) {
			ElementWiz kliknietyElement;
			bool znalezionoKlikniety = false;

			for (int i = 0; i < elementyWiz.size(); i++) {
				if (elementyWiz[i].zaznaczalny && elementyWiz[i].boundingBox.contains(pozycjaMyszki)) {
					okno->statusObiektu = elementyWiz[i].get_opis();
					if (okno->myszka) {
						kliknietyElement = elementyWiz[i];
						znalezionoKlikniety = true;
					}
				}
			}

			if (okno->myszka && znalezionoKlikniety) this->klikniete.push(kliknietyElement);

		}
		przekazDaneDoOknaKlatka(okno.get());
	}
}

void Wizualizacja::rysujOkna() {
	for (auto& okno : this->aktywneOkna) {
		okno->window.setActive(true);

		okno->rysujTlo();

		okno->rysujGlownyWidok();

		okno->rysujHUD();

		okno->rysujMinimape();
	}

	for (auto& okno : this->aktywneOkna) {
		okno->window.display();
	}
}

void Wizualizacja::obsluzTimery() {
	auto currentTime = clock.restart().asSeconds();
	auto fps = static_cast<int>(1.0f / currentTime + 0.5f);
	if (fpsCounter.getElapsedTime().asMilliseconds() >= 500) {
		for (auto& okno : this->aktywneOkna) {
			okno->window.setTitle("Wizualizacja [" + std::to_string(fps) + " FPS, " + std::to_string(okno->rysowaneObiekty) + " obj, " + std::to_string(okno->rysowaneTrojkaty / 3) + " trojk, " + std::to_string((double)(daneTrojkatow.size() * sizeof(float)) / 1024.0 / 1024.0) + " MB GPU, " + vendorName + ", " + openglVersion + "]");
		}
		fpsCounter.restart();
	}
	if (migajacaKomendaCounter.getElapsedTime().asMilliseconds() >= 600) {
		migajacaKomenda = !migajacaKomenda;
		migajacaKomendaCounter.restart();
	}
}

void Wizualizacja::obslugaOkna() {
	settings.antialiasingLevel = 4;

	std::unique_ptr<OknoWizualizacji> oknoPoczatkowe(new OknoWizualizacji(this->szerokoscPoczatkowa, this->wysokoscPoczatkowa, settings));
	this->inicjujGLEW();

	if (!this->wczytajAssety()) {
		std::cout << "BLAD WCZYTYWANIA ASSETOW, EXIT!" << std::endl;
		return;
	}

	this->przekazAssetyDoOkna(oknoPoczatkowe.get());

	this->aktywneOkna.push_back(std::move(oknoPoczatkowe));

	this->vendorName = (char*)glGetString(GL_VENDOR);
	this->openglVersion = (char*)glGetString(GL_VERSION);

	glGenBuffers(1, &vertexBuffer);

	unsigned long long dlugoscKlatkiOczekiwana = 1000000 / 60;

	while (!this->aktywneOkna.empty()) {
		auto poczatekKlatki = std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::steady_clock::now().time_since_epoch()).count();

		this->obsluzZdarzeniaOkien();

		this->block.lock();

		this->obsluzPozycjeMyszkiOkien();
		this->aktualizujVertexBuffer();

		this->block.unlock();

		this->rysujOkna();

		this->obsluzTimery();

		auto koniecKlatki = std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::steady_clock::now().time_since_epoch()).count();

		auto dlugoscKlatki = koniecKlatki - poczatekKlatki;

		auto pozostalyCzas = dlugoscKlatkiOczekiwana - dlugoscKlatki;

		if (pozostalyCzas > 0) {
			//while (std::chrono::duration_cast<std::chrono::microseconds>
			//	(std::chrono::steady_clock::now().time_since_epoch()).count() - poczatekKlatki < dlugoscKlatkiOczekiwana);

			std::this_thread::sleep_for(std::chrono::microseconds(pozostalyCzas));
		}
	}

	glDeleteBuffers(1, &vertexBuffer);
}
