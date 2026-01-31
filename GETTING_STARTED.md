# Getting Started Guide

## Quick Start (5 Minutes)

### Step 1: Build the Library

```bash
cd ct/code/sf
mkdir build && cd build
cmake ..
make
```

### Step 2: Run the Demo

```bash
./usage_demo
```

You should see 6 demos showing different algorithms in action.

### Step 3: Run Tests

```bash
./comprehensive_test
```

All tests should pass with "ALL TESTS PASSED! ✓"

## Your First Program

Create a file `my_program.cpp`:

```cpp
#include "sf/path_planning.h"
#include <iostream>

int main() {
    // Create a graph
    sf::Graph graph;
    
    // Add 3 nodes
    graph.addNode(sf::Node(0, 0.0, 0.0));
    graph.addNode(sf::Node(1, 1.0, 0.0));
    graph.addNode(sf::Node(2, 2.0, 0.0));
    
    // Connect them
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    
    // Find path using A*
    auto result = sf::AStar::search(graph, 0, 2);
    
    if (result.found) {
        std::cout << "Path: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost << "\n";
    } else {
        std::cout << "No path found!\n";
    }
    
    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -I/home/ct/.openclaw/workspace/ct/code/sf/include my_program.cpp \
    -L/home/ct/.openclaw/workspace/ct/code/sf/build -lpath_planning -o my_program
./my_program
```

Expected output:
```
Path: 0 1 2 
Cost: 2
```

## Choose Your Algorithm

### For Graph Search (Discrete)
```cpp
// A* - Best overall choice
auto result = sf::AStar::search(graph, start, goal);

// Dijkstra - For weighted graphs
auto result = sf::Dijkstra::search(graph, start, goal);

// BFS - For unweighted graphs
auto result = sf::BFS::search(graph, start, goal);
```

### For Continuous Space
```cpp
// RRT - Fast, probabilistic
sf::RRT rrt(10000, 0.1, 42);
auto result = rrt.search(x1, y1, x2, y2, minX, maxX, minY, maxY);

// RRT* - Optimal, slower
auto result = rrt.searchStar(x1, y1, x2, y2, minX, maxX, minY, maxY);

// PRM - Good for multiple queries
sf::PRM prm(1000, 2.0, 42);
auto result = prm.search(x1, y1, x2, y2, minX, maxX, minY, maxY);
```

## Common Use Cases

### 1. Game NPC Pathfinding
```cpp
// Grid-based pathfinding
sf::Graph graph = createGridMap(width, height);
auto result = sf::AStar::search(graph, npcPos, targetPos);
```

### 2. Robot Navigation
```cpp
// Continuous space planning
sf::RRT rrt(5000, 0.2, 42);
rrt.setCollisionChecker(isInCollision);
auto result = rrt.search(robotX, robotY, goalX, goalY, 0, 10, 0, 10);
```

### 3. Multiple Queries
```cpp
// Build roadmap once, query many times
sf::PRM prm(2000, 2.0, 42);
prm.setCollisionChecker(isInCollision);

// Query 1
auto result1 = prm.search(x1, y1, x2, y2, minX, maxX, minY, maxY);

// Query 2 (reuses roadmap)
auto result2 = prm.search(x3, y3, x4, y4, minX, maxX, minY, maxY);
```

## Algorithm Selection Guide

| Scenario | Recommended Algorithm |
|----------|----------------------|
| Grid-based game | A* or BFS |
| Weighted graph | A* or Dijkstra |
| Continuous space | RRT or RRT* |
| Multiple queries | PRM |
| Memory constrained | IDA* or RRT |
| Dynamic obstacles | D* Lite |
| Need optimality | A* or RRT* |
| Need speed | RRT or Best-First |

## Understanding Results

```cpp
auto result = sf::AStar::search(graph, start, goal);

if (result.found) {
    // Path exists
    std::vector<int> path = result.path;  // Node sequence
    double cost = result.cost;            // Total cost
    size_t explored = result.nodesExplored; // Search stats
    
    // Use path
    for (int node : path) {
        // Do something with each node
    }
} else {
    // No path exists
    std::cout << "No path found!\n";
}
```

## Adding Collision Detection

```cpp
// For RRT or PRM
auto collisionChecker = [](double x, double y) -> bool {
    // Check if (x, y) is in collision
    // Example: circular obstacle
    double dx = x - obstacleX;
    double dy = y - obstacleY;
    return (dx * dx + dy * dy) < obstacleRadius;
};

sf::RRT rrt(10000, 0.1, 42);
rrt.setCollisionChecker(collisionChecker);
```

## Performance Tips

1. **Use A*** for most graph search problems
2. **Use RRT** for high-dimensional or continuous spaces
3. **Use PRM** when you need many queries on the same map
4. **Adjust sample count**: More samples = better paths but slower
5. **Adjust step size**: Larger steps = faster but less precise

## Troubleshooting

### "No path found"
- Check if start/goal exist in graph
- Verify graph is connected
- For RRT/PRM: increase samples or step size

### "Slow performance"
- Use A* instead of Dijkstra
- For RRT: reduce max iterations
- For PRM: reduce sample count

### "Memory issues"
- Use IDA* instead of A*
- Use RRT for large spaces
- Reduce graph size

## Next Steps

1. Read `USAGE.md` for detailed API reference
2. Check `examples/simple_example.cpp` for more examples
3. Read `SUMMARY.md` for algorithm details
4. Explore `tests/` for test cases

## Need Help?

- Check the documentation files in this folder
- Look at the test files for examples
- Review the source code (well-commented)
- Check algorithm references online

## Quick Reference

```cpp
// Include
#include "sf/path_planning.h"

// Graph search
auto result = sf::AStar::search(graph, start, goal);

// Continuous search
sf::RRT rrt(10000, 0.1, 42);
auto result = rrt.search(x1, y1, x2, y2, minX, maxX, minY, maxY);

// Check result
if (result.found) {
    // result.path, result.cost, result.nodesExplored
}
```

## Build & Run Summary

```bash
# Build
cd ct/code/sf
mkdir build && cd build
cmake ..
make

# Run demos
./usage_demo
./simple_example

# Run tests
./path_planning_test
./comprehensive_test

# Your program
g++ -std=c++17 -I../include your.cpp -L. -lpath_planning -o your_program
./your_program
```

---

**Ready to go!** Start with `usage_demo` to see all algorithms in action, then build your own programs using the examples as templates.
