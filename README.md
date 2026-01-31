# Path Planning Library (sf)

A comprehensive C++ library implementing various path planning algorithms for robotics and AI applications.

## Features

### 1. Uninformed Search Algorithms
- **BFS (Breadth-First Search)**: Guarantees shortest path in unweighted graphs
- **DFS (Depth-First Search)**: Memory-efficient but not optimal
- **Dijkstra's Algorithm**: Optimal path finding in weighted graphs

### 2. Heuristic Search Algorithms
- **Best-First Search**: Uses heuristic to guide search
- **A* Algorithm**: Combines Dijkstra with heuristic for optimal and efficient path finding
- **IDA* (Iterative Deepening A*)**: Memory-efficient variant of A*

### 3. Dynamic Environment Path Planning
- **D* (Dynamic A*)**: Handles dynamic obstacles and graph changes
- **D* Lite**: More efficient variant of D*
- **LPA* (Lifelong Planning A*)**: Incremental path planning for dynamic environments

### 4. Sampling-Based Planning
- **PRM (Probabilistic Roadmap)**: Offline roadmap construction
- **RRT (Rapidly-exploring Random Tree)**: Online tree-based planning
- **RRT***: Optimal variant of RRT with rewiring

## Installation

### Using CMake

```bash
cd ct/code/sf
mkdir build && cd build
cmake ..
make
```

### Using as a Library

```bash
# Install to system
sudo make install

# Or use directly in your project
# Add to your CMakeLists.txt:
# add_subdirectory(path/to/sf)
# target_link_libraries(your_target path_planning)
```

## Usage

### Basic Example

```cpp
#include "sf/path_planning.h"

int main() {
    // Create a graph
    sf::Graph graph;
    
    // Add nodes
    graph.addNode(sf::Node(0, 0.0, 0.0));
    graph.addNode(sf::Node(1, 1.0, 0.0));
    graph.addNode(sf::Node(2, 2.0, 0.0));
    
    // Add edges
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    
    // Find path using A*
    auto result = sf::AStar::search(graph, 0, 2);
    
    if (result.found) {
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost << "\n";
    }
    
    return 0;
}
```

### Using RRT for Continuous Space

```cpp
#include "sf/path_planning.h"

int main() {
    sf::RRT rrt(10000, 0.1, 42);
    
    // Optional: Set collision checker
    rrt.setCollisionChecker([](double x, double y) {
        // Return true if (x,y) is in collision
        return false;
    });
    
    auto result = rrt.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    if (result.found) {
        std::cout << "Path found with " << result.path.size() << " waypoints\n";
    }
    
    return 0;
}
```

### Using PRM for Roadmap Construction

```cpp
#include "sf/path_planning.h"

int main() {
    sf::PRM prm(1000, 1.0, 42);
    
    // Optional: Set collision checker
    prm.setCollisionChecker([](double x, double y) {
        // Return true if (x,y) is in collision
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

## API Reference

### Graph Class

```cpp
// Add node
void addNode(const Node& node);

// Add directed edge
void addEdge(int from, int to, double weight);

// Add undirected edge
void addEdgeUndirected(int from, int to, double weight);

// Get node
const Node& getNode(int id) const;

// Get neighbors
const std::vector<Edge>& getNeighbors(int id) const;

// Check if node exists
bool hasNode(int id) const;

// Get node count
size_t nodeCount() const;
```

### PathResult Structure

```cpp
struct PathResult {
    bool found;              // Whether path was found
    std::vector<int> path;   // Sequence of node IDs
    double cost;             // Total path cost
    size_t nodesExplored;    // Number of nodes explored
};
```

### Heuristics

```cpp
// Euclidean distance (default for most algorithms)
heuristics::euclidean(a, b);

// Manhattan distance
heuristics::manhattan(a, b);

// Zero heuristic (for uninformed search)
heuristics::zero(a, b);
```

## Algorithm Comparison

| Algorithm | Optimality | Completeness | Memory | Time | Dynamic |
|-----------|------------|--------------|--------|------|---------|
| BFS | Yes (unweighted) | Yes | O(b^d) | O(b^d) | No |
| DFS | No | Yes | O(bm) | O(b^m) | No |
| Dijkstra | Yes | Yes | O(V) | O(E + V log V) | No |
| Best-First | No | Yes | O(b^d) | O(b^d) | No |
| A* | Yes | Yes | O(b^d) | O(b^d) | No |
| IDA* | Yes | Yes | O(d) | O(b^d) | No |
| D* | Yes | Yes | O(E) | O(E log V) | Yes |
| D* Lite | Yes | Yes | O(E) | O(E log V) | Yes |
| LPA* | Yes | Yes | O(E) | O(E log V) | Yes |
| PRM | Probabilistic | Probabilistic | O(n) | O(n log n) | No |
| RRT | Probabilistic | Probabilistic | O(n) | O(n) | No |
| RRT* | Probabilistic | Probabilistic | O(n) | O(n log n) | No |

## Notes

- **b**: Branching factor
- **d**: Depth of solution
- **m**: Maximum depth of search space
- **V**: Number of vertices
- **E**: Number of edges
- **n**: Number of samples

## License

MIT License

## Author

Path Planning Library (sf)
