#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>

void openFile(std::string nameFile, std::unordered_map<std::string, std::vector<int>>& invertedList);
void printMap(std::unordered_map<std::string, std::vector<int>>& invertedList);

int main() {
    std::unordered_map<std::string, std::vector<int>> invertedList;
    openFile("files/bible-washed.txt", invertedList);
    //printMap(invertedList);
}

void openFile(std::string nameFile, std::unordered_map<std::string, std::vector<int>>& invertedList) {
    std::ifstream file{nameFile};
    std::string line, word;
    int lineNum = 1;

    if (!file) { std::cout << "Error opening file" << std::endl; return; }

    else {
        while(getline(file, line)) {
            std::stringstream temp (line);
            while(temp >> word) {
                if (invertedList[word].empty() || (invertedList[word].back() != lineNum))    { invertedList[word].push_back(lineNum); }
            }
            lineNum++;
        }
    }
}

void printMap(std::unordered_map<std::string, std::vector<int>>& invertedList) {
    for(const auto & elem : invertedList) {
        std::cout << elem.first << " : ";
        for(unsigned int i = 0; i < elem.second.size(); ++i) {
            std::cout << elem.second[i] << ", ";
        }
        std::cout << std::endl;
    }
}
