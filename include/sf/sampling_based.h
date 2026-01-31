#pragma once

#include "graph.h"
#include <random>
#include <vector>

namespace sf {

// 4.1 PRM (Probabilistic Roadmap)
class PRM {
private:
    size_t numSamples;
    double connectionRadius;
    std::mt19937 rng;
    
    // Collision detection function (user-defined)
    using CollisionChecker = std::function<bool(double, double)>;
    CollisionChecker collisionChecker;
    
public:
    PRM(size_t samples = 1000, double radius = 1.0, unsigned int seed = 42);
    
    void setCollisionChecker(CollisionChecker checker);
    
    // Build roadmap
    Graph buildRoadmap(double minX, double maxX, double minY, double maxY);
    
    // Find path using built roadmap
    PathResult findPath(const Graph& roadmap, int startId, int goalId);
    
    // Complete path finding
    PathResult search(double startX, double startY, double goalX, double goalY,
                     double minX, double maxX, double minY, double maxY);
};

// 4.2 RRT (Rapidly-exploring Random Tree)
class RRT {
private:
    size_t maxIterations;
    double stepSize;
    std::mt19937 rng;
    
    // Collision detection function
    using CollisionChecker = std::function<bool(double, double)>;
    CollisionChecker collisionChecker;
    
    struct TreeNode {
        int id;
        double x, y;
        int parent;
        
        TreeNode(int i, double px, double py, int p) 
            : id(i), x(px), y(py), parent(p) {}
    };
    
public:
    RRT(size_t maxIter = 10000, double step = 0.1, unsigned int seed = 42);
    
    void setCollisionChecker(CollisionChecker checker);
    
    // Find path
    PathResult search(double startX, double startY, double goalX, double goalY,
                     double minX, double maxX, double minY, double maxY);
    
    // RRT* (optimal variant)
    PathResult searchStar(double startX, double startY, double goalX, double goalY,
                         double minX, double maxX, double minY, double maxY,
                         double radius = 1.0);
};

} // namespace sf
