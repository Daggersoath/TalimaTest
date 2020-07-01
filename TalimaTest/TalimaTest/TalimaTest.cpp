#include "Maze.h"
#include <time.h>

constexpr int WIDTH = 100;
constexpr int HEIGHT = 16;

/*
    Greetings! Your challenge today, if you choose to accept it is to create a pathfinding algorithm.
    This algorithm must be able to find its way through the maze provided by the Maze class.
    To get a Maze from the Maze class, do:
        Maze<MAZE_WIDTH, MAZE_HEIGHT> maze;
        maze.GetMaze();

    The maze also contains a player position, which is the starting position and a destination which is the exit.
    To test to see if your path works you must send it into "maze.TestPath", where path.front() is the starting position and path.back() is the destination

    The maze is an array of chars, in the Maze header there is definitions for what different chars mean
        Wall
        Path
        Player

    To help you with this I recommend reading through - https://en.wikipedia.org/wiki/A*_search_algorithm
    This article has some very nice pseudocode for the algorithm.

    If you have any questions please feel free to ask in the discord!
    If you are feeling completely lost, I have included my own implementation of A*
*/

int main() {
    std::srand((unsigned int)time(0));
    Maze<WIDTH, HEIGHT> maze;
    maze.Print();

    std::vector<std::pair<size_t, size_t>> path;

    if (maze.TestPath(path)) {
        std::cout << "You did it!!!" << std::endl;
    }
    else {
        std::cout << "Sorry, not quite" << std::endl;
    }
    maze.Print();

    return 0;
}