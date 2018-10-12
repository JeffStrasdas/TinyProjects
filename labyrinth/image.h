#ifndef IMAGE_H
#define IMAGE_H

#include <string>

namespace img
{
void writeImage(int** tile,int w, int h);
void writeImageStream(int** tile,int w, int h);
int** loadImage(std::string filename,int& width, int& height);

class Window
{
public:
    Window(int width, int height):w(width),h(height){};
private:
    int w,h;
};
}

#endif
