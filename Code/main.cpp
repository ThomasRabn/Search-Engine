#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>
#include <chrono>

void openFile(std::string nameFile, std::unordered_map<std::string, std::vector<int>>& invertedList);
void printMap(std::unordered_map<std::string, std::vector<int>>& invertedList);
void query(std::unordered_map<std::string, std::vector<int>>& invertedList);

int main() {
    std::unordered_map<std::string, std::vector<int>> invertedList;
    openFile("files/bible-washed.txt", invertedList);
    //printMap(invertedList);
    while(true){
        query(invertedList);
    }
}

void openFile(std::string nameFile, std::unordered_map<std::string, std::vector<int>>& invertedList) {
    std::ifstream file{nameFile};
    std::string line, word;
    int lineNum = 1;

    auto t_start = std::chrono::high_resolution_clock::now(); // Store the start time of the sorting algorithm

    if (!file) { std::cout << "Error opening file" << std::endl; return; }

    else {
        while(getline(file, line)) {
            std::istringstream temp (line);
            while(temp >> word) {
                auto it = invertedList.find(word);
                if   (it != invertedList.end() && it->second.back() != lineNum) { it->second.push_back(lineNum); }
                else { invertedList.insert({word, {lineNum}}); }
            }
            lineNum++;
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now(); // Store the end time

    std::cout << std::chrono::duration<double, std::milli>(t_end - t_start).count() << " ms long";
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

void query(std::unordered_map<std::string, std::vector<int>>& invertedList) {
    std::string queryLine;
    std::string word;
    std::vector<int> results;
    std::unordered_map<std::string, std::vector<int>> invertedListSearch;

    std::cout << std::endl << "search?> ";
    getline(std::cin, queryLine);
    std::istringstream line(queryLine);

    while(line >> word) {
        auto searchIt = invertedList.find(word);
        if(searchIt != invertedList.end()) {
            std::cout << std::endl << "matches for " << word << " : " << searchIt->second[0];
            for(unsigned int i = 1; i < searchIt->second.size(); ++i) {
                std::cout << ", " << searchIt->second[i];
            }
            invertedListSearch.insert({word, searchIt->second});
        }
        else {
            std::cout << word << " : No match found" << std::endl;
            invertedListSearch.insert({word, {}});
        }
    }

    std::string wordSmallList;
    unsigned int smallestList=0;

    for(auto it = invertedListSearch.begin(); it != invertedListSearch.end(); ++it) {
        if (it == invertedListSearch.begin()) {
            wordSmallList = it->first;
            smallestList = it->second.size();
        }
        else if(it->second.size() < smallestList){
            wordSmallList = it->first;
            smallestList = it->second.size();
        }
    }

    if (smallestList == 0) {
        std::cout << "No match found" << std::endl;
    }
    else {
        auto searchIt = invertedListSearch.find(wordSmallList);
        std::cout << std::endl << "matches for " << word << " : " << searchIt->second[0];
        for(unsigned int i = 1; i < searchIt->second.size(); ++i) {
            std::cout << ", " << searchIt->second[i];
        }
        invertedListSearch.insert({word, searchIt->second});
    }


    for(auto const it : invertedListSearch) {
        std::cout << it.first << " : " << it.second.size() << std::endl;
    }
}
