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

template<class S, class T>
class BiUnorderedMap {
private:
    std::unordered_map<S, T> to;
    std::unordered_map<T, S> from;

public:
    BiUnorderedMap() {}

    bool emplace(S& s, T& t) {
        if (to.find(s) != to.end() || from.find(t) != from.end()) return false;
        to.emplace(s, t);
        from.emplace(t, s);
        return true;
    }

    void replace(S& s, T& t) {
        to.emplace(s, t);
        to.at(s) = t;
        from.emplace(t, s);
        to.at(t) = s;
    }

    const T& to_at(S& s) const {
        return to.at(s);
    }

    const S& from_at(T& t) const {
        return from.at(t);
    }

    std::unordered_map<S, T>::const_iterator to_begin() const {
        return to.begin();
    }

    std::unordered_map<S, T>::const_iterator to_end() const {
        return to.end();
    }
    
    std::unordered_map<T, S>::const_iterator from_begin() const {
        return from.begin();
    }

    std::unordered_map<T, S>::const_iterator from_end() const {
        return from.end();
    }
};

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Get all points
    std::regex delimiter(",");
    
    // Kahn's BFS setup
    std::vector<std::vector<long>> adjacencyList;
    std::queue<long> queue;
    std::vector<long> inDegrees;
    std::vector<long> topoOrder;
    
    // Normal BFS setup
    std::vector<long> visited;
    
    // Number of paths step
    std::vector<long> numPaths;

    // Parse nodes (map all nodes into IDs)
    BiUnorderedMap<std::string, long> nodeIdMap;
    long lastId = 0;
    while (std::getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
        std::replace(line.begin(), line.end(), ' ', ',');
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        std::sregex_token_iterator it(line.begin(), line.end(), delimiter, -1);
        std::sregex_token_iterator end;
        for (int i = 0; it != end; it++) {
            std::string s = it->str();
            if (nodeIdMap.emplace(s, lastId)) lastId++;
            i++;
        }
    }
    topoOrder.reserve(lastId);
    adjacencyList.resize(lastId);
    inDegrees.resize(lastId); 
    std::fill(inDegrees.begin(), inDegrees.end(), 0);
    visited.resize(lastId);
    std::fill(visited.begin(), visited.end(), false);
    numPaths.resize(lastId);
    std::fill(numPaths.begin(), numPaths.end(), 0);

    // Parse nodes (Create adjacency list)
    file.clear();
    file.seekg(0);
    while (std::getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
        std::replace(line.begin(), line.end(), ' ', ',');
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        long currHeadNode;
        std::sregex_token_iterator it(line.begin(), line.end(), delimiter, -1);
        std::sregex_token_iterator end;
        for (int i = 0; it != end; it++) {
            std::string s = it->str();
            if (i == 0) currHeadNode = nodeIdMap.to_at(s);
            else {
                adjacencyList[currHeadNode].emplace_back(nodeIdMap.to_at(s));
                inDegrees[nodeIdMap.to_at(s)]++;
            }
            i++;
        }
    }
    file.close();

    // DEBUG
    /*for (long i = 0; i < adjacencyList.size(); i++) {
        std::println("{}", nodeIdMap.from_at(i));
        for (int j = 0; j < adjacencyList[i].size(); j++) {
            std::println("\t {}", nodeIdMap.from_at(adjacencyList[i][j]));
        }
    }*/

    // Do normal BFS remove all nodes inaccessible from 'you'
    std::string you = "you";
    queue.push(nodeIdMap.to_at(you));
    visited[nodeIdMap.to_at(you)] = true;
    while (!queue.empty()) {
        auto u = queue.front(); queue.pop();        
        for (auto v : adjacencyList.at(u)) {
            if (visited[v] == false) { 
                visited[v] = true; 
                queue.push(v);                     
            }
        }
    }
    for (long i = 0; i < visited.size(); i++) {
        if (visited[i] == false) {
            for (int j = 0; j < adjacencyList[i].size(); j++) inDegrees[adjacencyList[i][j]]--;
            adjacencyList[i].clear();
            inDegrees[i] = -1;
            topoOrder[i] = -1;
            numPaths[i] = -1;
        }
    }

    // DEBUG
    /*for (long i = 0; i < inDegrees.size(); i++) {
        std::println("node {} has in-degree of {}", nodeIdMap.from_at(i), inDegrees[i]);
    }*/

    // Do Kahn's BFS to get topological sort
    for (long i = 0; i < inDegrees.size(); i++) {
        if (inDegrees[i] == 0) queue.push(i);
    }
    while (!queue.empty()) {
        long u = queue.front(); queue.pop();
        topoOrder.emplace_back(u);
        for (long v: adjacencyList[u]) {
            inDegrees[v]--;
            if (inDegrees[v] == 0) queue.push(v);
        }
    }

    // DEBUG
    /*for (long i = 0; i < topoOrder.size(); i++) {
        std::println("{}", nodeIdMap.from_at(topoOrder[i]));
    }*/

    // Calculate number of paths in topological order
    numPaths[topoOrder[0]] = 1;
    for (int i = 0; i < topoOrder.size(); i++) {
        for (int j = 0; j < adjacencyList[topoOrder[i]].size(); j++) {
            numPaths[adjacencyList[topoOrder[i]][j]] += numPaths[topoOrder[i]];
            //std::println("Number of paths at {} is now {}", nodeIdMap.from_at(adjacencyList[topoOrder[i]][j]), numPaths[adjacencyList[topoOrder[i]][j]]);
        }
    }
    
    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::println("{}μs", duration.count());

    // Print results
    std::string out = "out";
    std::println("{}", numPaths[nodeIdMap.to_at(out)]);

    return 0;
}