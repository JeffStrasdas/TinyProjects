#ifndef FILEOP_H
#define FILEOP_H

#include <string>

void writeImage(int** tile,int w, int h,float maxpertile);
void writeImageCompressed(int** square,int* sidebar, int Root, int length, float maxpertile);
//void writeImageCompressed2(int** square,int* sidebar, int Root, int length, float maxpertile);
//void writeImageFromFile(std::string filepath); idea for later

void writeToFile(int** square,int* sidebar,int Root, int length);
void writeToFileCompresed(int** square,int* sidebar,int Root, int length, int buffer = 2);
void writeToFileCompresed2(int** triangle,int* sidebar,int Root, int length, int buffer = 2);

void loadFromText(int**& square, int*& sidebar, int& Root, int& length, std::string filepath);
void loadFromText2(int**& triangle, int*& sidebar, int& Root, int& length, std::string filepath, int buffer = 0);

void printpile(int** pile,int x,int y,bool numbers = false);
#endif
