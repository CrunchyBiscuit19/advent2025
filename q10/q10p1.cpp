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

#define INF_DISTANCE 1e18
#define SOURCE 0

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Get all points
    long out = 0;
    std::regex tgtRegex("\\[(.*?)\\]");
    std::regex edgeRegex("\\((.*?)\\)");
    std::regex edgeDelimiter(",");
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        
        // Parse target
        long tgt = 0;
        std::smatch tgtMatch;
        std::regex_search(line, tgtMatch, tgtRegex);
        std::string tgtLights = tgtMatch.str();
        for (int i = 0; i < tgtLights.size(); i++) {
            if (tgtLights[i] == '#') {
                tgt |= 1 << (i - 1);
            }
        }
        std::println("Target {}", tgt);
        
        // Parse edges
        std::vector<long> edges;
        std::smatch edgeMatch;
        while (std::regex_search(line, edgeMatch, edgeRegex)) {
            long edge = 0;
            std::string edgeLights = edgeMatch.str().substr(1, edgeMatch.str().size() - 2);
            std::sregex_token_iterator it(edgeLights.begin(), edgeLights.end(), edgeDelimiter, -1);
            std::sregex_token_iterator end;
            while (it != end) {
                edge |= 1 << std::stol(*it);
                it++;
            }
            edges.emplace_back(edge);
            line = edgeMatch.suffix().str();
            std::println("Edge {}", edge);
        }
        
        // BFS traversal
        long max = std::pow(2, tgtLights.size()) - 1;
        std::vector<long> visited; visited.resize(max);
        std::vector<long> distance; distance.resize(max);
        for (int i = 0; i < distance.size(); i++) distance[i] = INF_DISTANCE;
        std::queue<long> queue;

        distance[SOURCE] = 0;
        queue.push(SOURCE);
        while (!queue.empty()) { 
            long currEdge = queue.front(); queue.pop();
            for (int i = 0; i < edges.size(); i++) { 
                long nextEdge = currEdge ^ edges[i]; 
                if (distance[nextEdge] == INF_DISTANCE) queue.push(nextEdge);
                if (distance[currEdge] + 1 < distance[nextEdge]) distance[nextEdge] = distance[currEdge] + 1;
            }
        }
        out += distance[tgt];
        std::println("{} button presses to {}", distance[tgt], tgt);
    }
    file.close();
  
    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::println("{}μs", duration.count());
    
    // Print result
    std::println("Out {}", out);

    return 0;
}