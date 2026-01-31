#pragma once

#include "graph.h"
#include <queue>
#include <stack>

namespace sf {

// 1.1 Breadth-First Search (BFS)
class BFS {
public:
    static PathResult search(const Graph& graph, int startId, int goalId);
};

// 1.2 Depth-First Search (DFS)
class DFS {
public:
    static PathResult search(const Graph& graph, int startId, int goalId);
};

// 1.3 Dijkstra's Algorithm
class Dijkstra {
public:
    static PathResult search(const Graph& graph, int startId, int goalId);
};

} // namespace sf
