#include <time.h>
#include <cstdlib>

#include "maze.h"


int main(int argc, char** argv)
{
    std::srand(time(NULL));

    Maze::RawMaze* Labyrinth = new Maze::RawMaze();
    Labyrinth->loadFromImage("maps/500x500.bmp");
    Labyrinth->GenerateNodes();
    Labyrinth->Simplify();
    Labyrinth->DrawSolution();
    Labyrinth->ExportMaze();

    delete Labyrinth;
    return 0;
}
