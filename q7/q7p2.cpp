#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <print>

#define LINE_LENGTH (142 - 1)
#define LINE_NUMS (143 - 1)
//#define LINE_LENGTH (16 - 1) 
//#define LINE_NUMS (17 - 1)

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Build map
    long out = 0;
    std::string map; map.reserve(LINE_LENGTH * LINE_LENGTH);
    std::unordered_map<long, long> numPaths;
    std::priority_queue<long, std::vector<long>, std::greater<long>> minHeap;
    std::unordered_set<long> joinedHeap;
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        map.append(line);
    }
    file.close();
    
    // Find first node
    long firstNodePos = 0;
    for (int i = 0; i < LINE_LENGTH; i++) {
        if (map[i] == 'S') {
            while (map[i] != '^') i += LINE_LENGTH;
            firstNodePos = i;
            break;
        }
    }
    numPaths.emplace(firstNodePos, 1);

    // BFS
    minHeap.push(firstNodePos);
    while (!minHeap.empty()) {
        // Get node
        long currNode = minHeap.top();
        minHeap.pop();

        // Quit at leaf nodes
        if (currNode >= map.size() - LINE_LENGTH) {
            out += numPaths.at(currNode);
            continue;
        }

        // Find neighbours
        long leftTrav = currNode - 1;
        long rightTrav = currNode + 1;
        while (leftTrav < map.size() - LINE_LENGTH && map[leftTrav] != '^') {
            if (map[leftTrav] != 'S') map[leftTrav] = '|';
            leftTrav += LINE_LENGTH;
        }
        while (rightTrav < map.size() - LINE_LENGTH && map[rightTrav] != '^') {
            if (map[rightTrav] != 'S') map[rightTrav] = '|';
            rightTrav += LINE_LENGTH;
        }

        // Update number of paths at each node, and continue only if valid
        if (!joinedHeap.contains(leftTrav)) { 
            minHeap.push(leftTrav);
            joinedHeap.insert(leftTrav);
        }
        numPaths.emplace(leftTrav, 0);
        numPaths.at(leftTrav) += numPaths.at(currNode);
        if (!joinedHeap.contains(rightTrav)) { 
            minHeap.push(rightTrav);
            joinedHeap.insert(rightTrav);
        }
        numPaths.emplace(rightTrav, 0);
        numPaths.at(rightTrav) += numPaths.at(currNode);
    }
    
    // Output
    std::println("Out {}", out);

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "μs" << std::endl;

    return 0;
}