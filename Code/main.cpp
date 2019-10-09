#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <chrono>

void openFile(std::string nameFile, std::unordered_map<std::string, std::vector<int>>& invertedList);
void query(std::unordered_map<std::string, std::vector<int>>& invertedList);
int skipTo(std::vector<int>& vec, int index, unsigned int start);
void printMap(std::unordered_map<std::string, std::vector<int>>& invertedList);


int main() {
    /// The unordered map store the word as the key and a vector of ints (representing the line the word is in) as the second element
    std::unordered_map<std::string, std::vector<int>> invertedList;
    openFile("files/bible-washed.txt", invertedList); /// We initialize the inverted list with the wanted .txt file

    /// If uncommented : it allows you to see the entire inverted list
    //printMap(invertedList);

    /// Infinite loop for inputs and researches
    while(true){
        query(invertedList);
    }
}



    ///****************    OpenFile    ***************///
/** Parameters : 1. name of the file ; 2. invertedList that needs to be initialized **/
/** Open the file and store each word in the map while putting in the linked integers vectors the numbers of the line the word is in **/

void openFile(std::string nameFile, std::unordered_map<std::string, std::vector<int>>& invertedList) {
    std::ifstream file{nameFile};
    std::string line, word;
    int lineNum = 1;

    auto t_start = std::chrono::high_resolution_clock::now(); // Store the start time of the creation of the inverted list

    if (!file) { std::cout << "Error opening file" << std::endl; return; }

    else {
        /// While there are lines that are not analyzed
        while(getline(file, line)) {
            /// Create a stingstream that contains the line
            std::istringstream temp (line);
            /// We put each word of the stringstream in a string, one by one
            while(temp >> word) {
                /// We either add the number of the line to the vector of ints, or we create the pair with the string and the number of the line
                auto it = invertedList.find(word);
                if(it == invertedList.end())    { invertedList.insert({word, {lineNum}}); }
                else if(it->second.back() != lineNum)   { it->second.push_back(lineNum); }
            }
            lineNum++; // We change line so we increment the variable
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now(); // Store the end time

    std::cout << "Creating inverted list was " << std::chrono::duration<double, std::milli>(t_end - t_start).count() << " ms long";
}



    ///***************    Query    ***************///
/*** Parameter : 1. the inverted list in which we are going to search for the query ***/
/*** Take an input consisting of words. Search in the inverted list to find the sentences that contain every word that is in the query ***/
/*** Shows : 1. the time it took to find the good sentence ; 2. the sentences index numbers ; 3. the time it took to find and print everything ***/

void query(std::unordered_map<std::string, std::vector<int>>& invertedList) {
    std::string queryLine;
    std::string word;
    std::vector<int> results;
    std::unordered_map<std::string, std::vector<int>> invertedListSearch;

    /// The user enter a query line
    std::cout << std::endl << "search?> ";
    getline(std::cin, queryLine);

    auto t_start = std::chrono::high_resolution_clock::now(); // Store the start time of the research

    std::istringstream line(queryLine);

    /// For each word
    while(line >> word) {
        /// We search it in the inverted list
        auto searchIt = invertedList.find(word);
        if(searchIt != invertedList.end()) {
            ///If it is in the list, we add it in a second list
            invertedListSearch.insert({word, searchIt->second});

            /// If this is uncommented, it shows the results for each word
            /*std::cout << std::endl << "matches for " << word << " : " << searchIt->second[0];
            for(unsigned int i = 1; i < searchIt->second.size(); ++i) {
                std::cout << ", " << searchIt->second[i];
            }*/
        }
        else {
            /// If it is not in the list we add it in the second list with an empty vector
            invertedListSearch.insert({word, {}});
            //std::cout << word << " : No match found" << std::endl;
        }
    }

    /// We then search for the term with the smallest number of occurrences
    std::string wordSmallList;
    unsigned int smallestList=0;

    /// We store in wordSmallList the word with the smalles number of occurrences and in smallestList the actual nbr of occurrences
    for(auto it = invertedListSearch.begin(); it != invertedListSearch.end(); ++it) {
        if (it == invertedListSearch.begin()) {
            wordSmallList = it->first;
            smallestList = it->second.size();
        }
        else if(it->second.size() < smallestList){
            wordSmallList = it->first;
            smallestList = it->second.size();
            if (smallestList == 0)    break;
        }
    }


    /// If there is no word that are in 0 sentence we fill the results vector that store the number of the sentences that match every word
    if(smallestList != 0) {
        auto searchIt = invertedListSearch.find(wordSmallList);
        results=searchIt->second;
        for(auto it = invertedListSearch.begin(); it != invertedListSearch.end() && results.size() > 0 ; ++it) {
            if(it != searchIt) {
                std::vector<int> resultsTemp;
                unsigned int i = 0, j = 0;
                while (i < results.size() && j < it->second.size()) {
                    if(results[i]<it->second[j]) {
                        i = skipTo(results, it->second[j], i);
                    }
                    else {
                        j = skipTo(it->second, results[i], j);
                    }
                    if(results[i] == it->second[j]) {
                        resultsTemp.push_back(results[i]);
                        ++i;
                    }
                }
                results = resultsTemp;
            }
        }
    }

    /// We print everything
    auto t_endResearch = std::chrono::high_resolution_clock::now();
    std::cout << std::endl << "Research was " << std::chrono::duration<double, std::milli>(t_endResearch - t_start).count() << " ms long" << std::endl;

    if(results.size() == 0) {
        std::cout << std::endl << "No match found";
    }
    else {
        std::cout << std::endl << "final matches : " << results[0];

        for(unsigned int i = 1; i < results.size(); ++i) {
            std::cout << ", " << results[i];
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now(); // Store the end time

    std::cout << std::endl << std::endl << "Research and print was " << std::chrono::duration<double, std::milli>(t_end - t_start).count() << " ms long" << std::endl << std::endl;
}



    ///***************    SkipTo    ***************///
/** Parameters : 1. a vector we want to move forward in ; 2. a number we want to find or pass in the vector ; 3. the start index number in the vector **/
/** Returns : 1. the index that find or pass nbrToFind if such an index exists ; 2. the size of the vector if there isn't such index **/

int skipTo(std::vector<int>& vec, int nbrToFind, unsigned int start) {
    for(unsigned int i = start; i < vec.size(); ++i) {
        if(vec[i] >= nbrToFind){
            return i;
        }
    }
    return vec.size();
}



    ///***************    PrintMap    ***************///
/** Parameters : 1. unordered_map to print **/
/** Print every first element of the list followed by its integers vector **/

void printMap(std::unordered_map<std::string, std::vector<int>>& invertedList) {
    for(const auto & elem : invertedList) {
        std::cout << elem.first << " : ";
        for(unsigned int i = 0; i < elem.second.size(); ++i) {
            std::cout << elem.second[i] << ", ";
        }
        std::cout << std::endl;
    }
}
