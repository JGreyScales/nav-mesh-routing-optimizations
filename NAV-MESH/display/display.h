#include "mesh.h"
#include "nodeMatrix.h"
#include <unordered_set>
#include <stack>
#include <memory>
#include <array>
#include <algorithm>

class display
{
private:
    Meshnode *entryNode;
    std::unique_ptr<nodeMatrix> matrix;
    unsigned int entryX;
    unsigned int entryY;
    unsigned int nodeCount;

    void calculateEntryPosition();
    void calculateNodeMatrix();

    int display::pathdifficulty(std::array<Meshnode *, 3>);
    double distanceToGoal(Meshnode *, size_t, size_t);
    std::array<Meshnode*, 3> skimSearch(Meshnode *, size_t *, size_t *, std::array<Meshnode*, 3>, size_t = 0);

public:
    display(void);

    bool moveToPos(size_t, size_t);

    // getter
    Meshnode *getEntryNode();
    unsigned int getEntryX();
    unsigned int getEntryY();
    unsigned int getNodeCount();

    // setter
    void setEntryPosition(Meshnode *);

    ~display();
};

inline void display::calculateEntryPosition()
{
    if (this->entryNode == nullptr)
        return;

    this->entryX = this->entryNode->getX();
    this->entryY = this->entryNode->getY();
    return;
}

inline void display::calculateNodeMatrix()
{
    if (this->entryNode == nullptr)
        return;

    std::unordered_set<Meshnode *> visitedNodes; // uses bucket based on the hashed value of pointer, makes retrieval quick

    std::stack<Meshnode *> nodePath; // LIFO stack
    nodePath.emplace(this->entryNode);

    while (!nodePath.empty())
    {
        Meshnode *cNode = nodePath.top();
        nodePath.pop(); // since we will be computing all neighbors, just pop now to save memory

        // current node is not in visited nodes and successfully emplaced
        if (visitedNodes.emplace(cNode).second)
        {
            this->nodeCount++;
            this->matrix->addToMatrix(cNode->getX(), cNode->getY());
            std::unordered_set<Meshnode *>::iterator setEnd = visitedNodes.end();

            Meshnode *north = cNode->getNorthMesh();
            Meshnode *east = cNode->getEastMesh();
            Meshnode *south = cNode->getSouthMesh();
            Meshnode *west = cNode->getWestMesh();

            // add every direction if unvisited & populated to the stack so it uses star pattern DFS algorithm

            if (north != nullptr && visitedNodes.find(north) == setEnd)
            {
                nodePath.emplace(north);
            }

            if (east != nullptr && visitedNodes.find(east) == setEnd)
            {
                nodePath.emplace(east);
            }

            if (south != nullptr && visitedNodes.find(south) == setEnd)
            {
                nodePath.emplace(south);
            }

            if (west != nullptr && visitedNodes.find(west) == setEnd)
            {
                nodePath.emplace(west);
            }
        }
    }
}

inline int display::pathdifficulty(std::array<Meshnode *, 3> path)
{
    int difficulty = 0;
    for (const auto &node : path)
    {
        if (node)
        {
            difficulty += node->getTerrainDifficulty();
        }
    }
    return difficulty;
}

// Euclidean distance to goal
inline double display::distanceToGoal(Meshnode *node, size_t goalX, size_t goalY)
{
    double dx = static_cast<double>(goalX) - node->getX();
    double dy = static_cast<double>(goalY) - node->getY();
    return std::sqrt(dx * dx + dy * dy);
}


// Recursive DFS skim search to find optimal path processing to depths of 3 before selecting best 
// option & then repeating until target met
std::array<Meshnode*, 3> display::skimSearch(Meshnode* cNode, size_t* goalX, size_t* goalY, std::array<Meshnode*, 3> path, size_t cDepth = 0) {
    if (cDepth >= 3 || !cNode) {
        return path;
    }

    // Store current node at this depth
    path[cDepth] = cNode;

    // Set up directions
    Meshnode* directions[4] = {
        cNode->getNorthMesh(),
        cNode->getEastMesh(),
        cNode->getSouthMesh(),
        cNode->getWestMesh()
    };

    int excludedDirection = -1;
    double maxDistance = -1.0;
    int validDirections = 0;

    // Determine which direction points furthest from goal
    for (size_t i = 0; i < 4; ++i) {
        Meshnode* neighbor = directions[i];
        if (neighbor) {
            ++validDirections;
            double dist = distanceToGoal(neighbor, *goalX, *goalY);

            if (dist > maxDistance) {
                maxDistance = dist;
                excludedDirection = static_cast<int>(i);
            } else if (abs(dist - maxDistance) < 1e-6 && excludedDirection >= 0) {
                // Tie-breaker: higher terrain difficulty gets excluded
                if (neighbor->getTerrainDifficulty() > directions[excludedDirection]->getTerrainDifficulty()) {
                    excludedDirection = static_cast<int>(i);
                }
            }
        }
    }

    // If only one direction is valid, do not exclude it
    if (validDirections <= 1) {
        excludedDirection = -1;
    }

    double bestScore = std::numeric_limits<double>::max();
    std::array<Meshnode*, 3> bestPath = path;

    // Explore all directions except the excluded one
    for (std::size_t i = 0; i < 4; ++i) {
        if (static_cast<int>(i) == excludedDirection || !directions[i]) {
            continue;
        }
        std::array<Meshnode*, 3> copiedPath;
        std::copy(path.begin(), path.end(), copiedPath);
        std::array<Meshnode*, 3> candidatePath = skimSearch(directions[i], goalX, goalY, copiedPath, cDepth + 1);

        if (candidatePath[3 - 1]) {
            double distance = distanceToGoal(candidatePath[2], *goalX, *goalY);
            double difficulty = static_cast<double>(this->pathdifficulty(candidatePath));
            double score = distance + difficulty;

            if (score < bestScore) {
                bestScore = score;
                bestPath = std::move(candidatePath);
            }
        }
    }

    return bestPath;
}

display::display(void)
{
    this->entryNode = nullptr;
    this->matrix = nullptr;
    this->entryX = 0;
    this->entryY = 0;
    this->nodeCount = 0;
}

inline bool display::moveToPos(size_t x, size_t y)
{
    // the matrix is already created
    // we do not need to generate the matrix map
    // we only need to generate the path

    // the mesh will feature void gaps, as well as a terrain difficuluty modifer.
    // this means the most straight path may not be the fastest

    std::stack<Meshnode *> path; // the stack will be our final path
    int allowedDepth = (this->distanceToGoal(this->entryNode, x, y) * 1.3);

    // spread out in every direction 3 tiles and find the lowest dif  route
}

inline Meshnode *display::getEntryNode()
{
    return this->entryNode;
}

inline unsigned int display::getEntryX()
{
    return this->entryX;
}

inline unsigned int display::getEntryY()
{
    return this->entryY;
}

inline unsigned int display::getNodeCount()
{
    return this->nodeCount;
}

inline void display::setEntryPosition(Meshnode *entryNode)
{
    this->entryNode = entryNode;
    if (this->entryNode != nullptr)
    {
        this->calculateEntryPosition();
        this->matrix = std::make_unique<nodeMatrix>(this->entryX, this->entryY);
        this->calculateNodeMatrix();
    }
    return;
}

display::~display()
{
    matrix.reset();
}
