#ifndef MAZE_H
#define MAZE_H

#include <cstdio>
#include <string>

namespace Maze
{
enum NODE_DIRECTION{UP,DOWN,LEFT,RIGHT};
struct Node
{
    Node* Connection[4] = {NULL,NULL,NULL,NULL};
    bool visited = false;
    int x = 0;
    int y = 0;
};
struct SElement
{
    Node* point    = NULL;
    SElement* next = NULL;
};
class RawMaze
{
public:
    RawMaze()
    {
        //passable   = NULL;
        coloration  = NULL;
        //Nodefield   = NULL;
        NodeList    = NULL;
        Start = End = NULL;
        Solution = NULL;
        width  = 0;
        height = 0;
        number = 0;
        length = 0;
    }
    ~RawMaze()
    {
        printf("deleting...\n");
        if(coloration != NULL)
        {
            for(int i=0;i<width;++i)
            {
                delete[] coloration[i];
            }
            delete[] coloration;
        }
        if(NodeList != NULL)
        {
            delete[] NodeList;
        }
        if(Solution != NULL)
        {
            SElement* tempSE = Solution;
            while(tempSE != NULL)
            {
                Solution = tempSE;
                tempSE = tempSE->next;
                delete Solution;
            }
        }
        delete Start;
        delete End;
    }
    //Acquiring a maze
    void loadFromText(std::string filepath);
    void loadFromImage(std::string filepath);
    void GenerateMaze(int w,int h, int Start_x = -1, int Start_y = -1, int End_x = -1, int End_y = -1);


    void GenerateNodes(bool Print = false);

    //Solving Algorithms
    void Dijkstra(int* test = NULL);
    void A_Star();
    void Simplify();

    void PrintMaze();
    void ExportMaze();
    void ExportMazeStream();

    void DrawSolution();
private:
    void PrintNodes();

    int width, height,number,length;

    int** coloration;
    Maze::Node** NodeList;
    //Maze::Node*** Nodefield;
    Maze::SElement* Solution;

    Node* Start;
    Node* End;
};

}
#endif
