#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

#include <stdio.h>
#include <string.h>

using std::stringstream;

int main() 
{ 
    std::ifstream file("./entries/33.txt");
    std::vector<int> first_line;
    std::vector<int> second_line;

    std::string line;
    std::string strNumInput;
    bool is_first_line = true;

    while (getline(file, line)) {
        std::string entry;

        if (is_first_line == true) {
            char* c = const_cast<char*>(line.c_str());
            char* token = strtok(c, " ");
            // loop through the string to extract all other tokens
            while( token != NULL && strcmp(token, "\r") != 0) {
                first_line.push_back(std::stoi(token));
                token = strtok(NULL, " ");
            }
            is_first_line = false;
        } else {
            char* c = const_cast<char*>(line.c_str());
            char* token = strtok(c, " ");
            while( token != NULL && strcmp(token, "\r") != 0) {
                second_line.push_back(std::stoi(token));
                // printf(" %s\n", token ); //printing each token
                token = strtok(NULL, " ");
            }
        }
    }
    printf("Finished!");
}