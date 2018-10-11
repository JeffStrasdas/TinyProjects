#include <cstdio>

#include "calc.h"
#include "FileOP.h"

bool calcpile(int** pile,int x,int y)
{
    bool done = true;
    int i,k;
    int** tile = new int*[x];
    for(i=0;i<x;++i)
    {
        tile[i] = new int[y];
        for(k=0;k<y;++k)
            tile[i][k] = pile[i][k];
    }
    for(i=0;i<x;++i)
    {
        for(k=0;k<y;++k)
        {
            if(pile[i][k] >3)
            {
                tile[i][k] -= 4;
                if(tile[i][k] > 3)
                    done = false;
                if(i != 0)
                {
                    tile[i-1][k] += 1;
                    if(tile[i-1][k] > 3)
                        done = false;
                }
                if(i != x-1)
                {
                    tile[i+1][k] += 1;
                    if(tile[i+1][k] > 3)
                        done = false;
                }
                if(k != 0)
                {
                    tile[i][k-1] += 1;
                    if(tile[i][k-1] > 3)
                        done = false;
                }
                if(k != y-1)
                {
                    tile[i][k+1] += 1;
                    if(tile[i][k+1] > 3)
                        done = false;
                }
            }
        }
    }
    for(i=0;i<x;++i)
    {
        for(k=0;k<y;++k)
            pile[i][k] = tile[i][k];
        delete tile[i];
    }
    delete tile;
    return done;
}
bool calcpile2(int** tile, int x, int y)
{
    bool done = true;
    int i,k,temp;
    for(i=0;i<x;++i)
        for(k=0;k<y;++k)
            if(tile[i][k] > 3)
            {
                temp = tile[i][k]/4;
                if(i != 0)
                {
                    tile[i-1][k] += temp;
                    if(tile[i-1][k] > 3)
                        done = false;
                }
                if(i != x-1)
                {
                    tile[i+1][k] += temp;
                    if(tile[i+1][k] > 3)
                        done = false;
                }
                if(k != 0)
                {
                    tile[i][k-1] += temp;
                    if(tile[i][k-1] > 3)
                        done = false;
                }
                if(k != y-1)
                {
                    tile[i][k+1] += temp;
                    if(tile[i][k+1] > 3)
                        done = false;
                }
                tile[i][k] %= 4;
            }
    return done;
}
bool calcpile3(int** tile, int x)
{
    int counter = 0;
    int minX = x/2 - 1;
    int maxX = x/2 + 1;
    int i,k,temp;
    bool done;
    while(!(minX == 1 && maxX == x-1))
    {
        done = true;
        for(i = minX;i<maxX;++i)
            for(k=minX;k<maxX;++k)
            {
                if(tile[i][k] > 3)
                {
                    temp = tile[i][k]/4;
                    tile[i+1][k] += temp;
                    tile[i-1][k] += temp;
                    tile[i][k+1] += temp;
                    tile[i][k-1] += temp;

                    tile[i][k] %= 4;
                    done = false;
                }
            }
        if(done)
        {
            --minX;
            ++maxX;
            if(minX < 1)
                minX = 1;
            if(maxX > x-1)
                maxX = x-1;
        }
        if(++counter%100 == 0)
        {
            printf(".");
            if(counter%1000000 == 0)
            {
                printf("printing an image");
                writeImage(tile,x,x,3);
            }
        }
    }
    printf("\n\ncalculated in %i steps\n",counter);
    return true;
}
bool calcpile4(int StartingAmmount,int length, bool image, bool text, int** square, int* sidebar, int Root)
{
    bool done = true;
    int counter = 0;
    int i,k;
    Root += StartingAmmount;
    if(sidebar == NULL)
    {
        sidebar = new int[length];
        done = false;
    }
    if(square == NULL)
    {
        square = new int*[length];
        done = false;
    }
    if(!done)
        for(i=0;i<length;++i)
        {
            sidebar[i] = 0;
            square[i] = new int[length];
            for(k=0;k<length;++k)
                square[i][k] = 0;
        }
    int w = 1;
    int temp;
    while(true)
    {
        done = true;
        //Check Root
        if(Root>3)
        {
            done = false;
            sidebar[0] += Root/4;
            Root %= 4;
        }
        for(i=0;i<w;++i)
        {
            //Check sidebar
            if(sidebar[i] > 3)
            {
                done = false;
                temp = sidebar[i]/4;
                sidebar[i+1] += temp;
                square[i][0] += temp;
                square[0][i] += temp;
                if(i==0)
                    Root += sidebar[0] - sidebar[0]%4;
                else
                    sidebar[i-1] += temp;
                sidebar[i] %= 4;
            }
            //Check square
            for(k=0;k<w;++k)
            {
                if(square[i][k] > 3)
                {
                    done = false;
                    temp = square[i][k]/4;
                    square[i+1][k] += temp;
                    square[i][k+1] += temp;
                    if(k == 0)
                        sidebar[i] += temp;
                    else
                        square[i][k-1] += temp;
                    if(i != 0)
                        square[i-1][k] += temp;
                    else
                        sidebar[k] += temp;
                    square[i][k] %= 4;
                }
            }
        }
        if(++counter % 100 == 0)
        {
            if(counter % 1000000 == 0)
            {
                printf("Quick-saving current setup");
                writeToFileCompresed(square,sidebar,Root,length);
            }
            else
                printf(".");

        }
        if(done)
        {
            if(w++ >= length-1)
                break;
        }
    }

    if(text)
        writeToFileCompresed(square,sidebar,Root,length);
    //transporting to the outside...
    if(image)
        writeImageCompressed(square,sidebar,Root,length,3);

    for(i=0;i<length;++i)
    {
        delete[] square[i];
    }
    return true;
}

bool calcpile5(int StartingAmmount, int length, bool image, bool text, int** triangle, int* sidebar, int Root)
{
    int counter = 0;
    bool done = true;
    int i,k, temp;
    //Generate new
    Root += StartingAmmount;
    if(sidebar == NULL)
    {
        sidebar = new int[length];
        done = false;
    }
    if(triangle == NULL)
    {
        triangle = new int*[length];
        done = false;
    }
    //Fill arrays
    if(!done)
        for(i=0;i<length;++i)
        {
            sidebar[i] = 0;
            triangle[i] = new int[i+1];
            for(k=0;k<=i;++k)
                triangle[i][k] = 0;
        }
    //actually calculate
    int w = 1;
    while(true)
    {
        done = true;
        //Check Root
        if(Root>3)
        {
            done = false;
            sidebar[0] += Root/4;
            Root %= 4;
        }
        for(i=0;i<w;++i)
        {
            //Check sidebar
            if(sidebar[i] > 3)
            {
                done = false;
                temp = sidebar[i]/4;
                sidebar[i+1] += temp;
                if(i==0)
                {
                    Root += sidebar[0] - sidebar[0]%4;
                    triangle[0][0] += 2*temp;
                }
                else
                {
                    sidebar[i-1] += temp;
                    triangle[i][0] += temp;
                }
                sidebar[i] %= 4;
            }
            //Check inner triangle
            for(k=0;k<i;++k)
            {
                if(triangle[i][k] > 3)
                {
                    done = false;
                    temp = triangle[i][k]/4;
                    triangle[i+1][k] += temp;
                    if(k==0) //bordering sidebar
                    {
                        sidebar[i] += 2*temp;
                    }
                    else
                    {
                        triangle[i][k-1] += temp;
                    }
                    if(k==i-1) // practically diagonal
                    {
                        triangle[k][k] += 2*temp;
                        triangle[i][i] += 2*temp;
                    }
                    else
                    {
                        triangle[i-1][k] += temp;
                        triangle[i][k+1] += temp;
                    }
                    triangle[i][k] %= 4;
                }
            }
            //Check diagonal
            if(triangle[i][i] > 3)
            {
                done = false;
                temp = triangle[i][i] /4;
                triangle[i+1][i] += temp;
                if(i==0)
                {
                    sidebar[0] += 2*temp;
                }
                else
                    triangle[i][i-1] += temp;
                triangle[i][i] %= 4;
            }
        }
        if(++counter % 100 == 0)
        {
            if(counter % 5000000 == 0)
            {
                printf("Quick-saving current setup");
                writeToFileCompresed2(triangle,sidebar,Root,length);
            }
            else
                printf(".");

        }
        if(done)
        {
            if(w++ >= length-1)
                break;
        }
    }
    if(text)
        writeToFileCompresed2(triangle,sidebar,Root,length);
    //calculation done; creating image
    if(image)
    {
        int** square = new int*[length];
        for(i=0;i<length;++i)
        {
            square[i] = new int[length];
            for(k=0;k<i;++k)
            {
                square[i][k] = triangle[i][k];
                square[k][i] = triangle[i][k];
            }
            square[i][i] = triangle[i][i];
        }
        writeImageCompressed(square,sidebar,Root,length,3);
        for(i=0;i<length;++i)
        {
            delete[] square[i];
            delete[] triangle[i];
        }
        delete[] square;
    }
    else
        for(i=0;i<length;++i)
            delete[] triangle[i];
    delete[] sidebar;
    delete[] triangle;
    return true;
}



