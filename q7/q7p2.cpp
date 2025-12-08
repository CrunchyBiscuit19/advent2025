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

using Coords = std::pair<long, long>;

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Build map
    auto hashFn = [](const Coords& k) { return std::hash<long>()(k.first) ^ (std::hash<long>()(k.second) << 1); };
    auto eqFn = [](const Coords& a, const Coords& b) { return a.first == b.first && a.second == b.second; };
    auto cmpFn = [](const Coords& a, const Coords& b) { 
        if (a.first == b.first) return a.second < b.second;
        return a.first < b.first; 
    };
    auto inverseCmpFn = [](const Coords& a, const Coords& b) { 
        if (a.first == b.first) return a.second > b.second;
        return a.first > b.first; 
    };
    std::vector<std::string> map; map.reserve(LINE_NUMS);
    std::unordered_map<Coords, long, decltype(hashFn), decltype(eqFn)> numPaths;
    std::priority_queue<Coords, std::vector<Coords>, decltype(inverseCmpFn)> queue;
    std::unordered_set<Coords, decltype(hashFn), decltype(eqFn)> enqueued;

    long out = 0;
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        map.emplace_back(line);
    }
    file.close();
    
    // Find first node
    Coords firstNodePos;
    for (int i = 0; i < LINE_LENGTH; i++) {
        if (map[0][i] == 'S') {
            long j = 0;
            while (map[j][i] != '^') j++;
            firstNodePos = Coords(j, i);
            break;
        }
    }
    numPaths.emplace(firstNodePos, 1L);

    // BFS
    queue.push(firstNodePos);
    while (!queue.empty()) {
        // Get node
        Coords currNode = queue.top();
        queue.pop();

        // Quit at leaf nodes
        if (currNode.first >= LINE_NUMS - 1) {
            out += numPaths.at(currNode);
            continue;
        }

        // Find neighbours
        Coords leftTrav = Coords(currNode.first, currNode.second - 1);
        Coords rightTrav = Coords(currNode.first, currNode.second + 1);
        while (leftTrav.first < LINE_NUMS - 1 && map[leftTrav.first][leftTrav.second] != '^') {
            if (map[leftTrav.first][leftTrav.second] != 'S') map[leftTrav.first][leftTrav.second] = '|';
            leftTrav.first++;
        }
        while (rightTrav.first < LINE_NUMS - 1 && map[rightTrav.first][rightTrav.second] != '^') {
            if (map[rightTrav.first][rightTrav.second] != 'S') map[rightTrav.first][rightTrav.second] = '|';
            rightTrav.first++;
        }

        // Update number of paths at each node, and continue only if valid
        if (!enqueued.contains(leftTrav)) { 
            queue.push(leftTrav);
            enqueued.insert(leftTrav);
        }
        numPaths.emplace(leftTrav, 0);
        numPaths.at(leftTrav) += numPaths.at(currNode);
        if (!enqueued.contains(rightTrav)) { 
            queue.push(rightTrav);
            enqueued.insert(rightTrav);
        }
        numPaths.emplace(rightTrav, 0);
        numPaths.at(rightTrav) += numPaths.at(currNode);
    }

    for (auto it = numPaths.begin(); it != numPaths.end(); it++) std::println("[{}, {}] = {}", it->first.first, it->first.second, it->second);
    
    // Output
    std::println("Out {}", out);

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "μs" << std::endl;

    return 0;
}