#include <cstdlib>
#include <time.h>

#include "FileOP.h"
#include "calc.h"

#include <string>
#include <cstdio>

int main()
{
    std::srand(time(NULL));
    int starting_ammount = 4000000;
    //int width = (1599-1)/2;
    int** triangle;
    int* sidebar;
    int Root;
    int length;

    loadFromText2(triangle,sidebar,Root,length,"t/4000000.txt",400);
    //calcpile4(starting_ammount,length);
    calcpile5(starting_ammount,length,true,true,triangle,sidebar,Root);
//    calcpile5(starting_ammount,width);
}
