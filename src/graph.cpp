#include "sf/graph.h"
#include <stdexcept>

namespace sf {

void Graph::addNode(const Node& node) {
    nodes[node.id] = node;
    if (adjacency.find(node.id) == adjacency.end()) {
        adjacency[node.id] = std::vector<Edge>();
    }
}

void Graph::addEdge(int from, int to, double weight) {
    if (!hasNode(from) || !hasNode(to)) {
        throw std::runtime_error("Cannot add edge: nodes do not exist");
    }
    adjacency[from].emplace_back(from, to, weight);
}

void Graph::addEdgeUndirected(int from, int to, double weight) {
    addEdge(from, to, weight);
    addEdge(to, from, weight);
}

const Node& Graph::getNode(int id) const {
    auto it = nodes.find(id);
    if (it == nodes.end()) {
        throw std::runtime_error("Node not found: " + std::to_string(id));
    }
    return it->second;
}

const std::vector<Edge>& Graph::getNeighbors(int id) const {
    auto it = adjacency.find(id);
    if (it == adjacency.end()) {
        static std::vector<Edge> empty;
        return empty;
    }
    return it->second;
}

const std::unordered_map<int, Node>& Graph::getNodes() const {
    return nodes;
}

bool Graph::hasNode(int id) const {
    return nodes.find(id) != nodes.end();
}

size_t Graph::nodeCount() const {
    return nodes.size();
}

size_t Graph::edgeCount() const {
    size_t count = 0;
    for (const auto& pair : adjacency) {
        count += pair.second.size();
    }
    return count;
}

std::vector<int> Graph::getNodeIds() const {
    std::vector<int> ids;
    ids.reserve(nodes.size());
    for (const auto& pair : nodes) {
        ids.push_back(pair.first);
    }
    return ids;
}

void Graph::clear() {
    nodes.clear();
    adjacency.clear();
}

namespace heuristics {

double euclidean(const Node& a, const Node& b) {
    return a.distance(b);
}

double manhattan(const Node& a, const Node& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

double zero(const Node& a, const Node& b) {
    (void)a; (void)b;
    return 0.0;
}

} // namespace heuristics

} // namespace sf
