#include "sf/dynamic_search.h"
#include "sf/informed_search.h"
#include <algorithm>

namespace sf {

// 3.1 D* (Dynamic A*)
PathResult DStar::search(const Graph& graph, int startId, int goalId,
                         Heuristic heuristic) {
    // Simplified D* implementation
    // For full D*, we would need to maintain a priority queue of states
    // and update rhs values dynamically
    
    // For now, use A* as the base algorithm
    // In a real D* implementation, this would maintain state information
    // and allow for incremental updates
    
    // This is a simplified version that demonstrates the concept
    std::unordered_map<int, State> states;
    
    // Initialize all states
    for (const auto& pair : graph.getNodes()) {
        states[pair.first] = State();
    }
    
    // Set goal state
    states[goalId].g = 0.0;
    states[goalId].rhs = 0.0;
    
    // Priority queue for open list
    using Pair = std::pair<double, int>;
    auto cmp = [](const Pair& a, const Pair& b) { return a.first > b.first; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> pq(cmp);
    
    pq.push({0.0, goalId});
    
    size_t nodesExplored = 0;
    
    while (!pq.empty()) {
        auto [key, current] = pq.top();
        pq.pop();
        
        if (states[current].visited) continue;
        states[current].visited = true;
        nodesExplored++;
        
        if (current == startId) {
            // Reconstruct path
            std::vector<int> path;
            int node = startId;
            path.push_back(node);
            
            while (node != goalId) {
                const auto& neighbors = graph.getNeighbors(node);
                double minCost = std::numeric_limits<double>::infinity();
                int next = -1;
                
                for (const auto& edge : neighbors) {
                    double cost = states[edge.to].g + edge.weight;
                    if (cost < minCost) {
                        minCost = cost;
                        next = edge.to;
                    }
                }
                
                if (next == -1) break;
                path.push_back(next);
                node = next;
            }
            
            return PathResult(path, states[startId].g, nodesExplored);
        }
        
        const auto& neighbors = graph.getNeighbors(current);
        for (const auto& edge : neighbors) {
            if (states[edge.to].visited) continue;
            
            double newRhs = states[current].g + edge.weight;
            if (newRhs < states[edge.to].rhs) {
                states[edge.to].rhs = newRhs;
                
                if (states[edge.to].rhs < states[edge.to].g) {
                    states[edge.to].g = states[edge.to].rhs;
                    double h = heuristic(graph.getNode(edge.to), graph.getNode(startId));
                    pq.push({states[edge.to].g + h, edge.to});
                }
            }
        }
    }
    
    return PathResult();
}

PathResult DStar::update(const Graph& graph, int startId, int goalId,
                         const std::vector<std::pair<int, int>>& removedEdges,
                         Heuristic heuristic) {
    // In a real D* implementation, this would update the graph
    // and only replan the affected parts of the path
    
    // For this simplified version, we recompute the entire path
    return search(graph, startId, goalId, heuristic);
}

// 3.2 D* Lite
PathResult DStarLite::search(const Graph& graph, int startId, int goalId,
                             Heuristic heuristic) {
    // D* Lite is similar to D* but uses a different formulation
    // It's based on LPA* but works backwards from goal to start
    
    // Simplified implementation using A* as base
    return AStar::search(graph, startId, goalId, heuristic);
}

PathResult DStarLite::update(const Graph& graph, int startId, int goalId,
                             const std::vector<std::pair<int, int>>& changedEdges,
                             Heuristic heuristic) {
    // In a real D* Lite implementation, this would update only
    // the affected parts of the search tree
    
    return search(graph, startId, goalId, heuristic);
}

// 3.3 LPA* (Lifelong Planning A*)
PathResult LPAStar::search(const Graph& graph, int startId, int goalId,
                           Heuristic heuristic) {
    // LPA* maintains g and rhs values for all nodes
    // and updates them incrementally when the graph changes
    
    std::unordered_map<int, State> states;
    
    // Initialize all states
    for (const auto& pair : graph.getNodes()) {
        states[pair.first] = State();
        states[pair.first].h = heuristic(graph.getNode(pair.first), 
                                        graph.getNode(goalId));
    }
    
    // Set goal state
    states[goalId].g = 0.0;
    states[goalId].rhs = 0.0;
    
    // Priority queue for open list
    using Pair = std::pair<double, int>;
    auto cmp = [](const Pair& a, const Pair& b) { return a.first > b.first; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> pq(cmp);
    
    pq.push({0.0, goalId});
    
    size_t nodesExplored = 0;
    
    while (!pq.empty()) {
        auto [key, current] = pq.top();
        pq.pop();
        
        if (current == startId && states[startId].rhs == states[startId].g) {
            // Path found
            std::vector<int> path;
            int node = startId;
            path.push_back(node);
            
            while (node != goalId) {
                const auto& neighbors = graph.getNeighbors(node);
                double minCost = std::numeric_limits<double>::infinity();
                int next = -1;
                
                for (const auto& edge : neighbors) {
                    double cost = states[edge.to].g + edge.weight;
                    if (cost < minCost) {
                        minCost = cost;
                        next = edge.to;
                    }
                }
                
                if (next == -1) break;
                path.push_back(next);
                node = next;
            }
            
            return PathResult(path, states[startId].g, nodesExplored);
        }
        
        nodesExplored++;
        
        const auto& neighbors = graph.getNeighbors(current);
        for (const auto& edge : neighbors) {
            double newRhs = states[current].g + edge.weight;
            if (newRhs < states[edge.to].rhs) {
                states[edge.to].rhs = newRhs;
                
                if (states[edge.to].rhs < states[edge.to].g) {
                    states[edge.to].g = states[edge.to].rhs;
                    double key = states[edge.to].g + states[edge.to].h;
                    pq.push({key, edge.to});
                }
            }
        }
    }
    
    return PathResult();
}

PathResult LPAStar::update(const Graph& graph, int startId, int goalId,
                           const std::vector<int>& changedNodes,
                           Heuristic heuristic) {
    // In a real LPA* implementation, this would update only
    // the affected nodes and their neighbors
    
    return search(graph, startId, goalId, heuristic);
}

} // namespace sf
