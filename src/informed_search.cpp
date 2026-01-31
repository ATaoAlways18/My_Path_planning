#include "sf/informed_search.h"
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

namespace sf {

// 2.1 Best-First Search (Heuristic)
PathResult BestFirstSearch::search(const Graph& graph, int startId, int goalId,
                                   Heuristic heuristic) {
    if (!graph.hasNode(startId) || !graph.hasNode(goalId)) {
        return PathResult();
    }
    
    std::unordered_map<int, double> hScore;
    std::unordered_map<int, int> parent;
    std::unordered_set<int> visited;
    
    // Priority queue: (heuristic, node)
    using Pair = std::pair<double, int>;
    auto cmp = [](const Pair& a, const Pair& b) { return a.first > b.first; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> pq(cmp);
    
    // Initialize heuristic scores
    for (const auto& pair : graph.getNodes()) {
        hScore[pair.first] = heuristic(pair.second, graph.getNode(goalId));
    }
    
    parent[startId] = -1;
    pq.push({hScore[startId], startId});
    
    size_t nodesExplored = 0;
    
    while (!pq.empty()) {
        auto [h, current] = pq.top();
        pq.pop();
        
        if (visited.find(current) != visited.end()) {
            continue;
        }
        visited.insert(current);
        nodesExplored++;
        
        if (current == goalId) {
            // Reconstruct path
            std::vector<int> path;
            int node = goalId;
            while (node != -1) {
                path.push_back(node);
                node = parent[node];
            }
            std::reverse(path.begin(), path.end());
            
            // Calculate cost
            double cost = 0.0;
            for (size_t i = 1; i < path.size(); i++) {
                const auto& neighbors = graph.getNeighbors(path[i-1]);
                for (const auto& edge : neighbors) {
                    if (edge.to == path[i]) {
                        cost += edge.weight;
                        break;
                    }
                }
            }
            
            return PathResult(path, cost, nodesExplored);
        }
        
        const auto& neighbors = graph.getNeighbors(current);
        for (const auto& edge : neighbors) {
            if (visited.find(edge.to) == visited.end()) {
                parent[edge.to] = current;
                pq.push({hScore[edge.to], edge.to});
            }
        }
    }
    
    return PathResult();
}

// 2.2 A* Algorithm
PathResult AStar::search(const Graph& graph, int startId, int goalId,
                         Heuristic heuristic) {
    if (!graph.hasNode(startId) || !graph.hasNode(goalId)) {
        return PathResult();
    }
    
    std::unordered_map<int, double> gScore;
    std::unordered_map<int, double> fScore;
    std::unordered_map<int, int> parent;
    std::unordered_set<int> closedSet;
    
    // Priority queue: (fScore, node)
    using Pair = std::pair<double, int>;
    auto cmp = [](const Pair& a, const Pair& b) { return a.first > b.first; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> pq(cmp);
    
    // Initialize
    for (const auto& pair : graph.getNodes()) {
        gScore[pair.first] = std::numeric_limits<double>::infinity();
        fScore[pair.first] = std::numeric_limits<double>::infinity();
    }
    
    gScore[startId] = 0.0;
    fScore[startId] = heuristic(graph.getNode(startId), graph.getNode(goalId));
    parent[startId] = -1;
    pq.push({fScore[startId], startId});
    
    size_t nodesExplored = 0;
    
    while (!pq.empty()) {
        auto [f, current] = pq.top();
        pq.pop();
        
        if (closedSet.find(current) != closedSet.end()) {
            continue;
        }
        closedSet.insert(current);
        nodesExplored++;
        
        if (current == goalId) {
            // Reconstruct path
            std::vector<int> path;
            int node = goalId;
            while (node != -1) {
                path.push_back(node);
                node = parent[node];
            }
            std::reverse(path.begin(), path.end());
            return PathResult(path, gScore[goalId], nodesExplored);
        }
        
        const auto& neighbors = graph.getNeighbors(current);
        for (const auto& edge : neighbors) {
            if (closedSet.find(edge.to) != closedSet.end()) {
                continue;
            }
            
            double tentativeG = gScore[current] + edge.weight;
            if (tentativeG < gScore[edge.to]) {
                parent[edge.to] = current;
                gScore[edge.to] = tentativeG;
                fScore[edge.to] = tentativeG + heuristic(graph.getNode(edge.to), 
                                                        graph.getNode(goalId));
                pq.push({fScore[edge.to], edge.to});
            }
        }
    }
    
    return PathResult();
}

// 2.3 IDA* (Iterative Deepening A*)
namespace idastar_internal {
    struct IDAStarNode {
        int id;
        double g;
        double f;
        
        IDAStarNode(int i, double gVal, double fVal) : id(i), g(gVal), f(fVal) {}
    };
    
    PathResult searchRecursive(const Graph& graph, int current, int goalId,
                              double g, double bound, Heuristic heuristic,
                              std::unordered_map<int, int>& parent,
                              std::unordered_set<int>& visited,
                              size_t& nodesExplored) {
        nodesExplored++;
        
        double h = heuristic(graph.getNode(current), graph.getNode(goalId));
        double f = g + h;
        
        if (f > bound) {
            return PathResult();
        }
        
        if (current == goalId) {
            return PathResult(std::vector<int>(), g, nodesExplored);
        }
        
        double minBound = std::numeric_limits<double>::infinity();
        
        const auto& neighbors = graph.getNeighbors(current);
        for (const auto& edge : neighbors) {
            if (visited.find(edge.to) != visited.end()) {
                continue;
            }
            
            visited.insert(edge.to);
            parent[edge.to] = current;
            
            auto result = searchRecursive(graph, edge.to, goalId, g + edge.weight,
                                         bound, heuristic, parent, visited, nodesExplored);
            
            if (result.found) {
                return result;
            }
            
            if (result.cost > 0 && result.cost < minBound) {
                minBound = result.cost;
            }
            
            visited.erase(edge.to);
        }
        
        return PathResult();
    }
}

PathResult IDAStar::search(const Graph& graph, int startId, int goalId,
                           Heuristic heuristic) {
    if (!graph.hasNode(startId) || !graph.hasNode(goalId)) {
        return PathResult();
    }
    
    double bound = heuristic(graph.getNode(startId), graph.getNode(goalId));
    size_t nodesExplored = 0;
    
    while (true) {
        std::unordered_map<int, int> parent;
        std::unordered_set<int> visited;
        visited.insert(startId);
        parent[startId] = -1;
        
        auto result = idastar_internal::searchRecursive(
            graph, startId, goalId, 0.0, bound, heuristic, parent, visited, nodesExplored);
        
        if (result.found) {
            // Reconstruct path
            std::vector<int> path;
            int node = goalId;
            while (node != -1) {
                path.push_back(node);
                node = parent[node];
            }
            std::reverse(path.begin(), path.end());
            return PathResult(path, result.cost, nodesExplored);
        }
        
        if (result.cost == std::numeric_limits<double>::infinity()) {
            return PathResult();
        }
        
        bound = result.cost;
    }
}

} // namespace sf
