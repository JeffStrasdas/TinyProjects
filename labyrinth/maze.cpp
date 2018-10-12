#include <fstream>
#include <cmath>

#include "maze.h"
#include "image.h"

void Maze::RawMaze::loadFromText(std::string filepath)
{
    int i,k;

    std::ifstream ifile (filepath.c_str());
    if(ifile.is_open())
    {
        ifile >> width;
        ifile >> height;

        coloration = new int*[width];
        for(i = 0;i<width;++i)
        {
            coloration[i] = new int[height];
        }
        for(i = 0;i<height;++i)
            for(k = 0;k<width;++k)
                ifile >> coloration[k][i];
        ifile.close();
        printf("loading successful!\n");
    }
    else
        printf("Can't open file %s!\n",filepath.c_str());
}
void Maze::RawMaze::PrintMaze()
{
    if(coloration == NULL)
        return;
    int i,j;
    for(i=0;i<height;++i)
    {
        for(j=0;j<width;++j)
        {
            printf("%c",coloration[j][i] == 0 ? 219 : coloration[j][i] == 1 ? 32 : coloration[j][i] == 2 || coloration[j][i] == 3 ? 64 : '?');
        }
        printf("\n");
    }
    if(true)
    {
        printf("width: %i, height: %i\nnumber: %i, length: %i\n",width,height,number,length);
        if(NodeList != NULL)
            printf("Nodelist is not empty\n");

    }
}

void Maze::RawMaze::PrintNodes()
{/*
    if(Nodefield == NULL)
        printf("No nodes found!");
    for(int i=0;i<height;++i)
    {
        for(int k=0;k<width;++k)
            if(coloration[k][i] == 0)
                printf("%c",176);
            else
                if(Nodefield[i][k] != NULL)
                {
                    if(Nodefield[i][k]->x == k && Nodefield[i][k]->y == i)
                    {
                        if(Nodefield[i][k] == Start || Nodefield[i][k] == End)
                            printf("@");
                        else
                        {
                            if(Nodefield[i][k]->Connection[UP] != NULL)
                            {
                                if(Nodefield[i][k]->Connection[RIGHT] != NULL)
                                {
                                    if(Nodefield[i][k]->Connection[DOWN] != NULL)
                                        if(Nodefield[i][k]->Connection[LEFT] != NULL)
                                            printf("%c",197);
                                        else
                                            printf("%c",195);
                                    else
                                        if(Nodefield[i][k]->Connection[LEFT] != NULL)
                                            printf("%c",193);
                                        else
                                            printf("%c",192);
                                }
                                else
                                {
                                    if(Nodefield[i][k]->Connection[LEFT] != NULL)
                                        if(Nodefield[i][k]->Connection[DOWN] != NULL)
                                            printf("%c",180);
                                        else
                                            printf("%c",217);
                                    else
                                        printf("x");
                                }
                            }
                            else
                            {
                                if(Nodefield[i][k]->Connection[RIGHT] != NULL)
                                {
                                    if(Nodefield[i][k]->Connection[DOWN] != NULL)
                                        if(Nodefield[i][k]->Connection[LEFT] != NULL)
                                            printf("%c",194);
                                        else
                                            printf("%c",218);
                                    else
                                        printf("x");
                                }
                                else
                                {
                                    if(Nodefield[i][k]->Connection[DOWN] != NULL)
                                    {
                                        if(Nodefield[i][k]->Connection[LEFT] != NULL)
                                            printf("%c",191);
                                        else
                                            printf("x");
                                    }
                                    else
                                        printf("x");
                                }
                            }
                        }
                    }
                    if(Nodefield[i][k]-> x < k || Nodefield[i][k]->x > k)
                        printf("%c",196); // horizontal line
                    if(Nodefield[i][k]->y < i || Nodefield[i][k]->y > i)
                        printf("%c",179); //vertical line
                }
                else
                    printf(" ");
        printf("\n");
    }*/
}

void Maze::RawMaze::GenerateNodes(bool Print)
{
    if(coloration == NULL)
    {
        printf("Load a maze first!\n");
        return;
    }
    int i,k;
    bool active;

    //Initialize empty node-field
    Maze::Node*** Nodefield = new Maze::Node**[height];
    for(i=0;i<height;++i)
    {
        Nodefield[i] = new Maze::Node*[width];
        for(k=0;k<width;++k)
            Nodefield[i][k] = NULL;
    }

    for(i=1;i<height-1;++i)
    {
        active = true;
        for(k=1;k<width-1;++k)
        {
            switch(coloration[k][i])
            {
            case 0:
                //wall
                active = true;
                break;
            case 1:
                //passage
                if(active)
                {
                    if(coloration[k+1][i] == 0 && coloration[k][i-1] != 0 && coloration[k][i+1] != 0)
                    {
                        Nodefield[i][k] = Nodefield[i-1][k];
                    }
                    else
                    {
                        Nodefield[i][k] = new Maze::Node;
                        Nodefield[i][k]->x = k;
                        Nodefield[i][k]->y = i;
                        Nodefield[i][k]->visited = false;
                        Nodefield[i][k]->Connection[UP] = Nodefield[i-1][k];
                        Nodefield[i][k]->Connection[LEFT] = Nodefield[i][k-1];
                        if(Nodefield[i-1][k] != NULL)
                            Nodefield[i-1][k]->Connection[DOWN] = Nodefield[i][k];
                        if(Nodefield[i][k-1] != NULL)
                            Nodefield[i][k-1]->Connection[RIGHT] = Nodefield[i][k];
                        ++number;
                        active = false;
                    }
                }
                else
                {
                    if(coloration[k+1][i] == 0 || coloration[k][i-1] != 0 || coloration[k][i+1] != 0)
                    {
                        Nodefield[i][k] = new Maze::Node;
                        Nodefield[i][k]->x = k;
                        Nodefield[i][k]->y = i;
                        Nodefield[i][k]->visited = false;
                        Nodefield[i][k]->Connection[UP] = Nodefield[i-1][k];
                        Nodefield[i][k]->Connection[LEFT] = Nodefield[i][k-1];
                        if(Nodefield[i-1][k] != NULL)
                            Nodefield[i-1][k]->Connection[DOWN] = Nodefield[i][k];
                        if(Nodefield[i][k-1] != NULL)
                            Nodefield[i][k-1]->Connection[RIGHT] = Nodefield[i][k];
                        ++number;
                        active = false;
                    }
                    else
                    {
                        Nodefield[i][k] = Nodefield[i][k-1];
                    }
                }
                break;
            case 2:
                //start
                Start = new Maze::Node;
                Nodefield[i][k] = Start;
                Nodefield[i][k]->x = k;
                Nodefield[i][k]->y = i;
                Nodefield[i][k]->visited = false;
                Nodefield[i][k]->Connection[UP] = Nodefield[i-1][k];
                Nodefield[i][k]->Connection[LEFT] = Nodefield[i][k-1];
                if(Nodefield[i-1][k] != NULL)
                    Nodefield[i-1][k]->Connection[DOWN] = Nodefield[i][k];
                if(Nodefield[i][k-1] != NULL)
                    Nodefield[i][k-1]->Connection[RIGHT] = Nodefield[i][k];
                ++number;
                active = false;
                break;
            case 3:
                //end
                End = new Maze::Node;
                Nodefield[i][k] = End;
                Nodefield[i][k]->x = k;
                Nodefield[i][k]->y = i;
                Nodefield[i][k]->visited = false;
                Nodefield[i][k]->Connection[UP] = Nodefield[i-1][k];
                Nodefield[i][k]->Connection[LEFT] = Nodefield[i][k-1];
                if(Nodefield[i-1][k] != NULL)
                    Nodefield[i-1][k]->Connection[DOWN] = Nodefield[i][k];
                if(Nodefield[i][k-1] != NULL)
                    Nodefield[i][k-1]->Connection[RIGHT] = Nodefield[i][k];
                ++number;
                active = false;
                break;
            }
        }
    }
    //Check borders
    if(Start == NULL || End == NULL)
    {
        for(i=0;i<width;++i)
        {
            if(coloration[i][0] != 0)
            {
                Nodefield[0][i] = new Maze::Node;
                Nodefield[0][i]->x = i;
                Nodefield[0][i]->y = 0;
                Nodefield[0][i]->visited = false;
                Nodefield[0][i]->Connection[DOWN] = Nodefield[1][i];
                if(Nodefield[1][i] != NULL)
                    Nodefield[1][i]->Connection[UP] = Nodefield[0][i];
                ++number;
                coloration[i][0] == 2 ? Start = Nodefield[0][i] : End = Nodefield[0][i];
            }
            if(coloration[i][height-1] != 0)
            {
                Nodefield[height-1][i] = new Maze::Node;
                Nodefield[height-1][i]->x = i;
                Nodefield[height-1][i]->y = height-1;
                Nodefield[height-1][i]->visited = false;
                Nodefield[height-1][i]->Connection[UP] = Nodefield[height-2][i];
                if(Nodefield[height-2][i] != NULL)
                    Nodefield[height-2][i]->Connection[DOWN] = Nodefield[height-1][i];
                ++number;
                coloration[i][height-1] == 2 ? Start = Nodefield[height-1][i] : End = Nodefield[height-1][i];
            }
        }
        for(i=0;i<height;++i)
        {
            if(coloration[0][i] != 0)
            {
                Nodefield[i][0] = new Maze::Node;
                Nodefield[i][0]->x = 0;
                Nodefield[i][0]->y = i;
                Nodefield[i][0]->visited = false;
                Nodefield[i][0]->Connection[RIGHT] = Nodefield[i][1];
                if(Nodefield[i][1] != NULL)
                    Nodefield[i][1]->Connection[LEFT] = Nodefield[i][0];
                ++number;
                coloration[0][i] == 2 ? Start = Nodefield[i][0] : End = Nodefield[i][0];
            }
            if(coloration[width-1][i] != 0)
            {
                Nodefield[i][width-1] = new Maze::Node;
                Nodefield[i][width-1]->x = width-1;
                Nodefield[i][width-1]->y = i;
                Nodefield[i][width-1]->visited = false;
                Nodefield[i][width-1]->Connection[LEFT] = Nodefield[i][width-2];
                if(Nodefield[i][width-2] != NULL)
                    Nodefield[i][width-2]->Connection[RIGHT] = Nodefield[i][width-1];
                ++number;
                coloration[width-1][i] == 2 ? Start = Nodefield[i][width-1] : End = Nodefield[i][width-1];
            }
        }
    }

    if(Start == NULL || End == NULL)
        printf("Error; no start/end found!\n");
    printf("%i nodes created\n",number);

    if(Print)
    {
        PrintNodes();
    }

    NodeList = new Maze::Node*[number];

    int j;
    k = 0;

    for(i=0;i<number;++i)
        NodeList[i] = NULL;

    for(i=0;i<height;++i)
    {
        for(j=0;j<width;++j)
        {
            if(Nodefield[i][j] != NULL && !Nodefield[i][j]->visited)
            {
                NodeList[k++] = Nodefield[i][j];
                Nodefield[i][j]->visited = true;
                Nodefield[i][j] = NULL;
            }
        }
        delete[] Nodefield[i];
    }
    delete[] Nodefield;

    for(i=0;i<number;++i)
        NodeList[i]->visited = false;
}
void Maze::RawMaze::A_Star()
{
    if(number == 0)
        return;
    int* distance = new int[number];
    int x = End->x;
    int y = End->y;
    for(int i=0;i<number;++i)
        distance[i] = (NodeList[i]->x - x) * (NodeList[i]->x - x) + (NodeList[i]->y - y) * (NodeList[i]->y - y);
    Dijkstra(distance);
    delete distance;
}

void Maze::RawMaze::Dijkstra(int* test)
{
    if(Start == NULL || End == NULL)
    {
        printf("Can't look for a route if there's no start or exit\n");
        return;
    }
    //initializing variables
    int i,k,minimal,minimalindex;

    Maze::Node* Previous[number];
    int distance[number];
    for(i=0;i<number;++i)
    {
        Previous[i] = NULL;
        if(Start == NodeList[i])
        {
            distance[i] = 0;
            minimalindex = i;
        }
        else
            distance[i] = -1;
    }

    Solution = new Maze::SElement;
    Solution->point = End;
    Maze::Node* tempN = Start;
    printf("looking for exit");
    int counter = 0;
    while(tempN != End && tempN != NULL)
    {
        //Look for exit
        tempN->visited = true;
        i = 0;
        k = 0;
        //Find entries and modify length
        for(i=0;i<number;++i)
        {
            for(k=0;k<4;++k)
            {
                if(tempN->Connection[k] != NULL && NodeList[i] == tempN->Connection[k])
                    if(distance[i] == -1)
                    {
                        distance[i] = std::abs(tempN->x - tempN->Connection[k]->x) + std::abs(tempN->y - tempN->Connection[k]->y) + distance[minimalindex];
                        Previous[i] = tempN;
                        //printf("Set distance to node %i,%i to %i previous node is %i,%i\n",tempN->Connection[k]->x,tempN->Connection[k]->y,distance[i],Previous[i]->x,Previous[i]->y);
                    }
                    else
                    {
                        minimal = std::abs(tempN->x - tempN->Connection[k]->x) + std::abs(tempN->y - tempN->Connection[k]->y) + distance[minimalindex];
                        if(distance[i] > minimal)
                        {
                            distance[i] = minimal;
                            Previous[i] = tempN;
                            //printf("update distance to node %i,%i to %i previous node is %i,%i\n",tempN->Connection[k]->x,tempN->Connection[k]->y,distance[i],Previous[i]->x,Previous[i]->y);
                        }
                    }
            }
        }
        //look for smallest value without weighting distance
        if(test == NULL)
        {
            minimalindex = -1;
            for(i=0;i<number;++i)
            {
                if(distance[i] != -1 && !NodeList[i]->visited && (minimalindex == -1 || distance[minimalindex] > distance[i]))
                    minimalindex = i;
            }
        }
        else
        {
            minimalindex = -1;
            for(i=0;i<number;++i)
            {
                if(distance[i] != -1 && !NodeList[i]->visited && (minimalindex == -1 || distance[minimalindex] + test[minimalindex] > distance[i] + test[i]))
                    minimalindex = i;
            }
        }
        //repeat the same deal with new shortest route
        if(minimalindex == -1)
        {
            printf("something went wrong..\n");
            return;
        }
        tempN = NodeList[minimalindex];
        if(++counter % 100 == 0)
            printf(".");
    }
    if(tempN == End)
    {
        printf("\nfound end node, mapping return path\n");
        Maze::SElement* tempSE = Solution;
        length = -1;
        while(tempN != NULL)
        {
            //find index
            for(i = 0;i<number;++i)
            {
                if(tempN == NodeList[i])
                    break;
            }
            if(length == -1)
                length = distance[i];
            if(i == number)
                break;
            //Add previous to solution
            tempSE->next = new Maze::SElement;
            tempSE = tempSE->next;
            tempSE->point = Previous[i];
            tempN = Previous[i];
        }
        printf("Solution found after %i steps\npath is %i pixels long!\n",counter,length);
        delete[] NodeList;
    }
}

void Maze::RawMaze::Simplify()
{
    if(Start == NULL || End == NULL)
    {
        printf("Can't look for a route if there's no start or exit\n");
        return;
    }
    printf("removing dead ends");
    //initializing variables
    int i,k,j,connectioncount, loopcount = 0,removecount = 0;
    bool change = true;

    while(change)
    {
        change = false;
        for(i=0;i<number;++i)
        {
            if(NodeList[i] != NULL)
            {
                connectioncount = 0;
                for(k=0;k<4;++k)
                    if(NodeList[i]->Connection[k] != NULL)
                        ++connectioncount;
                //find and remove dead ends
                if(connectioncount == 1)
                {
                    if(NodeList[i] != Start && NodeList[i] != End)
                    {
                        //find sole connection
                        for(k=0;k<4;++k)
                            if(NodeList[i]->Connection[k] != NULL)
                                break;
                        //Remove connection
                        for(j=0;j<4;++j)
                            if(NodeList[i]->Connection[k]->Connection[j] == NodeList[i])
                            {
                                NodeList[i]->Connection[k]->Connection[j] = NULL;
                                break;
                            }
                        //printf("delete %i, %i - remove connection from %i,%i\n",NodeList[i]->x,NodeList[i]->y,NodeList[i]->Connection[k]->x,NodeList[i]->Connection[k]->y);
                        change = true;
                        delete NodeList[i];
                        NodeList[i] = NULL;
                        ++removecount;
                    }
                }
            }
        }
        if(++loopcount % 100 == 0)
            printf(".");
    }
    printf("\nremoved %i in %i steps\n",removecount,loopcount);
    //Now only a single path to the exit should exist
    Solution = new Maze::SElement;
    Maze::Node*     tempN;
    Maze::Node*     lastN;
    Maze::SElement* tempSE = Solution;

    tempSE->point = End;
    for(i=0;i<4;++i)
        if(End->Connection[i] != NULL)
            break;
    lastN = End;
    tempN = End->Connection[i];
    tempSE->next = new Maze::SElement;
    tempSE = tempSE->next;
    length = 0;
    while(tempN != NULL)
    {
        tempSE->point = tempN;
        length += std::abs(tempN->x - lastN->x) + std::abs(tempN->y-lastN->y);
        for(i=0;i<4;++i)
            if(tempN->Connection[i] != NULL && tempN->Connection[i] != lastN)
                break;
        if(i == 4)
            break;
        lastN = tempN;
        tempN = tempN->Connection[i];
        tempSE->next = new Maze::SElement;
        tempSE = tempSE->next;
    }
    delete[] NodeList;
    NodeList = NULL;
}

void Maze::RawMaze::DrawSolution()
{
    if(Solution == NULL)
    {
        printf("No solution found!\n");
        return;
    }
    Maze::SElement* tempSE = Solution;
    int x1,y1,x2,y2,l = 0;
    while(tempSE != NULL)
    {
        x1 = tempSE->point->x;
        y1 = tempSE->point->y;
        tempSE = tempSE->next;
        if(tempSE == NULL || tempSE->point == NULL)
            break;
        x2 = tempSE->point->x;
        y2 = tempSE->point->y;
        //printf("Mapping from %i,%i to %i,%i\n",x1,y1,x2,y2);
        if(x1 == x2)
        {
            while(y1 < y2)
                coloration[x1][y1++] = (l++*1.0/length*1.0)*255.0 + 5;
            while(y1 > y2)
                coloration[x1][y1--] = (l++*1.0/length*1.0)*255.0 + 5;
        }
        else
        {
            while(x1 < x2)
                coloration[x1++][y1] = (l++*1.0/length*1.0)*255.0 + 5;
            while(x1 > x2)
                coloration[x1--][y1] = (l++*1.0/length*1.0)*255.0 + 5;
        }
    }
    coloration[Start->x][Start->y] = 260;
}

void Maze::RawMaze::loadFromImage(std::string filepath)
{
    coloration = img::loadImage(filepath,width,height);
}

void Maze::RawMaze::GenerateMaze(int w,int h, int Start_x, int Start_y, int End_x, int End_y)
{
    width = w;
    height = h;
    //do something intelligent
}


void Maze::RawMaze::ExportMaze()
{
    img::writeImage(coloration,width,height);
}

void Maze::RawMaze::ExportMazeStream()
{
    img::writeImageStream(coloration,width,height);
}
