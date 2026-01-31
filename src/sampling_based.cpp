#include "sf/sampling_based.h"
#include "sf/informed_search.h"
#include <algorithm>
#include <cmath>

namespace sf {

// 4.1 PRM (Probabilistic Roadmap)
PRM::PRM(size_t samples, double radius, unsigned int seed) 
    : numSamples(samples), connectionRadius(radius), rng(seed) {}

void PRM::setCollisionChecker(CollisionChecker checker) {
    collisionChecker = checker;
}

Graph PRM::buildRoadmap(double minX, double maxX, double minY, double maxY) {
    Graph graph;
    
    std::uniform_real_distribution<double> distX(minX, maxX);
    std::uniform_real_distribution<double> distY(minY, maxY);
    
    // Sample points
    std::vector<std::pair<double, double>> points;
    points.reserve(numSamples);
    
    for (size_t i = 0; i < numSamples; i++) {
        double x = distX(rng);
        double y = distY(rng);
        
        if (!collisionChecker || !collisionChecker(x, y)) {
            points.emplace_back(x, y);
            graph.addNode(Node(points.size() - 1, x, y));
        }
    }
    
    // Connect nearby points
    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = i + 1; j < points.size(); j++) {
            double dx = points[i].first - points[j].first;
            double dy = points[i].second - points[j].second;
            double dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist <= connectionRadius) {
                // Check for collisions along the edge
                bool collision = false;
                if (collisionChecker) {
                    int numSteps = static_cast<int>(dist / 0.05);
                    for (int k = 0; k <= numSteps; k++) {
                        double t = static_cast<double>(k) / numSteps;
                        double x = points[i].first + t * (points[j].first - points[i].first);
                        double y = points[i].second + t * (points[j].second - points[i].second);
                        if (collisionChecker(x, y)) {
                            collision = true;
                            break;
                        }
                    }
                }
                
                if (!collision) {
                    graph.addEdge(i, j, dist);
                    graph.addEdge(j, i, dist);
                }
            }
        }
    }
    
    return graph;
}

PathResult PRM::findPath(const Graph& roadmap, int startId, int goalId) {
    return AStar::search(roadmap, startId, goalId, heuristics::euclidean);
}

PathResult PRM::search(double startX, double startY, double goalX, double goalY,
                      double minX, double maxX, double minY, double maxY) {
    // Build roadmap
    Graph roadmap = buildRoadmap(minX, maxX, minY, maxY);
    
    // Add start and goal nodes
    int startId = roadmap.nodeCount();
    int goalId = roadmap.nodeCount() + 1;
    
    roadmap.addNode(Node(startId, startX, startY));
    roadmap.addNode(Node(goalId, goalX, goalY));
    
    // Connect start and goal to nearby nodes
    for (const auto& pair : roadmap.getNodes()) {
        if (pair.first == startId || pair.first == goalId) continue;
        
        double distStart = std::sqrt(std::pow(pair.second.x - startX, 2) + 
                                    std::pow(pair.second.y - startY, 2));
        double distGoal = std::sqrt(std::pow(pair.second.x - goalX, 2) + 
                                   std::pow(pair.second.y - goalY, 2));
        
        if (distStart <= connectionRadius) {
            bool collision = false;
            if (collisionChecker) {
                int numSteps = static_cast<int>(distStart / 0.05);
                for (int k = 0; k <= numSteps; k++) {
                    double t = static_cast<double>(k) / numSteps;
                    double x = startX + t * (pair.second.x - startX);
                    double y = startY + t * (pair.second.y - startY);
                    if (collisionChecker(x, y)) {
                        collision = true;
                        break;
                    }
                }
            }
            if (!collision) {
                roadmap.addEdge(startId, pair.first, distStart);
                roadmap.addEdge(pair.first, startId, distStart);
            }
        }
        
        if (distGoal <= connectionRadius) {
            bool collision = false;
            if (collisionChecker) {
                int numSteps = static_cast<int>(distGoal / 0.05);
                for (int k = 0; k <= numSteps; k++) {
                    double t = static_cast<double>(k) / numSteps;
                    double x = goalX + t * (pair.second.x - goalX);
                    double y = goalY + t * (pair.second.y - goalY);
                    if (collisionChecker(x, y)) {
                        collision = true;
                        break;
                    }
                }
            }
            if (!collision) {
                roadmap.addEdge(goalId, pair.first, distGoal);
                roadmap.addEdge(pair.first, goalId, distGoal);
            }
        }
    }
    
    // Find path
    return findPath(roadmap, startId, goalId);
}

// 4.2 RRT (Rapidly-exploring Random Tree)
RRT::RRT(size_t maxIter, double step, unsigned int seed)
    : maxIterations(maxIter), stepSize(step), rng(seed) {}

void RRT::setCollisionChecker(CollisionChecker checker) {
    collisionChecker = checker;
}

PathResult RRT::search(double startX, double startY, double goalX, double goalY,
                      double minX, double maxX, double minY, double maxY) {
    std::vector<TreeNode> tree;
    tree.reserve(maxIterations);
    
    std::uniform_real_distribution<double> distX(minX, maxX);
    std::uniform_real_distribution<double> distY(minY, maxY);
    
    // Add start node
    tree.emplace_back(0, startX, startY, -1);
    
    std::uniform_real_distribution<double> goalProb(0.0, 1.0);
    
    for (size_t iter = 1; iter < maxIterations; iter++) {
        // Sample random point (with goal bias)
        double randX, randY;
        if (goalProb(rng) < 0.1) {
            randX = goalX;
            randY = goalY;
        } else {
            randX = distX(rng);
            randY = distY(rng);
        }
        
        // Find nearest node in tree
        int nearestId = -1;
        double minDist = std::numeric_limits<double>::infinity();
        
        for (const auto& node : tree) {
            double dx = node.x - randX;
            double dy = node.y - randY;
            double dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist < minDist) {
                minDist = dist;
                nearestId = node.id;
            }
        }
        
        if (nearestId == -1) continue;
        
        const TreeNode& nearest = tree[nearestId];
        
        // Steer towards random point
        double dx = randX - nearest.x;
        double dy = randY - nearest.y;
        double dist = std::sqrt(dx * dx + dy * dy);
        
        if (dist < 1e-6) continue;
        
        double stepX = nearest.x + (dx / dist) * std::min(stepSize, dist);
        double stepY = nearest.y + (dy / dist) * std::min(stepSize, dist);
        
        // Check collision
        if (collisionChecker && collisionChecker(stepX, stepY)) {
            continue;
        }
        
        // Add new node
        tree.emplace_back(iter, stepX, stepY, nearestId);
        
        // Check if goal is reached
        double goalDist = std::sqrt(std::pow(stepX - goalX, 2) + 
                                   std::pow(stepY - goalY, 2));
        
        if (goalDist <= stepSize) {
            // Reconstruct path
            std::vector<int> path;
            int node = iter;
            
            while (node != -1) {
                path.push_back(node);
                node = tree[node].parent;
            }
            
            std::reverse(path.begin(), path.end());
            
            // Calculate cost
            double cost = 0.0;
            for (size_t i = 1; i < path.size(); i++) {
                double dx = tree[path[i]].x - tree[path[i-1]].x;
                double dy = tree[path[i]].y - tree[path[i-1]].y;
                cost += std::sqrt(dx * dx + dy * dy);
            }
            
            return PathResult(path, cost, iter);
        }
    }
    
    return PathResult();
}

PathResult RRT::searchStar(double startX, double startY, double goalX, double goalY,
                          double minX, double maxX, double minY, double maxY,
                          double radius) {
    // RRT* implementation (optimal variant)
    // This is a simplified version that rewires the tree for optimality
    
    std::vector<TreeNode> tree;
    tree.reserve(maxIterations);
    
    std::uniform_real_distribution<double> distX(minX, maxX);
    std::uniform_real_distribution<double> distY(minY, maxY);
    
    // Add start node
    tree.emplace_back(0, startX, startY, -1);
    
    std::uniform_real_distribution<double> goalProb(0.0, 1.0);
    
    for (size_t iter = 1; iter < maxIterations; iter++) {
        // Sample random point
        double randX, randY;
        if (goalProb(rng) < 0.1) {
            randX = goalX;
            randY = goalY;
        } else {
            randX = distX(rng);
            randY = distY(rng);
        }
        
        // Find nearest node
        int nearestId = -1;
        double minDist = std::numeric_limits<double>::infinity();
        
        for (const auto& node : tree) {
            double dx = node.x - randX;
            double dy = node.y - randY;
            double dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist < minDist) {
                minDist = dist;
                nearestId = node.id;
            }
        }
        
        if (nearestId == -1) continue;
        
        const TreeNode& nearest = tree[nearestId];
        
        // Steer
        double dx = randX - nearest.x;
        double dy = randY - nearest.y;
        double dist = std::sqrt(dx * dx + dy * dy);
        
        if (dist < 1e-6) continue;
        
        double stepX = nearest.x + (dx / dist) * std::min(stepSize, dist);
        double stepY = nearest.y + (dy / dist) * std::min(stepSize, dist);
        
        // Check collision
        if (collisionChecker && collisionChecker(stepX, stepY)) {
            continue;
        }
        
        // Find best parent in neighborhood
        int bestParent = nearestId;
        double bestCost = std::numeric_limits<double>::infinity();
        
        for (const auto& node : tree) {
            double dx = node.x - stepX;
            double dy = node.y - stepY;
            double dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist <= radius) {
                double cost = std::sqrt(std::pow(node.x - startX, 2) + 
                                       std::pow(node.y - startY, 2)) + dist;
                
                if (cost < bestCost) {
                    bestCost = cost;
                    bestParent = node.id;
                }
            }
        }
        
        // Add new node
        tree.emplace_back(iter, stepX, stepY, bestParent);
        
        // Rewire nearby nodes
        for (auto& node : tree) {
            if (node.id == iter) continue;
            
            double dx = node.x - stepX;
            double dy = node.y - stepY;
            double dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist <= radius) {
                double newCost = bestCost + dist;
                double currentCost = std::sqrt(std::pow(node.x - startX, 2) + 
                                              std::pow(node.y - startY, 2));
                
                if (newCost < currentCost) {
                    node.parent = iter;
                }
            }
        }
        
        // Check if goal is reached
        double goalDist = std::sqrt(std::pow(stepX - goalX, 2) + 
                                   std::pow(stepY - goalY, 2));
        
        if (goalDist <= stepSize) {
            // Reconstruct path
            std::vector<int> path;
            int node = iter;
            
            while (node != -1) {
                path.push_back(node);
                node = tree[node].parent;
            }
            
            std::reverse(path.begin(), path.end());
            
            // Calculate cost
            double cost = 0.0;
            for (size_t i = 1; i < path.size(); i++) {
                double dx = tree[path[i]].x - tree[path[i-1]].x;
                double dy = tree[path[i]].y - tree[path[i-1]].y;
                cost += std::sqrt(dx * dx + dy * dy);
            }
            
            return PathResult(path, cost, iter);
        }
    }
    
    return PathResult();
}

} // namespace sf
