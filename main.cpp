#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>


int main {
    std::unordered_map<std::string, std::vector<int>> invertedList;
    openFile("oldhouse.txt", invertedList);
}

void openFile(std::string nameFile, std::unordered_map<std::string, std::vector<int>>& invertedList) {
    //std::unordered_map<std::string, std::vector<int>> invertedList;
    std::fstream file{nameFile};
    std::string line, word;
    int lineNum = 1;

    if (!file) { std::cout << "Error opening file" << std::endl; return; }
    
    else {
        while(getline(file, line)) {
            std::stringstream temp (line);
            while(temp >> word) {
                if (*(invertedList[word].end()-1) != lineNum)    { invertedList[word].push_back(lineNum); }
            }
            lineNum++;
        }
    }
}