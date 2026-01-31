#include "sf/path_planning.h"
#include <iostream>
#include <cassert>
#include <cmath>

void testGraphBasics() {
    std::cout << "Testing Graph Basics... ";
    
    sf::Graph graph;
    
    // Add nodes
    for (int i = 0; i < 5; i++) {
        graph.addNode(sf::Node(i, i, i));
    }
    
    assert(graph.nodeCount() == 5);
    
    // Add edges
    graph.addEdge(0, 1, 1.0);
    graph.addEdgeUndirected(2, 3, 2.0);
    
    assert(graph.edgeCount() == 3);
    assert(graph.hasNode(0));
    assert(!graph.hasNode(10));
    
    // Test neighbors
    auto neighbors = graph.getNeighbors(0);
    assert(neighbors.size() == 1);
    assert(neighbors[0].to == 1);
    
    std::cout << "PASSED\n";
}

void testBFS() {
    std::cout << "Testing BFS... ";
    
    sf::Graph graph;
    for (int i = 0; i < 4; i++) {
        graph.addNode(sf::Node(i, i, 0));
    }
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(2, 3, 1.0);
    
    auto result = sf::BFS::search(graph, 0, 3);
    
    assert(result.found);
    assert(result.path.size() == 4);
    assert(result.cost == 3.0);
    
    std::cout << "PASSED\n";
}

void testDFS() {
    std::cout << "Testing DFS... ";
    
    sf::Graph graph;
    for (int i = 0; i < 4; i++) {
        graph.addNode(sf::Node(i, i, 0));
    }
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(2, 3, 1.0);
    
    auto result = sf::DFS::search(graph, 0, 3);
    
    assert(result.found);
    assert(result.path.size() == 4);
    
    std::cout << "PASSED\n";
}

void testDijkstra() {
    std::cout << "Testing Dijkstra... ";
    
    sf::Graph graph;
    for (int i = 0; i < 4; i++) {
        graph.addNode(sf::Node(i, i, 0));
    }
    
    graph.addEdge(0, 1, 1.0);
    graph.addEdge(1, 2, 1.0);
    graph.addEdge(2, 3, 1.0);
    graph.addEdge(0, 3, 10.0); // Longer path
    
    auto result = sf::Dijkstra::search(graph, 0, 3);
    
    assert(result.found);
    assert(result.path.size() == 4);
    assert(result.cost == 3.0); // Should take shorter path
    
    std::cout << "PASSED\n";
}

void testAStar() {
    std::cout << "Testing A*... ";
    
    sf::Graph graph;
    for (int i = 0; i < 4; i++) {
        graph.addNode(sf::Node(i, i, 0));
    }
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(2, 3, 1.0);
    
    auto result = sf::AStar::search(graph, 0, 3);
    
    assert(result.found);
    assert(result.path.size() == 4);
    assert(result.cost == 3.0);
    
    // Test with different heuristic
    auto result2 = sf::AStar::search(graph, 0, 3, sf::heuristics::manhattan);
    assert(result2.found);
    
    std::cout << "PASSED\n";
}

void testBestFirst() {
    std::cout << "Testing Best-First Search... ";
    
    sf::Graph graph;
    for (int i = 0; i < 4; i++) {
        graph.addNode(sf::Node(i, i, 0));
    }
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(2, 3, 1.0);
    
    auto result = sf::BestFirstSearch::search(graph, 0, 3);
    
    assert(result.found);
    assert(result.path.size() == 4);
    
    std::cout << "PASSED\n";
}

void testIDAStar() {
    std::cout << "Testing IDA*... ";
    
    sf::Graph graph;
    for (int i = 0; i < 4; i++) {
        graph.addNode(sf::Node(i, i, 0));
    }
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    graph.addEdgeUndirected(2, 3, 1.0);
    
    auto result = sf::IDAStar::search(graph, 0, 3);
    
    assert(result.found);
    assert(result.path.size() == 4);
    
    std::cout << "PASSED\n";
}

void testRRT() {
    std::cout << "Testing RRT... ";
    
    sf::RRT rrt(2000, 0.2, 42);
    
    auto result = rrt.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    assert(result.found);
    assert(result.path.size() > 0);
    assert(result.cost > 0);
    
    std::cout << "PASSED\n";
}

void testRRTStar() {
    std::cout << "Testing RRT*... ";
    
    sf::RRT rrt(3000, 0.2, 42);
    
    auto result = rrt.searchStar(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0, 1.0);
    
    assert(result.found);
    assert(result.path.size() > 0);
    
    std::cout << "PASSED\n";
}

void testPRM() {
    std::cout << "Testing PRM... ";
    
    sf::PRM prm(200, 2.0, 42);
    
    auto result = prm.search(0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0);
    
    assert(result.found);
    assert(result.path.size() > 0);
    
    std::cout << "PASSED\n";
}

void testHeuristics() {
    std::cout << "Testing Heuristics... ";
    
    sf::Node a(0, 0.0, 0.0);
    sf::Node b(1, 3.0, 4.0);
    
    // Euclidean: sqrt(3^2 + 4^2) = 5
    double euclidean = sf::heuristics::euclidean(a, b);
    assert(std::abs(euclidean - 5.0) < 0.001);
    
    // Manhattan: |3| + |4| = 7
    double manhattan = sf::heuristics::manhattan(a, b);
    assert(std::abs(manhattan - 7.0) < 0.001);
    
    // Zero
    double zero = sf::heuristics::zero(a, b);
    assert(zero == 0.0);
    
    std::cout << "PASSED\n";
}

void testNoPath() {
    std::cout << "Testing No Path Scenario... ";
    
    sf::Graph graph;
    
    // Create two disconnected components
    graph.addNode(sf::Node(0, 0.0, 0.0));
    graph.addNode(sf::Node(1, 1.0, 0.0));
    graph.addNode(sf::Node(2, 10.0, 0.0));
    graph.addNode(sf::Node(3, 11.0, 0.0));
    
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(2, 3, 1.0);
    
    auto result = sf::AStar::search(graph, 0, 3);
    
    assert(!result.found);
    
    std::cout << "PASSED\n";
}

void testPathResultStructure() {
    std::cout << "Testing PathResult Structure... ";
    
    sf::PathResult result;
    assert(!result.found);
    assert(result.path.empty());
    assert(result.cost == 0.0);
    assert(result.nodesExplored == 0);
    
    std::vector<int> path = {1, 2, 3};
    sf::PathResult result2(path, 5.0, 10);
    assert(result2.found);
    assert(result2.path.size() == 3);
    assert(result2.cost == 5.0);
    assert(result2.nodesExplored == 10);
    
    std::cout << "PASSED\n";
}

int main() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "Comprehensive Path Planning Test Suite\n";
    std::cout << "========================================\n\n";
    
    try {
        testGraphBasics();
        testBFS();
        testDFS();
        testDijkstra();
        testAStar();
        testBestFirst();
        testIDAStar();
        testRRT();
        testRRTStar();
        testPRM();
        testHeuristics();
        testNoPath();
        testPathResultStructure();
        
        std::cout << "\n";
        std::cout << "========================================\n";
        std::cout << "ALL TESTS PASSED! ✓\n";
        std::cout << "========================================\n";
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTEST FAILED: " << e.what() << "\n";
        return 1;
    }
}
