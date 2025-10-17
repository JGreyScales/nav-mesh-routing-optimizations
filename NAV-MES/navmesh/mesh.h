#pragma once
class Meshnode
{
private:
    Meshnode *north;
    Meshnode *east;
    Meshnode *south;
    Meshnode *west;
    unsigned short int terrainDifficulty;
    size_t x;
    size_t y;
public:
    Meshnode(unsigned short int, size_t, size_t);

    void decoupleNeighbors(Meshnode*);
    void coupleNeighbor(Meshnode*, unsigned short int);

    // getter
    Meshnode* getNorthMesh();
    Meshnode* getEastMesh();
    Meshnode* getSouthMesh();
    Meshnode* getWestMesh();
    size_t getX();
    size_t getY();
    unsigned short int getTerrainDifficulty();

    // setter
    void setNorthMesh(Meshnode*);
    void setEastMesh(Meshnode*);
    void setSouthMesh(Meshnode*);
    void setWestMesh(Meshnode*);

    ~Meshnode();
};


Meshnode::Meshnode(unsigned short int dif, size_t x, size_t y){
    this->terrainDifficulty = dif;
    this->x = x;
    this->y = y;
    this->north = nullptr;
    this->east = nullptr;
    this->south = nullptr;
    this->west = nullptr;
}


void Meshnode::decoupleNeighbors(Meshnode* nodeToDecouple){
    // check if self
    if (nodeToDecouple == this){
        this->~Meshnode();
    }

    // check if neighbor, unbind from both directions
    if (this->north == nodeToDecouple){
        if (this->north != nullptr){
            this->north->setSouthMesh(nullptr);
        }
        this->north = nullptr;

    } else if (this->east == nodeToDecouple){
        if (this->east != nullptr) {
            this->east->setWestMesh(nullptr);
        }
        this->east = nullptr;

    } else if (this->south == nodeToDecouple){
        if (this->south != nullptr){
            this->south->setNorthMesh(nullptr);
        }
        this->south = nullptr;
    } else if (this->west == nodeToDecouple){
        if (this->west != nullptr){
            this->west->setEastMesh(nullptr);
        }
        this->west = nullptr;
    }
    return;
}

// 1 = North, 2 = East, 3 = South, 4 = West
void Meshnode::coupleNeighbor(Meshnode* nodeToCouple, unsigned short int direction){
    switch(direction){
        case 1: {
            this->setNorthMesh(nodeToCouple);
            this->north->setSouthMesh(this);
            break;
        }

        case 2: {
            this->setEastMesh(nodeToCouple);
            this->east->setWestMesh(this);
            break;
        }

        case 3: {
            this->setSouthMesh(nodeToCouple);
            this->south->setNorthMesh(this);
            break;
        }

        case 4: {
            this->setWestMesh(nodeToCouple);
            this->west->setEastMesh(this);
            break;
        }
    }
    return;
}

Meshnode* Meshnode::getNorthMesh(void){
    return this->north;
}

Meshnode* Meshnode::getEastMesh(void){
    return this->east;
}

Meshnode* Meshnode::getSouthMesh(void){
    return this->south;
}

Meshnode* Meshnode::getWestMesh(void){
    return this->west;
}

size_t Meshnode::getX(void){
    return this->x;
}

size_t Meshnode::getY(void){
    return this->y;
}

unsigned short int Meshnode::getTerrainDifficulty()
{
    return this->terrainDifficulty;
}

void Meshnode::setNorthMesh(Meshnode* node){
    this->north = node;
    return;
}

void Meshnode::setEastMesh(Meshnode* node){
    this->east = node;
    return;
}

void Meshnode::setSouthMesh(Meshnode* node){
    this->south = node;
    return;
}

void Meshnode::setWestMesh(Meshnode* node){
    this->west = node;
    return;
}


Meshnode::~Meshnode(){
    this->decoupleNeighbors(this->north);
    this->decoupleNeighbors(this->east);
    this->decoupleNeighbors(this->south);
    this->decoupleNeighbors(this->west);
    return;
}