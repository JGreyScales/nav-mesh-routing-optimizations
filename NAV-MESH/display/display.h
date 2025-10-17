#include "mesh.h"
#include "nodeMatrix.h"
#include <unordered_set>
#include <stack>
#include <memory>


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
public:
    display(void);

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


display::display(void)
{
    this->entryNode = nullptr;
    this->matrix = nullptr;
    this->entryX = 0;
    this->entryY = 0;
    this->nodeCount = 0;
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
