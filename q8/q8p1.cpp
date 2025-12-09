#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
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

    bool operator==(Coord const& rhs) const { 
        return x == rhs.x && y == rhs.y && z == rhs.z; 
    }
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

    struct Comp {
        bool operator()(const Distance& a, const Distance& b) const noexcept {
            return a.squaredDistance < b.squaredDistance;
        }
    };
};
template <>
struct std::formatter<Distance> : std::formatter<std::string> {
    auto format(Distance d, format_context& ctx) const {
        return formatter<string>::format(std::format("{} -> {} ==> {}", d.from, d.to, d.squaredDistance), ctx);
    }
};

class UnionFind {                                
public:
    std::unordered_map<Coord, Coord, Coord::Hash, Coord::Eq> p; 
    std::unordered_map<Coord, long, Coord::Hash, Coord::Eq> rank;                           
    std::unordered_map<Coord, long, Coord::Hash, Coord::Eq> setSize;                           
    long numSets;
    
    void constructLoneSet(Coord i) {
        p.emplace(i, i);
        rank.emplace(i, 0);
        setSize.emplace(i, 1);
    }

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
        auto& x = findSet(i);
        auto& y = findSet(j);  
        Coord& higherRankNode = x;
        Coord& lowerRankNode = y;
        if (rank.at(y) > rank.at(x)) std::swap(higherRankNode, lowerRankNode);
        p.at(lowerRankNode) = higherRankNode;
        if (rank.at(higherRankNode) == rank.at(lowerRankNode)) rank.at(higherRankNode)++;
        setSize.at(higherRankNode) += setSize.at(lowerRankNode);
        numSets--;
    }
};

void radixSort(std::vector<Distance>& sqDistances, long maxDigits) {
	long base = 10;
	std::vector<std::vector<Distance>> holder;
    holder.resize(base);

	// Iterate over each digit (w)
	for (long sig = 0; sig < maxDigits; sig++) {
		// Iterate over each number (n) at current digit and place into holder accordingly
		for (long i = 0; i < sqDistances.size(); i++) {
            long digit = static_cast<long>(sqDistances[i].squaredDistance / std::pow(base, sig)) % base;
            holder[digit].emplace_back(sqDistances[i]);
		}
		// Iterate over each digit in the base (k) and place it's subarrays elements back into a in order
		long index = 0;
		for (long i = 0; i < base; i++) {
			for (int j = 0; j < holder[i].size(); j++) {
				sqDistances[index++] = holder[i][j];
			}
			holder[i].clear();
		}
	}
}

void radixSort(std::vector<long>& v, long maxDigits) {
	long base = 10;
	std::vector<std::vector<long>> holder;
    holder.resize(base);

	// Iterate over each digit (w)
	for (long sig = 0; sig < maxDigits; sig++) {
		// Iterate over each number (n) at current digit and place into holder accordingly
		for (long i = 0; i < v.size(); i++) {
            long digit = static_cast<long>(v[i] / std::pow(base, sig)) % base;
            holder[digit].emplace_back(v[i]);
		}
		// Iterate over each digit in the base (k) and place it's subarrays elements back into a in order
		long index = 0;
		for (long i = 0; i < base; i++) {
			for (int j = 0; j < holder[i].size(); j++) {
				v[index++] = holder[i][j];
			}
			holder[i].clear();
		}
	}
}

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Get all points
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

    // Get all squared distances O(n^2)
    std::vector<Distance> sqDistances;
    for (auto it = points.begin(); it != points.end(); it++) { 
        auto& currPoint = *it;
        for (auto innerIt = std::next(it); innerIt != points.end(); innerIt++) {
            auto& nextPoint = *innerIt;
            sqDistances.emplace_back(currPoint, nextPoint);
        }
    }
    
    // Radix sort on squared distances O(n^2)
    long maxSqDistance = std::max_element(sqDistances.begin(), sqDistances.end(), Distance::Comp())->squaredDistance;
    long maxDigits = 1;
    while (maxSqDistance >= 10) {
        maxSqDistance /= 10;
        maxDigits++;
    }
    radixSort(sqDistances, maxDigits);

    // Iterate and use UFDS to join circuits O(n^2)
    UnionFind uf;
    for (auto it = sqDistances.begin(); it != sqDistances.end(); it++) uf.unionSet(it->from, it->to);
    
    // Get UFDS unique set sizes O(n)
    std::unordered_set<long> uniqueSetSizes;
    for (auto it = uf.setSize.begin(); it != uf.setSize.end(); it++) uniqueSetSizes.insert(it->second);
    std::vector<long> uniqueSetSizesVector; uniqueSetSizesVector.reserve(uniqueSetSizes.size());
    for (auto it = uniqueSetSizes.begin(); it != uniqueSetSizes.end(); it++) uniqueSetSizesVector.emplace_back(*it);

    // Sort unique set sizes O(n)
    long maxSetSize = *std::max_element(uniqueSetSizesVector.begin(), uniqueSetSizesVector.end());
    long maxDigits1 = 1;
    while (maxSetSize >= 10) {
        maxSetSize /= 10;
        maxDigits1++;
    }
    radixSort(uniqueSetSizesVector, maxDigits1);
    /*for (auto it = uniqueSetSizesVector.begin(); it != uniqueSetSizesVector.end(); it++) std::println("{}", *it);*/

    // Get 3 largest set sizes and multiply O(1)
    out = 1;
    for (int i = uniqueSetSizesVector.size() - 1; i >= uniqueSetSizesVector.size() - 3; i--) out *= uniqueSetSizesVector[i];
    std::println("Out {}", out);

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::println("{}μs", duration.count());

    return 0;
}