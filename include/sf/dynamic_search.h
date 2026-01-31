#pragma once

#include "graph.h"
#include <unordered_map>
#include <unordered_set>

namespace sf {

// 3.1 D* (Dynamic A*)
class DStar {
private:
    struct State {
        double g;
        double rhs;
        bool visited;
        
        State() : g(std::numeric_limits<double>::infinity()), 
                  rhs(std::numeric_limits<double>::infinity()), 
                  visited(false) {}
    };
    
public:
    static PathResult search(const Graph& graph, int startId, int goalId,
                            Heuristic heuristic = heuristics::euclidean);
    
    // Update graph with new obstacles (edge removals)
    static PathResult update(const Graph& graph, int startId, int goalId,
                            const std::vector<std::pair<int, int>>& removedEdges,
                            Heuristic heuristic = heuristics::euclidean);
};

// 3.2 D* Lite
class DStarLite {
public:
    static PathResult search(const Graph& graph, int startId, int goalId,
                            Heuristic heuristic = heuristics::euclidean);
    
    // Update for dynamic environment
    static PathResult update(const Graph& graph, int startId, int goalId,
                            const std::vector<std::pair<int, int>>& changedEdges,
                            Heuristic heuristic = heuristics::euclidean);
};

// 3.3 LPA* (Lifelong Planning A*)
class LPAStar {
private:
    struct State {
        double g;
        double rhs;
        double h;
        
        State() : g(std::numeric_limits<double>::infinity()),
                  rhs(std::numeric_limits<double>::infinity()),
                  h(0.0) {}
    };
    
public:
    static PathResult search(const Graph& graph, int startId, int goalId,
                            Heuristic heuristic = heuristics::euclidean);
    
    // Update for dynamic changes
    static PathResult update(const Graph& graph, int startId, int goalId,
                            const std::vector<int>& changedNodes,
                            Heuristic heuristic = heuristics::euclidean);
};

} // namespace sf
