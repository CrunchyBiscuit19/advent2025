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

#define INF_DISTANCE 1e180
#define LONGEST_STATE 12

class State {
private:
    std::vector<long> state;
    long last;

public:
    State(): last(0) {
        state.resize(LONGEST_STATE);
        for (int i = 0; i < state.size(); i++) {
            state[i] = 0;
        }
    }

    void emplace_back(long a) {
        if (last == LONGEST_STATE) return;
        state[last++] = a;
    }

    std::string toString() {
        std::string out;
        for (int i = 0; i < state.size(); i++) {
            out += std::format("{}", state[i]);
            out += ' ';
        }
        return out;
    }

    long operator[](long i) {
        return state[i];
    }

    State& operator+=(const State& rhs) {
        for (int i = 0; i < state.size(); i++) {
            state[i] += rhs.state[i];
        }
        return *this;
    }

    State operator+(const State& rhs) {
        State s;
        s += rhs; 
        return s; 
    }

    bool operator==(const State& rhs) {
        for (int i = 0; i < state.size(); i++) if (state[i] != rhs.state[i]) return false;
        return true;
    }

    bool operator>(const State& rhs) {
        for (int i = 0; i < state.size(); i++) if (state[i] <= rhs.state[i]) return false;
        return true;
    }

    bool operator>=(const State& rhs) {
        for (int i = 0; i < state.size(); i++) if (state[i] < rhs.state[i]) return false;
        return true;
    }

    bool operator<(const State& rhs) {
        for (int i = 0; i < state.size(); i++) if (state[i] >= rhs.state[i]) return false;
        return true;
    }

    bool operator<=(const State& rhs) {
        for (int i = 0; i < state.size(); i++) if (state[i] > rhs.state[i]) return false;
        return true;
    }

    struct Hash {
        std::size_t operator()(const State& a) const noexcept {
            std::size_t h = 0;
            for (int i = 0; i < a.state.size(); i++) {
                h ^= std::hash<std::size_t>{}(a.state[i]) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
            }
            return h;
        }
    };

    struct Eq {
        bool operator()(const State& a, const State& b) const noexcept {
            for (int i = 0; i < a.state.size(); i++) if (a.state[i] != b.state[i]) return false;
            return true;
        }
    };
};

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Get all points
    long out = 0;
    State source;
    std::regex tgtRegex("\\{(.*?)\\}");
    std::regex tgtDelimiter(",");
    std::regex edgeRegex("\\((.*?)\\)");
    std::regex edgeDelimiter(",");
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        
        // Parse target
        State tgtState;
        std::smatch tgtMatch;
        std::regex_search(line, tgtMatch, tgtRegex);
        std::string tgtString = tgtMatch.str().substr(1, tgtMatch.str().size() - 2);
        std::sregex_token_iterator it(tgtString.begin(), tgtString.end(), tgtDelimiter, -1);
        std::sregex_token_iterator end;
        while (it != end) {
            tgtState.emplace_back(std::stol(it->str()));
            it++;
        }
        
        // Parse edges
        std::vector<const State> edges;
        std::smatch edgeMatch;
        while (std::regex_search(line, edgeMatch, edgeRegex)) {
            State edge;
            std::string edgeLights = edgeMatch.str().substr(1, edgeMatch.str().size() - 2);
            std::sregex_token_iterator it(edgeLights.begin(), edgeLights.end(), edgeDelimiter, -1);
            std::sregex_token_iterator end;
            while (it != end) {
                edge.emplace_back(std::stol(it->str()));
                it++;
            }
            edges.emplace_back(edge);
            line = edgeMatch.suffix().str();
        }
        
        // BFS traversal
        /*std::unordered_map<const State&, long> distance; 
        std::queue<const State&, long> queue;

        distance.emplace(source, 0);
        distance.at(source) = 0;
        queue.push(source);
        while (!queue.empty()) { 
            const State& currNode = queue.front(); queue.pop();
            for (int i = 0; i < edges.size(); i++) { 
                State nextNode = currNode + edges[i]; 
                if (distance.at(nextNode) == INF_DISTANCE) queue.push(nextEdge);
                if (distance[currEdge] + 1 < distance[nextEdge]) distance[nextEdge] = distance[currEdge] + 1;
            }
        }
        out += distance[tgt];*/
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