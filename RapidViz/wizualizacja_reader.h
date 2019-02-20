#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include "element_wiz.h"

class WizualizacjaReader {
public:
    WizualizacjaReader(std::string filename);

    unsigned long long currentTime = 0;

    unsigned long long length = 0;

    unsigned long long pierwszyCzas = 0;

    std::pair<bool, std::vector<ElementWiz>> getCurrentState();
private:
    void preload();
    void removeOldBlocks(int currentBlock);
    bool isBlockLoaded(int index);

    std::pair<int, std::vector<std::vector<ElementWiz>>> loadBlock(const int index);
    std::vector<char> loadAndDecompress(const int index);

    void readFromVector(std::vector<char>& vec, unsigned long long& index, char* dst, unsigned long long length) const;
    void readFromVector(std::vector<char>& vec, unsigned long long& index, std::string& dst, unsigned long long length) const;

    void loadTimestamps();

    std::string filenameToName(std::string filename);

    std::pair<int, int> timeToIndex(const unsigned long long time);

    std::thread watek;
    std::mutex mutex;

    std::vector<std::pair<int, std::vector<std::vector<ElementWiz>>>> loaded;

    std::string name;
    std::vector<unsigned long long> czasy;
    std::map<unsigned long long, std::pair<int, int>> czasToIndex;
    int blockCount = 0;

    std::pair<int, int> lastState = { -1, -1 };
};
