#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <print>
#include <functional>
#include <cassert>

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Get all points
    long out = 0;
    std::unordered_map<std::string, std::vector<std::string>> adjacencyList;
    std::regex delimiter(",");
    // Kahn's BFS setup
    std::unordered_map<std::string, long> inDegrees;
    std::queue<std::string> queue;
    std::vector<std::string> topo; topo.reserve(12 << 1);
    std::unordered_map<std::string, long> numPaths;
    while (std::getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
        std::replace(line.begin(), line.end(), ' ', ',');
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        // Parse nodes
        std::string currHeadNode;
        std::sregex_token_iterator it(line.begin(), line.end(), delimiter, -1);
        std::sregex_token_iterator end;
        for (int i = 0; it != end; it++) {
            std::string s = it->str();
            adjacencyList.emplace(s, std::vector<std::string>());
            inDegrees.emplace(s, 0);
            numPaths.emplace(s, 0);
            if (i == 0) {
                currHeadNode = s;
            } else {
                adjacencyList.at(currHeadNode).emplace_back(s);
                inDegrees.at(s)++;
            }
            i++;
        }
    }
    file.close();

    // Do Kahn's BFS to get topological sort
    for (auto it = inDegrees.begin(); it != inDegrees.end(); it++) if (it->second == 0) queue.push(it->first);
    while (!queue.empty()) {
        std::string u = queue.front(); queue.pop();
        topo.emplace_back(u);
        for (std::string v: adjacencyList.at(u)) {
            inDegrees.at(v)--;
            if (inDegrees.at(v) == 0) queue.push(v);
        }
    }

    // Calculate number of paths in topological order
    numPaths.at(topo[0]) = 1;
    for (int i = 0; i < topo.size(); i++) {
        std::println("topo {}", topo[i]);
        for (int j = 0; j < adjacencyList.at(topo[i]).size(); j++) {
            numPaths.at(adjacencyList.at(topo[i])[j]) += numPaths.at(topo[i]);
            std::println("Number of paths at {} is now {}", adjacencyList.at(topo[i])[j], numPaths.at(adjacencyList.at(topo[i])[j]) );
        }
    }

    std::println("{}", numPaths["out"]);
  
    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::println("{}μs", duration.count());
    
    // Print result
    std::println("Out {}", out);

    return 0;
}