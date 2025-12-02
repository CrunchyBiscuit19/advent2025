#include <fstream>
#include <iostream>
#include <string>


int main()
{
    std::ifstream file("input");
    std::string line;
    if (!file.is_open()) return 0;

    int score = 50;
    int zeroes = 0;
    while (std::getline(file, line)) {
        char direction = line[0];
        int degree = stoi(line.substr(1, line.size() - 1)) % 100;
        if (direction == 'L') {
            score -= degree;
            if (score < 0) score += 100;
        } else if (direction == 'R') {
            score += degree;
            if (score >= 100) score -= 100;
        }
        if (score == 0) zeroes++;
    }
    file.close();

    std::cout << zeroes << std::endl;

    return 0;
}