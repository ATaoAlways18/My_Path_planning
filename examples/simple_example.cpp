#include "sf/path_planning.h"
#include <iostream>

int main() {
    std::cout << "Path Planning Library - Simple Example\n";
    std::cout << "=====================================\n\n";
    
    // Create a graph
    sf::Graph graph;
    
    // Add nodes with positions (x, y)
    // Let's create a simple 3x3 grid
    for (int i = 0; i < 9; i++) {
        int x = i % 3;
        int y = i / 3;
        graph.addNode(sf::Node(i, x, y));
    }
    
    // Add edges (undirected for bidirectional movement)
    // Horizontal connections
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(3, 4, 1.0);
    graph.addEdgeUndirected(4, 5, 1.0);
    graph.addEdgeUndirected(6, 7, 1.0);
    graph.addEdgeUndirected(7, 8, 1.0);
    
    // Vertical connections
    graph.addEdgeUndirected(0, 3, 1.0);
    graph.addEdgeUndirected(3, 6, 1.0);
    graph.addEdgeUndirected(1, 4, 1.0);
    graph.addEdgeUndirected(4, 7, 1.0);
    graph.addEdgeUndirected(2, 5, 1.0);
    graph.addEdgeUndirected(5, 8, 1.0);
    
    std::cout << "Graph created with " << graph.nodeCount() << " nodes and " 
              << graph.edgeCount() << " edges\n\n";
    
    // Test different algorithms
    std::cout << "Finding path from node 0 to node 8:\n";
    std::cout << "-----------------------------------\n";
    
    // BFS
    auto bfsResult = sf::BFS::search(graph, 0, 8);
    if (bfsResult.found) {
        std::cout << "BFS Path: ";
        for (int node : bfsResult.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << bfsResult.cost 
                  << "\nNodes explored: " << bfsResult.nodesExplored << "\n\n";
    }
    
    // Dijkstra
    auto dijkstraResult = sf::Dijkstra::search(graph, 0, 8);
    if (dijkstraResult.found) {
        std::cout << "Dijkstra Path: ";
        for (int node : dijkstraResult.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << dijkstraResult.cost 
                  << "\nNodes explored: " << dijkstraResult.nodesExplored << "\n\n";
    }
    
    // A*
    auto aStarResult = sf::AStar::search(graph, 0, 8);
    if (aStarResult.found) {
        std::cout << "A* Path: ";
        for (int node : aStarResult.path) {
            std::cout << node << " ";
        }
        std::cout << "\nCost: " << aStarResult.cost 
                  << "\nNodes explored: " << aStarResult.nodesExplored << "\n\n";
    }
    
    // RRT (continuous space planning)
    std::cout << "RRT (Continuous Space Planning):\n";
    std::cout << "--------------------------------\n";
    
    sf::RRT rrt(5000, 0.2, 42);
    // Optional: Set collision checker
    // rrt.setCollisionChecker([](double x, double y) {
    //     // Return true if (x,y) is in collision
    //     return false;
    // });
    
    auto rrtResult = rrt.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    if (rrtResult.found) {
        std::cout << "RRT found path with " << rrtResult.path.size() << " waypoints\n";
        std::cout << "Cost: " << rrtResult.cost << "\n";
        std::cout << "Nodes explored: " << rrtResult.nodesExplored << "\n\n";
    }
    
    // PRM (Probabilistic Roadmap)
    std::cout << "PRM (Probabilistic Roadmap):\n";
    std::cout << "----------------------------\n";
    
    sf::PRM prm(500, 2.0, 42);
    // Optional: Set collision checker
    // prm.setCollisionChecker([](double x, double y) {
    //     // Return true if (x,y) is in collision
    //     return false;
    // });
    
    auto prmResult = prm.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    if (prmResult.found) {
        std::cout << "PRM found path with " << prmResult.path.size() << " waypoints\n";
        std::cout << "Cost: " << prmResult.cost << "\n";
        std::cout << "Nodes explored: " << prmResult.nodesExplored << "\n\n";
    }
    
    std::cout << "Example completed successfully!\n";
    
    return 0;
}
