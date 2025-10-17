#include <vector>
#include <string>
#include <iostream>

class nodeMatrix
{
private:
    std::vector<std::vector<char>> matrix;
    size_t min_y;
    size_t min_x;

    size_t max_y;

public:
    nodeMatrix(size_t, size_t);

    void printMatrix();
    void addToMatrix(size_t, size_t);
    void removeFromMatrix(size_t, size_t);

    // getters
    size_t getMinY();
    size_t getMinX();
    std::vector<std::vector<char>> getMatrix();

    ~nodeMatrix();
};

nodeMatrix::nodeMatrix(size_t entryNodeX, size_t entryNodeY)
{

    // these are arranged by this so the first add will bind to min + max values
    this->min_x = entryNodeX;
    this->min_y = entryNodeY;

    this->matrix = std::vector<std::vector<char>>();
}

void nodeMatrix::printMatrix()
{
    for (std::vector<char> row : matrix){
        std::string line = "";
        for (char column : row){
            line += column;
        }
        std::cout << line << std::endl;
    }
}

// matrix[row][column]
void nodeMatrix::addToMatrix(size_t x, size_t y)
{
    // if we need increase the size of the vector to the left
    if (x < min_x)
    {
        matrix.insert(matrix.begin(), min_x - x, std::vector<char>());
        min_x = x;
    }

    if (y < min_y)
    {
        for (std::vector<char> row : matrix)
        {
            row.insert(row.begin(), min_y - y, ' ');
        }
        min_y = y;
    }

    // modify our offsets
    x = x - min_x;
    y = y - min_y;

    // if we need to increase the length of the vector  to the right
    if (x > matrix.size())
        matrix.resize(x + 1);


    // only scale rows to the required size as needed
    // this means each row may be a dynamic length
    // but will micro-optimize memory usage & iteration times
    if (y > matrix[x].size())
        matrix[x].resize(y + 1);


    matrix[x][y] = 'x'; // actually set the value that was added

    return;
}

void nodeMatrix::removeFromMatrix(size_t x, size_t y)
{
    matrix[x][y] = ' ';
}

size_t nodeMatrix::getMinY()
{
    return this->min_y;
}

size_t nodeMatrix::getMinX()
{
    return this->min_x;
}

std::vector<std::vector<char>> nodeMatrix::getMatrix()
{
    return this->matrix;
}

nodeMatrix::~nodeMatrix()
{
}
