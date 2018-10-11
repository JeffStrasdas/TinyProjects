#ifndef CALC_H
#define CALC_H

bool calcpile(int** pile,int x,int y);
bool calcpile2(int** tile, int x, int y);
bool calcpile3(int** tile, int x);
bool calcpile4(int StartingAmmount, int length, bool image = true, bool text = true, int** square = NULL, int* sidebar = NULL, int Root = 0);
bool calcpile5(int StartingAmmount, int length, bool image = true, bool text = true, int** triangle = NULL, int* sidebar = NULL, int Root = 0);

#endif
