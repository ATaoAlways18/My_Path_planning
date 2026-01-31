// Usage Demo - Copy and paste this to get started quickly
// Compile: g++ -std=c++17 -I/path/to/sf/include USAGE_DEMO.cpp -L/path/to/sf/build -lpath_planning -o demo

#include "sf/path_planning.h"
#include <iostream>

void demo1_basic_graph_search() {
    std::cout << "\n=== Demo 1: Basic Graph Search ===\n";
    
    sf::Graph graph;
    
    // Create a simple line: 0 -- 1 -- 2
    graph.addNode(sf::Node(0, 0.0, 0.0));
    graph.addNode(sf::Node(1, 1.0, 0.0));
    graph.addNode(sf::Node(2, 2.0, 0.0));
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    
    // Find path using A*
    auto result = sf::AStar::search(graph, 0, 2);
    
    if (result.found) {
        std::cout << "Path found: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost << "\n";
    }
}

void demo2_grid_pathfinding() {
    std::cout << "\n=== Demo 2: Grid Pathfinding ===\n";
    
    sf::Graph graph;
    int width = 5, height = 5;
    
    // Create 5x5 grid
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int id = y * width + x;
            graph.addNode(sf::Node(id, x, y));
        }
    }
    
    // Connect adjacent cells
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int id = y * width + x;
            if (x < width - 1) graph.addEdgeUndirected(id, id + 1, 1.0);
            if (y < height - 1) graph.addEdgeUndirected(id, id + width, 1.0);
        }
    }
    
    // Find path from top-left (0) to bottom-right (24)
    auto result = sf::AStar::search(graph, 0, 24);
    
    if (result.found) {
        std::cout << "Path found with " << result.path.size() << " nodes\n";
        std::cout << "Cost: " << result.cost << "\n";
    }
}

void demo3_continuous_planning() {
    std::cout << "\n=== Demo 3: Continuous Space Planning (RRT) ===\n";
    
    sf::RRT rrt(5000, 0.2, 42);
    
    // Find path from (0,0) to (10,10)
    auto result = rrt.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    if (result.found) {
        std::cout << "Path found with " << result.path.size() << " waypoints\n";
        std::cout << "Cost: " << result.cost << "\n";
    }
}

void demo4_with_collision() {
    std::cout << "\n=== Demo 4: With Collision Detection ===\n";
    
    sf::RRT rrt(5000, 0.2, 42);
    
    // Define circular obstacle at (5, 5) with radius 1
    rrt.setCollisionChecker([](double x, double y) {
        double dx = x - 5.0;
        double dy = y - 5.0;
        return (dx * dx + dy * dy) < 1.0;
    });
    
    auto result = rrt.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    if (result.found) {
        std::cout << "Path found avoiding obstacle!\n";
        std::cout << "Waypoints: " << result.path.size() << "\n";
        std::cout << "Cost: " << result.cost << "\n";
    }
}

void demo5_algorithm_comparison() {
    std::cout << "\n=== Demo 5: Algorithm Comparison ===\n";
    
    sf::Graph graph;
    
    // Create a weighted graph
    for (int i = 0; i < 6; i++) {
        graph.addNode(sf::Node(i, i, 0));
    }
    
    graph.addEdge(0, 1, 4.0);
    graph.addEdge(0, 2, 2.0);
    graph.addEdge(1, 2, 1.0);
    graph.addEdge(1, 3, 5.0);
    graph.addEdge(2, 3, 8.0);
    graph.addEdge(2, 4, 10.0);
    graph.addEdge(3, 4, 2.0);
    graph.addEdge(3, 5, 6.0);
    graph.addEdge(4, 5, 3.0);
    
    std::cout << "Comparing algorithms from node 0 to node 5:\n";
    
    // BFS
    auto bfs = sf::BFS::search(graph, 0, 5);
    if (bfs.found) {
        std::cout << "BFS: Cost=" << bfs.cost << ", Explored=" << bfs.nodesExplored << "\n";
    }
    
    // Dijkstra
    auto dijkstra = sf::Dijkstra::search(graph, 0, 5);
    if (dijkstra.found) {
        std::cout << "Dijkstra: Cost=" << dijkstra.cost << ", Explored=" << dijkstra.nodesExplored << "\n";
    }
    
    // A*
    auto astar = sf::AStar::search(graph, 0, 5);
    if (astar.found) {
        std::cout << "A*: Cost=" << astar.cost << ", Explored=" << astar.nodesExplored << "\n";
    }
}

void demo6_prm() {
    std::cout << "\n=== Demo 6: Probabilistic Roadmap (PRM) ===\n";
    
    sf::PRM prm(500, 2.0, 42);
    
    // Build roadmap and find path
    auto result = prm.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    if (result.found) {
        std::cout << "PRM found path with " << result.path.size() << " waypoints\n";
        std::cout << "Cost: " << result.cost << "\n";
    }
}

int main() {
    std::cout << "========================================\n";
    std::cout << "Path Planning Library - Usage Demo\n";
    std::cout << "========================================\n";
    
    demo1_basic_graph_search();
    demo2_grid_pathfinding();
    demo3_continuous_planning();
    demo4_with_collision();
    demo5_algorithm_comparison();
    demo6_prm();
    
    std::cout << "\n========================================\n";
    std::cout << "Demo completed successfully!\n";
    std::cout << "========================================\n";
    
    return 0;
}
