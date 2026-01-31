#pragma once

#include "graph.h"
#include <queue>

namespace sf {

// 2.1 Best-First Search (Heuristic)
class BestFirstSearch {
public:
    static PathResult search(const Graph& graph, int startId, int goalId, 
                            Heuristic heuristic = heuristics::euclidean);
};

// 2.2 A* Algorithm
class AStar {
public:
    static PathResult search(const Graph& graph, int startId, int goalId,
                            Heuristic heuristic = heuristics::euclidean);
};

// 2.3 IDA* (Iterative Deepening A*)
class IDAStar {
public:
    static PathResult search(const Graph& graph, int startId, int goalId,
                            Heuristic heuristic = heuristics::euclidean);
};

} // namespace sf
