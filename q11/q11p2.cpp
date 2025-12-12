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

#define SVR "svr"
#define DAC "dac"
#define FFT "fft"
#define OUT "out"

#define SVRDAC 0
#define SVRFFT 1
#define DACFFT 2
#define DACOUT 3
#define FFTDAC 4
#define FFTOUT 5
#define NUM_STATES 6

template<class S, class T>
class BiUnorderedMap {
private:
    std::unordered_map<S, T> to;
    std::unordered_map<T, S> from;

public:
    BiUnorderedMap() {}

    bool emplace(const S& s, const T& t) {
        if (to.find(s) != to.end() || from.find(t) != from.end()) return false;
        to.emplace(s, t);
        from.emplace(t, s);
        return true;
    }

    void replace(const S& s, const T& t) {
        to.emplace(s, t);
        to.at(s) = t;
        from.emplace(t, s);
        to.at(t) = s;
    }

    const T& to_at(const S& s) const {
        return to.at(s);
    }

    const S& from_at(const T& t) const {
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

    // Find svr->fft, svr->dac, fft->dac, fft->out, dac->fft, dac->out number of paths respectively
    std::vector<long> results; results.resize(NUM_STATES);
    std::vector<long> keyNodes = {nodeIdMap.to_at(SVR), nodeIdMap.to_at(FFT), nodeIdMap.to_at(DAC)};
    for (auto& keyNode : keyNodes) {
        // Reset all BFS data structures
        std::vector<std::vector<long>> adjacencyListCp = adjacencyList;
        std::vector<long> inDegreesCp = inDegrees;
        std::vector<long> topoOrderCp; topoOrderCp.reserve(lastId);
        std::vector<long> visitedCp = visited;
        std::vector<long> numPathsCp = numPaths;

        // Do normal BFS remove all nodes inaccessible from keyNode
        queue.push(keyNode);
        visitedCp[keyNode] = true;
        while (!queue.empty()) {
            auto u = queue.front(); queue.pop();        
            for (auto v : adjacencyListCp.at(u)) {
                if (visitedCp[v] == false) { 
                    visitedCp[v] = true; 
                    queue.push(v);                     
                }
            }
        }
        for (int i = 0; i < visitedCp.size(); i++) {
            if (visitedCp[i]) continue;
            for (int j = 0; j < adjacencyListCp[i].size(); j++) inDegreesCp[adjacencyListCp[i][j]]--;
            adjacencyListCp[i].clear();
            inDegreesCp[i] = -1;
            numPathsCp[i] = -1;
        }

        // Do Kahn's BFS to get topological sort
        for (long i = 0; i < inDegreesCp.size(); i++) if (inDegreesCp[i] == 0) queue.push(i);
        while (!queue.empty()) {
            long u = queue.front(); queue.pop();
            topoOrderCp.emplace_back(u);
            for (long v: adjacencyListCp[u]) {
                inDegreesCp[v]--;
                if (inDegreesCp[v] == 0) queue.push(v);
            }
        }

        // Calculate number of paths in topological order
        numPathsCp[topoOrderCp[0]] = 1;
        for (int i = 0; i < topoOrderCp.size(); i++) {
            for (int j = 0; j < adjacencyListCp[topoOrderCp[i]].size(); j++) {
                numPathsCp[adjacencyListCp[topoOrderCp[i]][j]] += numPathsCp[topoOrderCp[i]];
            }
        }

        // Get relevant data
        if (keyNode == nodeIdMap.to_at(SVR)) {
            results[SVRDAC] = numPathsCp[nodeIdMap.to_at(DAC)] == -1 ? 0 : numPathsCp[nodeIdMap.to_at(DAC)];
            results[SVRFFT] = numPathsCp[nodeIdMap.to_at(FFT)] == -1 ? 0 : numPathsCp[nodeIdMap.to_at(FFT)];
        } else if (keyNode == nodeIdMap.to_at(FFT)) {
            results[FFTDAC] = numPathsCp[nodeIdMap.to_at(DAC)] == -1 ? 0 : numPathsCp[nodeIdMap.to_at(DAC)];
            results[FFTOUT] = numPathsCp[nodeIdMap.to_at(OUT)] == -1 ? 0 : numPathsCp[nodeIdMap.to_at(OUT)];
        } else {
            results[DACFFT] = numPathsCp[nodeIdMap.to_at(FFT)] == -1 ? 0 : numPathsCp[nodeIdMap.to_at(FFT)];
            results[DACOUT] = numPathsCp[nodeIdMap.to_at(OUT)] == -1 ? 0 : numPathsCp[nodeIdMap.to_at(OUT)];
        }
    }
    
    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::println("{}μs", duration.count());

    // Print results
    long out = 0;
    out += results[SVRFFT] * results[FFTDAC] * results[DACOUT];
    out += results[SVRDAC] * results[DACFFT] * results[FFTOUT];
    std::println("{}", out);

    return 0;
}