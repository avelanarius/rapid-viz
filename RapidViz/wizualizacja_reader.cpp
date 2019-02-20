#include "wizualizacja_reader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "lz4.h"

WizualizacjaReader::WizualizacjaReader(std::string filename) {
    this->name = this->filenameToName(filename);

    this->loadTimestamps();

    this->watek = std::thread(&WizualizacjaReader::preload, this);
}

std::pair<bool, std::vector<ElementWiz>> WizualizacjaReader::getCurrentState() {
    std::pair<bool, std::vector<ElementWiz>> result;

    this->mutex.lock();

    const auto currentIndex = this->timeToIndex(this->currentTime);

    if (currentIndex == lastState) {
        result.first = false;
    } else if (!this->isBlockLoaded(currentIndex.first)) {
        result.first = false;
    } else {
        result.first = true;
        for (auto i = 0; i < loaded.size(); i++) {
            if (loaded[i].first == currentIndex.first) {
                result.second = loaded[i].second[currentIndex.second];
                break;
            }
        }

        lastState = currentIndex;
    }

    this->mutex.unlock();

    return result;
}

void WizualizacjaReader::preload() {
    while (true) {
        this->mutex.lock();

        const auto currentIndex = this->timeToIndex(this->currentTime);

        this->removeOldBlocks(currentIndex.first);

        auto blockToLoad = -1;
        for (auto i = 0; i < 4; i++) {
            if (!this->isBlockLoaded(currentIndex.first + i) && currentIndex.first + i < blockCount) {
                blockToLoad = currentIndex.first + i;
                break;
            }
        }
        this->mutex.unlock();

        if (blockToLoad == -1) {
            std::this_thread::sleep_for(std::chrono::microseconds(1000000 / 120));
        } else {
            const auto loadedBlock = this->loadBlock(blockToLoad);

            this->mutex.lock();

            this->loaded.push_back(loadedBlock);

            this->mutex.unlock();
        }
    }
}

void WizualizacjaReader::removeOldBlocks(int currentBlock) {
    this->loaded.erase(std::remove_if(loaded.begin(), loaded.end(), [currentBlock](const std::pair<int, std::vector<std::vector<ElementWiz>>>& elem) {
        return elem.first < currentBlock - 1 || elem.first > currentBlock + 4;
    }), loaded.end());
}

bool WizualizacjaReader::isBlockLoaded(int index) {
    return std::any_of(loaded.begin(), loaded.end(), [index](const std::pair<int, std::vector<std::vector<ElementWiz>>>& elem) {
        return elem.first == index;
    });
}

std::pair<int, std::vector<std::vector<ElementWiz>>> WizualizacjaReader::loadBlock(const int index) {
    std::pair<int, std::vector<std::vector<ElementWiz>>> result;

    result.first = index;

    auto data = this->loadAndDecompress(index);
    unsigned long long pos = 0;

    unsigned long long liczbaZbiorow;
    this->readFromVector(data, pos, reinterpret_cast<char *>(&liczbaZbiorow), sizeof(liczbaZbiorow));

    result.second.resize(liczbaZbiorow);

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        unsigned long long wielkoscZbioru;

        this->readFromVector(data, pos, reinterpret_cast<char *>(&wielkoscZbioru), sizeof(wielkoscZbioru));

        result.second[zbior].resize(wielkoscZbioru);
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            int dlugoscOpisu;
            this->readFromVector(data, pos, reinterpret_cast<char *>(&dlugoscOpisu), sizeof(dlugoscOpisu));

            result.second[zbior][rekord].opis.resize(dlugoscOpisu, ' ');
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, result.second[zbior][rekord].opis, result.second[zbior][rekord].opis.size());
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].boundingBox.left),
                                 sizeof(result.second[zbior][rekord].boundingBox.left));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].boundingBox.top),
                                 sizeof(result.second[zbior][rekord].boundingBox.top));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].boundingBox.height),
                                 sizeof(result.second[zbior][rekord].boundingBox.height));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].boundingBox.width),
                                 sizeof(result.second[zbior][rekord].boundingBox.width));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].zaznaczalny),
                                 sizeof(result.second[zbior][rekord].zaznaczalny));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].pozycja.first),
                                 sizeof(result.second[zbior][rekord].pozycja.first));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].pozycja.second),
                                 sizeof(result.second[zbior][rekord].pozycja.second));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].pozycjaDo.first),
                                 sizeof(result.second[zbior][rekord].pozycjaDo.first));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].pozycjaDo.second),
                                 sizeof(result.second[zbior][rekord].pozycjaDo.second));
        }
    }


    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].skala.first),
                                 sizeof(result.second[zbior][rekord].skala.first));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].skala.second),
                                 sizeof(result.second[zbior][rekord].skala.second));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].kolor.r),
                                 sizeof(result.second[zbior][rekord].kolor.r));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].kolor.g),
                                 sizeof(result.second[zbior][rekord].kolor.g));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].kolor.b),
                                 sizeof(result.second[zbior][rekord].kolor.b));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].kolor.a),
                                 sizeof(result.second[zbior][rekord].kolor.a));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].tekstura.idKsztaltu),
                                 sizeof(result.second[zbior][rekord].tekstura.idKsztaltu));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].typGrafiki),
                                 sizeof(result.second[zbior][rekord].typGrafiki));
        }
    }

    for (auto zbior = 0; zbior < liczbaZbiorow; zbior++) {
        for (auto rekord = 0; rekord < result.second[zbior].size(); rekord++) {
            this->readFromVector(data, pos, reinterpret_cast<char *>(&result.second[zbior][rekord].dodatkoweParametry),
                                 sizeof(result.second[zbior][rekord].dodatkoweParametry));
        }
    }

    return result;
}

std::vector<char> WizualizacjaReader::loadAndDecompress(const int index) {
    std::ifstream inputFile(this->name + "." + std::to_string(index) + ".dat", std::ios::binary);
    unsigned long long wielkoscDanychPrzedKompresja;
    unsigned long long wielkoscDanychPoKompresji;

    inputFile.read(reinterpret_cast<char*>(&wielkoscDanychPrzedKompresja), sizeof(wielkoscDanychPrzedKompresja));
    inputFile.read(reinterpret_cast<char*>(&wielkoscDanychPoKompresji), sizeof(wielkoscDanychPoKompresji));

    std::vector<char> danePoKompresji(wielkoscDanychPoKompresji);
    inputFile.read(danePoKompresji.data(), wielkoscDanychPoKompresji);

    std::vector<char> danePrzedKompresja(wielkoscDanychPrzedKompresja);
    const auto res = LZ4_decompress_safe(danePoKompresji.data(), danePrzedKompresja.data(), wielkoscDanychPoKompresji, wielkoscDanychPrzedKompresja);

    if (res < 0) {
        throw std::runtime_error("Blad dekompresji!");
    }

    inputFile.close();

    return danePrzedKompresja;
}

void WizualizacjaReader::readFromVector(std::vector<char>& vec, unsigned long long& index, char* dst,
                                        unsigned long long length) const {
    std::copy(vec.begin() + index, vec.begin() + index + length, dst);
    index += length;
}

void WizualizacjaReader::readFromVector(std::vector<char>& vec, unsigned long long& index, std::string& dst,
                                        unsigned long long length) const {
    std::copy(vec.begin() + index, vec.begin() + index + length, dst.begin());
    index += length;
}

void WizualizacjaReader::loadTimestamps() {
    auto index = 0;

    while (true) {
        std::ifstream inputFile(this->name + "." + std::to_string(index) + ".list", std::ios::binary);

        if (!inputFile.is_open()) break;

        unsigned long long liczbaRekordow;
        inputFile.read(reinterpret_cast<char*>(&liczbaRekordow), sizeof(liczbaRekordow));

        for (unsigned long long i = 0; i < liczbaRekordow; i++) {
            unsigned long long czas;
            inputFile.read(reinterpret_cast<char*>(&czas), sizeof(czas));

            if (index == 0 && i == 0) {
                pierwszyCzas = czas;
            }

            czas -= pierwszyCzas;

            this->length = czas;

            this->czasy.push_back(czas);
            this->czasToIndex[czas] = std::make_pair(index, i);
        }

        inputFile.close();

        index++;
    }
    blockCount = index;
}

inline bool ends_with(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string WizualizacjaReader::filenameToName(std::string filename) {
    if (ends_with(filename, ".list") || ends_with(filename, ".dat")) {
        const auto firstDotPosition = filename.rfind(".");
        filename = filename.substr(0, firstDotPosition);

        const auto secondDotPosition = filename.rfind(".");
        filename = filename.substr(0, secondDotPosition);
    }
    return filename;
}

std::pair<int, int> WizualizacjaReader::timeToIndex(const unsigned long long time) {
    auto firstGreater = std::upper_bound(czasy.begin(), czasy.end(), time);

    if (firstGreater == czasy.end()) {
        --firstGreater;
    } else {
        while (firstGreater != czasy.begin() && *firstGreater > time) {
            --firstGreater;
        }
    }

    const auto closestTime = *firstGreater;

    return this->czasToIndex[closestTime];
}
