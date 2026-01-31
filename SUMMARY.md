# Path Planning Library - Summary

## Overview

This is a comprehensive C++ library implementing 12 path planning algorithms commonly used in robotics, AI, and game development. The library is organized into 4 categories and provides a clean, object-oriented API with header-only convenience.

## What's Included

### 1. Uninformed Search (3 algorithms)
- **BFS (Breadth-First Search)**: Guarantees shortest path in unweighted graphs
- **DFS (Depth-First Search)**: Memory-efficient but not optimal
- **Dijkstra's Algorithm**: Optimal path finding in weighted graphs

### 2. Heuristic Search (3 algorithms)
- **Best-First Search**: Uses heuristic to guide search
- **A* Algorithm**: Optimal and efficient path finding with heuristics
- **IDA* (Iterative Deepening A*)**: Memory-efficient variant of A*

### 3. Dynamic Environment Planning (3 algorithms)
- **D* (Dynamic A*)**: Handles dynamic obstacles and graph changes
- **D* Lite**: More efficient variant of D*
- **LPA* (Lifelong Planning A*)**: Incremental path planning

### 4. Sampling-Based Planning (2 algorithms)
- **PRM (Probabilistic Roadmap)**: Offline roadmap construction
- **RRT (Rapidly-exploring Random Tree)**: Online tree-based planning
- **RRT***: Optimal variant with rewiring (included in RRT class)

## Architecture

### File Structure
```
sf/
├── CMakeLists.txt          # Build configuration
├── README.md               # Overview and features
├── USAGE.md                # Usage examples and API reference
├── INSTALL.md              # Installation guide
├── SUMMARY.md              # This file
├── include/sf/             # Public headers
│   ├── path_planning.h     # Main header (single include)
│   ├── graph.h             # Graph data structures
│   ├── uninformed_search.h
│   ├── informed_search.h
│   ├── dynamic_search.h
│   └── sampling_based.h
├── src/                    # Implementation files
│   ├── graph.cpp
│   ├── uninformed_search.cpp
│   ├── informed_search.cpp
│   ├── dynamic_search.cpp
│   └── sampling_based.cpp
├── tests/                  # Test suite
│   └── test.cpp
└── examples/               # Usage examples
    └── simple_example.cpp
```

### Core Components

#### Graph Structure
```cpp
struct Node {
    int id;
    double x, y;
};

struct Edge {
    int from, to;
    double weight;
};

class Graph {
    void addNode(const Node& node);
    void addEdge(int from, int to, double weight);
    void addEdgeUndirected(int from, int to, double weight);
    // ... more methods
};
```

#### Path Result
```cpp
struct PathResult {
    bool found;              // Path found?
    std::vector<int> path;   // Node sequence
    double cost;             // Total cost
    size_t nodesExplored;    // Search statistics
};
```

## Usage Examples

### Basic Graph Search
```cpp
#include "sf/path_planning.h"

sf::Graph graph;
graph.addNode(sf::Node(0, 0.0, 0.0));
graph.addNode(sf::Node(1, 1.0, 0.0));
graph.addEdgeUndirected(0, 1, 1.0);

auto result = sf::AStar::search(graph, 0, 1);
if (result.found) {
    // Path found!
}
```

### Continuous Space Planning
```cpp
#include "sf/path_planning.h"

sf::RRT rrt(10000, 0.1, 42);
auto result = rrt.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
```

### With Collision Detection
```cpp
sf::RRT rrt(10000, 0.1, 42);
rrt.setCollisionChecker([](double x, double y) {
    // Return true if (x,y) is in collision
    return false;
});
```

## Key Features

### 1. Clean API Design
- Single header include: `#include "sf/path_planning.h"`
- Consistent interface across all algorithms
- RAII-compliant resource management
- No external dependencies (STL only)

### 2. Algorithm Flexibility
- Multiple algorithms for different use cases
- Configurable heuristics (Euclidean, Manhattan, Zero)
- Customizable sampling parameters for RRT/PRM
- Collision detection callbacks

### 3. Performance
- Optimized data structures (unordered_map, priority_queue)
- Efficient memory usage
- Early termination when goal found
- Statistics tracking (nodes explored, cost)

### 4. Standards Compliance
- C++17 standard
- Modern C++ practices
- Type-safe interfaces
- Const-correctness

## Build System

### CMake Configuration
```cmake
cmake_minimum_required(VERSION 3.14)
project(PathPlanningLib VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build library
add_library(path_planning STATIC src/*.cpp)

# Include headers
target_include_directories(path_planning PUBLIC include)
```

### Build Commands
```bash
mkdir build && cd build
cmake ..
make
```

## Testing

### Test Suite
- Tests all 12 algorithms
- Validates correctness and performance
- Simple grid and continuous space scenarios
- Run with: `./path_planning_test`

### Example Program
- Demonstrates practical usage
- Shows multiple algorithms
- Run with: `./simple_example`

## Algorithm Comparison

| Algorithm | Optimality | Completeness | Memory | Time | Dynamic |
|-----------|------------|--------------|--------|------|---------|
| BFS | Yes (unweighted) | Yes | O(b^d) | O(b^d) | No |
| DFS | No | Yes | O(bm) | O(b^m) | No |
| Dijkstra | Yes | Yes | O(V) | O(E+V log V) | No |
| Best-First | No | Yes | O(b^d) | O(b^d) | No |
| A* | Yes | Yes | O(b^d) | O(b^d) | No |
| IDA* | Yes | Yes | O(d) | O(b^d) | No |
| D* | Yes | Yes | O(E) | O(E log V) | Yes |
| D* Lite | Yes | Yes | O(E) | O(E log V) | Yes |
| LPA* | Yes | Yes | O(E) | O(E log V) | Yes |
| PRM | Probabilistic | Probabilistic | O(n) | O(n log n) | No |
| RRT | Probabilistic | Probabilistic | O(n) | O(n) | No |
| RRT* | Probabilistic | Probabilistic | O(n) | O(n log n) | No |

## Use Cases

### Robotics
- Mobile robot navigation (A*, D*)
- Manipulator planning (RRT, RRT*)
- Multi-robot coordination (PRM)

### Game Development
- NPC pathfinding (A*, BFS)
- Procedural content generation (RRT)
- Real-time strategy (D* Lite)

### AI & Machine Learning
- Motion planning (RRT, RRT*)
- State space exploration (PRM)
- Reinforcement learning environments

### Autonomous Vehicles
- Global planning (A*, Dijkstra)
- Local planning (RRT)
- Dynamic obstacle avoidance (D* Lite)

## Performance Characteristics

### Memory Usage
- **Low**: DFS, IDA*, RRT
- **Medium**: BFS, Dijkstra, A*, RRT*
- **High**: PRM (preprocessing)

### Speed
- **Fast**: DFS, Best-First, RRT
- **Medium**: BFS, A*, Dijkstra
- **Slow**: IDA*, PRM (preprocessing)

### Optimality
- **Optimal**: BFS, Dijkstra, A*, IDA*, D*, D* Lite, LPA*
- **Probabilistic Optimal**: RRT*, PRM
- **Non-optimal**: DFS, Best-First, RRT

## Extensibility

### Adding Custom Heuristics
```cpp
auto customHeuristic = [](const sf::Node& a, const sf::Node& b) {
    return /* custom distance */;
};
sf::AStar::search(graph, start, goal, customHeuristic);
```

### Adding Custom Collision Checkers
```cpp
auto customChecker = [](double x, double y) {
    // Your collision logic
    return false;
};
rrt.setCollisionChecker(customChecker);
```

### Extending Graph Structure
The graph class can be extended with additional node/edge attributes as needed.

## Limitations

1. **Graph-based algorithms**: Require discrete graph representation
2. **Sampling algorithms**: Probabilistic, not guaranteed to find path
3. **Dynamic algorithms**: Simplified implementation (full D*/LPA* are complex)
4. **High dimensions**: Performance degrades in very high-dimensional spaces

## Future Enhancements

Potential additions:
- RRT-Connect for bidirectional search
- PRM* (optimal variant)
- Lazy PRM
- Multi-query planning
- Parallel implementations
- GPU acceleration for sampling

## License

MIT License - Free for commercial and personal use.

## Credits

Based on standard implementations of:
- A* (Hart, Nilsson, Raphael, 1968)
- RRT (LaValle, 1998)
- D* (Stentz, 1994)
- PRM (Kavraki et al., 1996)

## Getting Started

1. Read INSTALL.md for build instructions
2. Check USAGE.md for API reference
3. Run examples/simple_example.cpp
4. Explore tests/test.cpp for more examples

## Support

For questions or issues:
- Review documentation files
- Check test examples
- Examine source code comments
- Consult algorithm references
