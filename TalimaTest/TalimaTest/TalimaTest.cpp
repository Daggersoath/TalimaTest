#include "Maze.h"
#include <time.h>

#include "AStar.h"

constexpr int WIDTH = 100;
constexpr int HEIGHT = 16;

int main() {
    std::srand((unsigned int)time(0));
    Maze<WIDTH, HEIGHT> maze;
    maze.Print();

    auto* astar = new SorasPathfindImplementation::AStar<WIDTH, HEIGHT>(maze.GetMaze(), maze.GetPlayerPos(), maze.GetDestination(), [](char c) {
        return c == MAZE_WALL;
    });

    while (!astar->Step());

    if (maze.TestPath(astar->GetPath())) {
        std::cout << "You did it!!!" << std::endl;
    }
    maze.Print();

    return 0;
}