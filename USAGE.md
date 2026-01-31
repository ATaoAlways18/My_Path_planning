# Path Planning Library Usage Guide

## Quick Start

### 1. Build the Library

```bash
cd ct/code/sf
mkdir build && cd build
cmake ..
make
```

### 2. Run Tests

```bash
./path_planning_test
```

### 3. Run Example

```bash
./simple_example
```

## Integration into Your Project

### Method 1: Using CMake (Recommended)

Add to your project's `CMakeLists.txt`:

```cmake
# Add the path planning library
add_subdirectory(path/to/sf)

# Link against your executable
target_link_libraries(your_target path_planning)
```

### Method 2: Manual Compilation

Compile the library and link manually:

```bash
# Compile library
g++ -std=c++17 -I/path/to/sf/include -c src/*.cpp

# Create static library
ar rcs libpath_planning.a *.o

# Link with your program
g++ -std=c++17 -I/path/to/sf/include your_program.cpp -L. -lpath_planning -o your_program
```

## Basic Usage Examples

### Example 1: Graph Search with A*

```cpp
#include "sf/path_planning.h"
#include <iostream>

int main() {
    // Create graph
    sf::Graph graph;
    
    // Add nodes (id, x, y)
    graph.addNode(sf::Node(0, 0.0, 0.0));
    graph.addNode(sf::Node(1, 1.0, 0.0));
    graph.addNode(sf::Node(2, 2.0, 0.0));
    
    // Add edges (from, to, weight)
    graph.addEdge(0, 1, 1.0);
    graph.addEdge(1, 2, 1.0);
    
    // Find path
    auto result = sf::AStar::search(graph, 0, 2);
    
    if (result.found) {
        std::cout << "Path: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost << "\n";
    }
    
    return 0;
}
```

### Example 2: Grid Path Planning

```cpp
#include "sf/path_planning.h"

int main() {
    sf::Graph graph;
    
    // Create 5x5 grid
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            int id = y * 5 + x;
            graph.addNode(sf::Node(id, x, y));
        }
    }
    
    // Connect adjacent nodes
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            int id = y * 5 + x;
            
            // Right neighbor
            if (x < 4) {
                graph.addEdgeUndirected(id, id + 1, 1.0);
            }
            
            // Down neighbor
            if (y < 4) {
                graph.addEdgeUndirected(id, id + 5, 1.0);
            }
        }
    }
    
    // Find path from top-left to bottom-right
    auto result = sf::AStar::search(graph, 0, 24);
    
    if (result.found) {
        std::cout << "Path found with cost: " << result.cost << "\n";
    }
    
    return 0;
}
```

### Example 3: Continuous Space Planning with RRT

```cpp
#include "sf/path_planning.h"

int main() {
    sf::RRT rrt(10000, 0.1, 42);
    
    // Optional: Set collision detection function
    rrt.setCollisionChecker([](double x, double y) {
        // Example: circular obstacle at (5, 5) with radius 1
        double dx = x - 5.0;
        double dy = y - 5.0;
        return (dx * dx + dy * dy) < 1.0;
    });
    
    // Find path from (0, 0) to (10, 10) in workspace [0,10]x[0,10]
    auto result = rrt.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    if (result.found) {
        std::cout << "Path found with " << result.path.size() << " waypoints\n";
        std::cout << "Total cost: " << result.cost << "\n";
    }
    
    return 0;
}
```

### Example 4: Probabilistic Roadmap (PRM)

```cpp
#include "sf/path_planning.h"

int main() {
    sf::PRM prm(2000, 2.0, 42);
    
    // Optional: Set collision detection
    prm.setCollisionChecker([](double x, double y) {
        // Check if point is in collision
        return false;
    });
    
    // Build roadmap and find path
    auto result = prm.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    if (result.found) {
        std::cout << "Path found with " << result.path.size() << " waypoints\n";
    }
    
    return 0;
}
```

### Example 5: Using Different Heuristics

```cpp
#include "sf/path_planning.h"

int main() {
    sf::Graph graph;
    
    // ... build graph ...
    
    // Euclidean distance (default)
    auto result1 = sf::AStar::search(graph, start, goal, sf::heuristics::euclidean);
    
    // Manhattan distance
    auto result2 = sf::AStar::search(graph, start, goal, sf::heuristics::manhattan);
    
    // Zero heuristic (equivalent to Dijkstra)
    auto result3 = sf::AStar::search(graph, start, goal, sf::heuristics::zero);
    
    return 0;
}
```

## API Reference

### Graph Class

```cpp
// Add a node
void addNode(const Node& node);

// Add a directed edge
void addEdge(int from, int to, double weight);

// Add an undirected edge (bidirectional)
void addEdgeUndirected(int from, int to, double weight);

// Get node by ID
const Node& getNode(int id) const;

// Get neighbors of a node
const std::vector<Edge>& getNeighbors(int id) const;

// Check if node exists
bool hasNode(int id) const;

// Get number of nodes
size_t nodeCount() const;

// Get number of edges
size_t edgeCount() const;

// Get all node IDs
std::vector<int> getNodeIds() const;

// Clear graph
void clear();
```

### PathResult Structure

```cpp
struct PathResult {
    bool found;              // Whether a path was found
    std::vector<int> path;   // Sequence of node IDs
    double cost;             // Total path cost
    size_t nodesExplored;    // Number of nodes explored during search
};
```

### Search Algorithms

```cpp
// Uninformed Search
PathResult BFS::search(const Graph& graph, int startId, int goalId);
PathResult DFS::search(const Graph& graph, int startId, int goalId);
PathResult Dijkstra::search(const Graph& graph, int startId, int goalId);

// Heuristic Search
PathResult BestFirstSearch::search(const Graph& graph, int startId, int goalId,
                                   Heuristic heuristic = heuristics::euclidean);
PathResult AStar::search(const Graph& graph, int startId, int goalId,
                         Heuristic heuristic = heuristics::euclidean);
PathResult IDAStar::search(const Graph& graph, int startId, int goalId,
                           Heuristic heuristic = heuristics::euclidean);

// Dynamic Search
PathResult DStar::search(const Graph& graph, int startId, int goalId,
                         Heuristic heuristic = heuristics::euclidean);
PathResult DStarLite::search(const Graph& graph, int startId, int goalId,
                             Heuristic heuristic = heuristics::euclidean);
PathResult LPAStar::search(const Graph& graph, int startId, int goalId,
                           Heuristic heuristic = heuristics::euclidean);

// Sampling-Based Search
PathResult RRT::search(double startX, double startY, double goalX, double goalY,
                       double minX, double maxX, double minY, double maxY);
PathResult RRT::searchStar(double startX, double startY, double goalX, double goalY,
                           double minX, double maxX, double minY, double maxY,
                           double radius = 1.0);
PathResult PRM::search(double startX, double startY, double goalX, double goalY,
                       double minX, double maxX, double minY, double maxY);
```

### Heuristics

```cpp
// Euclidean distance (straight-line distance)
double heuristics::euclidean(const Node& a, const Node& b);

// Manhattan distance (L1 norm)
double heuristics::manhattan(const Node& a, const Node& b);

// Zero heuristic (for uninformed search)
double heuristics::zero(const Node& a, const Node& b);
```

### Collision Checker (for RRT/PRM)

```cpp
// Define a collision checker function
auto collisionChecker = [](double x, double y) -> bool {
    // Return true if (x, y) is in collision
    // Example: check against obstacles
    return false;
};

// Set it for RRT
rrt.setCollisionChecker(collisionChecker);

// Set it for PRM
prm.setCollisionChecker(collisionChecker);
```

## Algorithm Selection Guide

| Algorithm | Use Case | Pros | Cons |
|-----------|----------|------|------|
| **BFS** | Unweighted graphs, shortest path | Optimal, complete | High memory usage |
| **DFS** | Memory-constrained scenarios | Low memory | Not optimal, may be slow |
| **Dijkstra** | Weighted graphs, optimal path | Optimal, complete | Slower than A* |
| **A*** | General path planning | Optimal with good heuristic | Memory intensive |
| **Best-First** | Fast but non-optimal | Fast | Not optimal |
| **IDA*** | Memory-constrained A* | Low memory | May be slower |
| **D*/D* Lite** | Dynamic environments | Incremental updates | Complex implementation |
| **LPA*** | Lifelong planning | Incremental | Complex |
| **RRT** | High-dimensional spaces | Fast, probabilistic | Not optimal |
| **RRT*** | High-dimensional spaces | Optimal (probabilistic) | Slower than RRT |
| **PRM** | Multiple queries | Good for many queries | Offline preprocessing |

## Performance Tips

1. **Choose the right algorithm**: Use A* for general cases, RRT for high-dimensional spaces
2. **Good heuristic**: For A*, use Euclidean distance for 2D/3D spaces
3. **Graph density**: Sparse graphs work better with A*, dense graphs with RRT
4. **Sampling**: For RRT/PRM, adjust sample count based on space complexity
5. **Collision checking**: Keep collision checks efficient for real-time applications

## Troubleshooting

### No path found
- Check if start/goal nodes exist
- Verify graph connectivity
- For RRT/PRM, increase sample count or adjust step size

### Slow performance
- Use A* instead of Dijkstra for heuristic-guided search
- For RRT, reduce max iterations or increase step size
- For PRM, reduce sample count

### Memory issues
- Use IDA* instead of A* for memory-constrained scenarios
- For large graphs, consider using RRT instead of graph-based methods

## License

MIT License - Free to use in commercial and personal projects.
