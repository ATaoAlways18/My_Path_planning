#include "sf/path_planning.h"
#include <iostream>
#include <iomanip>

void testBFS() {
    std::cout << "\n=== Testing BFS ===\n";
    
    sf::Graph graph;
    
    // Create a simple grid graph
    // 0-1-2
    // | | |
    // 3-4-5
    // | | |
    // 6-7-8
    
    for (int i = 0; i < 9; i++) {
        int x = i % 3;
        int y = i / 3;
        graph.addNode(sf::Node(i, x, y));
    }
    
    // Horizontal edges
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(3, 4, 1.0);
    graph.addEdgeUndirected(4, 5, 1.0);
    graph.addEdgeUndirected(6, 7, 1.0);
    graph.addEdgeUndirected(7, 8, 1.0);
    
    // Vertical edges
    graph.addEdgeUndirected(0, 3, 1.0);
    graph.addEdgeUndirected(3, 6, 1.0);
    graph.addEdgeUndirected(1, 4, 1.0);
    graph.addEdgeUndirected(4, 7, 1.0);
    graph.addEdgeUndirected(2, 5, 1.0);
    graph.addEdgeUndirected(5, 8, 1.0);
    
    auto result = sf::BFS::search(graph, 0, 8);
    
    if (result.found) {
        std::cout << "Path found: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost;
        std::cout << "\nNodes explored: " << result.nodesExplored << "\n";
    } else {
        std::cout << "No path found\n";
    }
}

void testDFS() {
    std::cout << "\n=== Testing DFS ===\n";
    
    sf::Graph graph;
    
    for (int i = 0; i < 9; i++) {
        int x = i % 3;
        int y = i / 3;
        graph.addNode(sf::Node(i, x, y));
    }
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(3, 4, 1.0);
    graph.addEdgeUndirected(4, 5, 1.0);
    graph.addEdgeUndirected(6, 7, 1.0);
    graph.addEdgeUndirected(7, 8, 1.0);
    graph.addEdgeUndirected(0, 3, 1.0);
    graph.addEdgeUndirected(3, 6, 1.0);
    graph.addEdgeUndirected(1, 4, 1.0);
    graph.addEdgeUndirected(4, 7, 1.0);
    graph.addEdgeUndirected(2, 5, 1.0);
    graph.addEdgeUndirected(5, 8, 1.0);
    
    auto result = sf::DFS::search(graph, 0, 8);
    
    if (result.found) {
        std::cout << "Path found: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost;
        std::cout << "\nNodes explored: " << result.nodesExplored << "\n";
    } else {
        std::cout << "No path found\n";
    }
}

void testDijkstra() {
    std::cout << "\n=== Testing Dijkstra ===\n";
    
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
    
    auto result = sf::Dijkstra::search(graph, 0, 5);
    
    if (result.found) {
        std::cout << "Path found: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost;
        std::cout << "\nNodes explored: " << result.nodesExplored << "\n";
    } else {
        std::cout << "No path found\n";
    }
}

void testAStar() {
    std::cout << "\n=== Testing A* ===\n";
    
    sf::Graph graph;
    
    // Create a simple grid
    for (int i = 0; i < 9; i++) {
        int x = i % 3;
        int y = i / 3;
        graph.addNode(sf::Node(i, x, y));
    }
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(3, 4, 1.0);
    graph.addEdgeUndirected(4, 5, 1.0);
    graph.addEdgeUndirected(6, 7, 1.0);
    graph.addEdgeUndirected(7, 8, 1.0);
    graph.addEdgeUndirected(0, 3, 1.0);
    graph.addEdgeUndirected(3, 6, 1.0);
    graph.addEdgeUndirected(1, 4, 1.0);
    graph.addEdgeUndirected(4, 7, 1.0);
    graph.addEdgeUndirected(2, 5, 1.0);
    graph.addEdgeUndirected(5, 8, 1.0);
    
    auto result = sf::AStar::search(graph, 0, 8);
    
    if (result.found) {
        std::cout << "Path found: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost;
        std::cout << "\nNodes explored: " << result.nodesExplored << "\n";
    } else {
        std::cout << "No path found\n";
    }
}

void testBestFirst() {
    std::cout << "\n=== Testing Best-First Search ===\n";
    
    sf::Graph graph;
    
    for (int i = 0; i < 9; i++) {
        int x = i % 3;
        int y = i / 3;
        graph.addNode(sf::Node(i, x, y));
    }
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(3, 4, 1.0);
    graph.addEdgeUndirected(4, 5, 1.0);
    graph.addEdgeUndirected(6, 7, 1.0);
    graph.addEdgeUndirected(7, 8, 1.0);
    graph.addEdgeUndirected(0, 3, 1.0);
    graph.addEdgeUndirected(3, 6, 1.0);
    graph.addEdgeUndirected(1, 4, 1.0);
    graph.addEdgeUndirected(4, 7, 1.0);
    graph.addEdgeUndirected(2, 5, 1.0);
    graph.addEdgeUndirected(5, 8, 1.0);
    
    auto result = sf::BestFirstSearch::search(graph, 0, 8);
    
    if (result.found) {
        std::cout << "Path found: ";
        for (int node : result.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << result.cost;
        std::cout << "\nNodes explored: " << result.nodesExplored << "\n";
    } else {
        std::cout << "No path found\n";
    }
}

void testRRT() {
    std::cout << "\n=== Testing RRT ===\n";
    
    sf::RRT rrt(5000, 0.2, 42);
    
    // No collision checker for simple test
    rrt.setCollisionChecker(nullptr);
    
    auto result = rrt.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    if (result.found) {
        std::cout << "Path found with " << result.path.size() << " waypoints";
        std::cout << "\nCost: " << std::fixed << std::setprecision(2) << result.cost;
        std::cout << "\nNodes explored: " << result.nodesExplored << "\n";
    } else {
        std::cout << "No path found\n";
    }
}

void testPRM() {
    std::cout << "\n=== Testing PRM ===\n";
    
    sf::PRM prm(200, 2.0, 42);
    
    // No collision checker for simple test
    prm.setCollisionChecker(nullptr);
    
    auto result = prm.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    if (result.found) {
        std::cout << "Path found with " << result.path.size() << " waypoints";
        std::cout << "\nCost: " << std::fixed << std::setprecision(2) << result.cost;
        std::cout << "\nNodes explored: " << result.nodesExplored << "\n";
    } else {
        std::cout << "No path found\n";
    }
}

int main() {
    std::cout << "Path Planning Library Test Suite\n";
    std::cout << "================================\n";
    
    testBFS();
    testDFS();
    testDijkstra();
    testAStar();
    testBestFirst();
    testRRT();
    testPRM();
    
    std::cout << "\nAll tests completed!\n";
    
    return 0;
}
