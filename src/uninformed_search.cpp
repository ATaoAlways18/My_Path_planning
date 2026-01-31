#include "sf/uninformed_search.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>

namespace sf {

// 1.1 Breadth-First Search (BFS)
PathResult BFS::search(const Graph& graph, int startId, int goalId) {
    if (!graph.hasNode(startId) || !graph.hasNode(goalId)) {
        return PathResult();
    }
    
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;
    std::queue<int> queue;
    
    queue.push(startId);
    visited.insert(startId);
    parent[startId] = -1;
    
    size_t nodesExplored = 0;
    
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
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
                visited.insert(edge.to);
                parent[edge.to] = current;
                queue.push(edge.to);
            }
        }
    }
    
    return PathResult();
}

// 1.2 Depth-First Search (DFS)
PathResult DFS::search(const Graph& graph, int startId, int goalId) {
    if (!graph.hasNode(startId) || !graph.hasNode(goalId)) {
        return PathResult();
    }
    
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;
    std::stack<int> stack;
    
    stack.push(startId);
    visited.insert(startId);
    parent[startId] = -1;
    
    size_t nodesExplored = 0;
    
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
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
                visited.insert(edge.to);
                parent[edge.to] = current;
                stack.push(edge.to);
            }
        }
    }
    
    return PathResult();
}

// 1.3 Dijkstra's Algorithm
PathResult Dijkstra::search(const Graph& graph, int startId, int goalId) {
    if (!graph.hasNode(startId) || !graph.hasNode(goalId)) {
        return PathResult();
    }
    
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;
    std::unordered_set<int> visited;
    
    // Priority queue: (distance, node)
    using Pair = std::pair<double, int>;
    auto cmp = [](const Pair& a, const Pair& b) { return a.first > b.first; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> pq(cmp);
    
    // Initialize
    for (const auto& pair : graph.getNodes()) {
        dist[pair.first] = std::numeric_limits<double>::infinity();
    }
    dist[startId] = 0.0;
    parent[startId] = -1;
    pq.push({0.0, startId});
    
    size_t nodesExplored = 0;
    
    while (!pq.empty()) {
        auto [d, current] = pq.top();
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
            return PathResult(path, dist[goalId], nodesExplored);
        }
        
        const auto& neighbors = graph.getNeighbors(current);
        for (const auto& edge : neighbors) {
            if (visited.find(edge.to) != visited.end()) {
                continue;
            }
            
            double newDist = dist[current] + edge.weight;
            if (newDist < dist[edge.to]) {
                dist[edge.to] = newDist;
                parent[edge.to] = current;
                pq.push({newDist, edge.to});
            }
        }
    }
    
    return PathResult();
}

} // namespace sf
