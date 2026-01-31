#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <limits>
#include <cmath>
#include <functional>

namespace sf {

// Node structure
struct Node {
    int id;
    double x, y;
    
    Node(int id = -1, double x = 0, double y = 0) : id(id), x(x), y(y) {}
    
    bool operator==(const Node& other) const {
        return id == other.id;
    }
    
    double distance(const Node& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

// Edge structure
struct Edge {
    int from, to;
    double weight;
    
    Edge(int f, int t, double w) : from(f), to(t), weight(w) {}
};

// Graph class
class Graph {
private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, std::vector<Edge>> adjacency;
    
public:
    void addNode(const Node& node);
    void addEdge(int from, int to, double weight);
    void addEdgeUndirected(int from, int to, double weight);
    
    const Node& getNode(int id) const;
    const std::vector<Edge>& getNeighbors(int id) const;
    const std::unordered_map<int, Node>& getNodes() const;
    
    bool hasNode(int id) const;
    size_t nodeCount() const;
    size_t edgeCount() const;
    
    std::vector<int> getNodeIds() const;
    
    void clear();
};

// Path result
struct PathResult {
    bool found;
    std::vector<int> path;
    double cost;
    size_t nodesExplored;
    
    PathResult() : found(false), cost(0.0), nodesExplored(0) {}
    
    PathResult(const std::vector<int>& p, double c, size_t n) 
        : found(true), path(p), cost(c), nodesExplored(n) {}
};

// Heuristic function type
using Heuristic = std::function<double(const Node&, const Node&)>;

// Common heuristics
namespace heuristics {
    double euclidean(const Node& a, const Node& b);
    double manhattan(const Node& a, const Node& b);
    double zero(const Node& a, const Node& b);
}

} // namespace sf
