#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <print>
#include <functional>

struct Coord {
    long x;
    long y;
    long z;

    Coord(long x, long y, long z): x(x), y(y), z(z) {}

    struct Hash {
        std::size_t operator()(const Coord& c) const noexcept {
            std::size_t h1 = std::hash<int>{}(c.x);
            std::size_t h2 = std::hash<int>{}(c.y);
            std::size_t h3 = std::hash<int>{}(c.z);
            return (h1 ^ (h2 << 1)) ^ h3;
        }
    };

    struct Eq {
        bool operator()(const Coord& a, const Coord& b) const noexcept {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }
    };

    struct Comp {
        bool operator()(const Coord& a, const Coord& b) const noexcept {
            if (a.x == b.x) {
                if (a.y == b.y) return a.z < b.z;
                return a.y < b.y;
            }
            return a.x < b.x;
        }
    };

    bool operator==(Coord const&) const = default;
};
template <>
struct std::formatter<Coord> : std::formatter<std::string> {
    auto format(Coord p, format_context& ctx) const {
        return formatter<string>::format(std::format("[{}, {}, {}]", p.x, p.y, p.z), ctx);
    }
};

struct Distance {
    Coord from;
    Coord to;
    double squaredDistance;

    Distance(Coord& from, Coord& to) : from(from), to(to) { // Should be fine. We only need relative distance.
        squaredDistance = std::pow(from.x - to.x, 2) + std::pow(from.y - to.y, 2) + std::pow(from.z - to.z, 2);
    }
};
template <>
struct std::formatter<Distance> : std::formatter<std::string> {
    auto format(Distance d, format_context& ctx) const {
        return formatter<string>::format(std::format("{} -> {} ==> {}", d.from, d.to, d.squaredDistance), ctx);
    }
};

class UnionFind {                                
private:
    std::unordered_map<Coord, Coord, Coord::Hash, Coord::Eq> p; 
    std::unordered_map<Coord, long, Coord::Hash, Coord::Eq> rank;                           
    std::unordered_map<Coord, long, Coord::Hash, Coord::Eq> setSize;                           
    long numSets;
    
    void constructLoneSet(Coord i) {
        p.emplace(i, i);
        rank.emplace(i, 0);
        setSize.emplace(i, 1);
    }

public:
    UnionFind() : numSets(0) {}

    Coord& findSet(Coord& i) {
        if (p.find(i) == p.end()) constructLoneSet(i);
        if (p.at(i) == i) {
            return i;
        } else {
            p.at(i) = findSet(p.at(i));
            return p.at(i);
        }
    }

    bool isSameSet(Coord& i, Coord& j) { 
        return findSet(i) == findSet(j); 
    }

    long numDisjointSets() { 
        return numSets; 
    }

    long sizeOfSet(Coord i) { 
        return setSize.at(findSet(i)); 
    } 

    void unionSet(Coord& i, Coord& j) {
        if (isSameSet(i, j)) return;                 
        auto x = findSet(i), y = findSet(j);  
        Coord& higherRankNode = i;
        Coord& lowerRankNode = j;
        if (rank.at(j) > rank.at(i)) {
            higherRankNode = j;
            lowerRankNode = i;
        }
        p.at(lowerRankNode) = higherRankNode;
        if (rank.at(higherRankNode) == rank.at(lowerRankNode)) rank.at(higherRankNode)++;
        setSize.at(higherRankNode) += setSize.at(lowerRankNode);
        numSets--;
    }
};

void dividePoints(std::vector<Coord>& points, long start, long end) {
    /*std::print("{}, {} ", start, end);
    for (int i = start; i <= end; i++) std::print("{}, ", points[i]);
    std::println();*/
    if (end - start + 1 <= 3) return; 
    long mid = start + (end + 1 - start) / 2;
    dividePoints(points, start, mid - 1);
    dividePoints(points, mid, end);
}

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    std::vector<Coord> points;
    std::regex del(",");
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        std::sregex_token_iterator it(line.begin(), line.end(), del, -1);
        std::sregex_token_iterator end;
        long x; long y; long z;
        for (int i = 0; it != end; it++) {
            switch (i++) {
                case 0:
                    x = std::stol(*it);
                    break;
                case 1:
                    y = std::stol(*it);
                    break;
                case 2:
                    z = std::stol(*it);
                    break;
            }  
        } 
        points.emplace_back(x, y, z); 
    }
    file.close();

    std::sort(points.begin(), points.end(), Coord::Comp());
    //for (auto it = points.begin(); it != points.end(); it++) std::println("{}", *it);
    dividePoints(points, 0, points.size() - 1);
    

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "μs" << std::endl;

    return 0;
}