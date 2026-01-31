# Quick Reference Card

## Installation

```bash
cd ct/code/sf
mkdir build && cd build
cmake ..
make
```

## Single Include

```cpp
#include "sf/path_planning.h"
```

## Common Patterns

### 1. Graph Search (A* - Recommended)

```cpp
sf::Graph graph;
// Add nodes
graph.addNode(sf::Node(0, x, y));
// Add edges
graph.addEdgeUndirected(0, 1, weight);
// Search
auto result = sf::AStar::search(graph, startId, goalId);
if (result.found) {
    // result.path contains node IDs
    // result.cost contains total cost
}
```

### 2. Grid Pathfinding

```cpp
// Create grid
for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        int id = y * width + x;
        graph.addNode(sf::Node(id, x, y));
    }
}

// Connect neighbors
for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        int id = y * width + x;
        if (x < width - 1) graph.addEdgeUndirected(id, id + 1, 1.0);
        if (y < height - 1) graph.addEdgeUndirected(id, id + width, 1.0);
    }
}
```

### 3. Continuous Space (RRT)

```cpp
sf::RRT rrt(10000, 0.1, 42);  // maxIter, stepSize, seed

// Optional: Add collision detection
rrt.setCollisionChecker([](double x, double y) {
    // Return true if in collision
    return false;
});

auto result = rrt.search(0, 0, 10, 10, 0, 10, 0, 10);
// result.path contains waypoints (node IDs)
```

### 4. Probabilistic Roadmap (PRM)

```cpp
sf::PRM prm(1000, 2.0, 42);  // samples, radius, seed

// Optional: Add collision detection
prm.setCollisionChecker([](double x, double y) {
    return false;
});

auto result = prm.search(0, 0, 10, 10, 0, 10, 0, 10);
```

## Algorithm Quick Reference

| Algorithm | When to Use | Code |
|-----------|-------------|------|
| **BFS** | Unweighted graphs, shortest path | `sf::BFS::search(graph, s, g)` |
| **DFS** | Memory-constrained, not optimal | `sf::DFS::search(graph, s, g)` |
| **Dijkstra** | Weighted graphs, optimal | `sf::Dijkstra::search(graph, s, g)` |
| **A*** | General purpose, optimal | `sf::AStar::search(graph, s, g)` |
| **Best-First** | Fast but not optimal | `sf::BestFirstSearch::search(graph, s, g)` |
| **IDA*** | Memory-constrained A* | `sf::IDAStar::search(graph, s, g)` |
| **RRT** | High-dimensional spaces | `rrt.search(x1, y1, x2, y2, ...)` |
| **RRT*** | Optimal high-dim planning | `rrt.searchStar(x1, y1, x2, y2, ...)` |
| **PRM** | Multiple queries, offline | `prm.search(x1, y1, x2, y2, ...)` |

## Heuristics

```cpp
// Euclidean distance (default, good for 2D/3D)
sf::heuristics::euclidean(a, b)

// Manhattan distance (good for grids)
sf::heuristics::manhattan(a, b)

// Zero heuristic (uninformed)
sf::heuristics::zero(a, b)
```

## PathResult Fields

```cpp
struct PathResult {
    bool found;              // Was path found?
    std::vector<int> path;   // Node sequence
    double cost;             // Total cost
    size_t nodesExplored;    // Search statistics
};
```

## Common Issues & Solutions

### No path found
- Check if start/goal exist in graph
- Verify graph is connected
- For RRT/PRM: increase samples or step size

### Slow performance
- Use A* instead of Dijkstra
- For RRT: reduce max iterations
- For PRM: reduce sample count

### Memory issues
- Use IDA* instead of A*
- Use RRT for large spaces
- Reduce graph size

## Performance Tips

1. **A*** is usually best for graph search
2. **RRT** for continuous/high-dimensional spaces
3. **PRM** when you need multiple queries
4. **BFS** only for unweighted graphs
5. **DFS** when memory is very limited

## Example: Complete Program

```cpp
#include "sf/path_planning.h"
#include <iostream>

int main() {
    // Create graph
    sf::Graph graph;
    
    // Add 3 nodes in a line
    graph.addNode(sf::Node(0, 0.0, 0.0));
    graph.addNode(sf::Node(1, 1.0, 0.0));
    graph.addNode(sf::Node(2, 2.0, 0.0));
    
    // Connect them
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    
    // Find path
    auto result = sf::AStar::search(graph, 0, 2);
    
    if (result.found) {
        std::cout << "Path: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost << "\n";
        std::cout << "Nodes explored: " << result.nodesExplored << "\n";
    } else {
        std::cout << "No path found!\n";
    }
    
    return 0;
}
```

## Build & Run

```bash
# Build
g++ -std=c++17 -I/path/to/sf/include main.cpp -L/path/to/sf/build -lpath_planning -o main

# Run
./main
```

## Files to Include

- **Main header**: `include/sf/path_planning.h`
- **All headers**: `include/sf/*.h`
- **Library**: `build/libpath_planning.a`

## API Summary

### Graph
- `addNode(Node)` - Add node
- `addEdge(from, to, weight)` - Add directed edge
- `addEdgeUndirected(from, to, weight)` - Add bidirectional edge
- `getNode(id)` - Get node
- `getNeighbors(id)` - Get adjacent nodes
- `hasNode(id)` - Check existence
- `nodeCount()` - Number of nodes
- `edgeCount()` - Number of edges

### Search Algorithms
All search algorithms follow the pattern:
```cpp
ResultType::search(graph, startId, goalId [, heuristic])
```

### Sampling Algorithms
```cpp
Algorithm::search(startX, startY, goalX, goalY, minX, maxX, minY, maxY)
```

## License

MIT License - Free to use in any project.
