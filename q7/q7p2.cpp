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

//#define LINE_LENGTH (142 - 1)
//#define LINE_NUMS (143 - 1)
#define LINE_LENGTH (16 - 1) 
#define LINE_NUMS (17 - 1)

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
    std::queue<long> queue;
    std::unordered_set<long> joinedQueue;
    std::unordered_map<long, std::unordered_set<long>> parents;
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
    queue.push(firstNodePos);
    while (!queue.empty()) {
        // Get node
        long currNode = queue.front();
        queue.pop();

        // Find neighbours
        long leftTrav = currNode - 1;
        long rightTrav = currNode + 1;
        while (leftTrav <= map.size() && map[leftTrav] != '^') {
            if (map[leftTrav] != 'S') map[leftTrav] = '|';
            leftTrav += LINE_LENGTH;
        }
        while (rightTrav <= map.size() && map[rightTrav] != '^') {
            if (map[rightTrav] != 'S') map[rightTrav] = '|';
            rightTrav += LINE_LENGTH;
        }

        // Update number of paths at each node, and continue only if valid
        if (leftTrav < map.size()) {
            if (!joinedQueue.contains(leftTrav)) { 
                queue.push(leftTrav);
                joinedQueue.insert(leftTrav);
            }
            numPaths.emplace(leftTrav, 0);
            numPaths.at(leftTrav) += numPaths.at(currNode);
        } 
        if (rightTrav < map.size()) { 
            if (!joinedQueue.contains(rightTrav)) { 
                queue.push(rightTrav);
                joinedQueue.insert(rightTrav);
            }
            numPaths.emplace(rightTrav, 0);
            numPaths.at(rightTrav) += numPaths.at(currNode);
        }
    }
    
    // DEBUG
    for (auto it = numPaths.begin(); it != numPaths.end(); it++) std::println("[{}, {}]", it->first, it->second);
    /*for (auto it = parents.begin(); it != parents.end(); it++) {
        std::print("{}: ", it->first);
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            std::print("{}, ", *it1);
        }
        std::println();
    }*/

    // Output
    std::println("Out {}", numPaths.size());

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "μs" << std::endl;

    return 0;
}