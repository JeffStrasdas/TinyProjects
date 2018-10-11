#include "image.h"

#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <string>

void img::writeImageStream(int** tile, int w, int h)
{
    int filesize = 54 + 3*w*h;
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char padding[4-(w*3)%4];
    for(int i=0;i<4-(w*3)%4;++i)
        padding[i] = 0;
    int r,g,b;

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);
    printf("bla\n");
    std::string name = "";
    for(int i=0;i<10;++i)
        name += rand()%26 + 65 + rand()%2 * 32;
    printf("2222\n");
    std::string opath = "i/" + name + ".bmp";
    printf("saving image as %s...\n",name.c_str());

    std::ofstream ofile (opath.c_str());
    if(ofile.is_open())
    {
        ofile << bmpfileheader << bmpinfoheader;
        for(int i=0; i<h; i++)
        {
            for(int k=0;k<w;++k)
            {
                switch(tile[k][i])
                {
                case 0:
                    r = g = b = 0;
                    break;
                case 1:
                    r = g = b = 255;
                    break;
                case 2:
                    r = 255;
                    g = b = 0;
                    break;
                case 3:
                    b = 255;
                    g = r = 0;
                    break;
                default:
                    r = tile[k][i] - 5;
                    b = 255 - r;
                    g = 0;
                    break;
                }
                if(r > 255)
                    r = 255;
                if(g > 255)
                    g = 255;
                if(b > 255)
                    b = 255;
                ofile << (unsigned char)(b) << (unsigned char)(g) << (unsigned char)(r);
            }
            //padding
            ofile << padding;
        }
        ofile.close();
    }

}

void img::writeImage(int** tile,int w, int h)
{
    int r,g,b;
    int current;
    FILE *f;
    unsigned char *img = NULL;
    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
    //if( img )
    //    free( img );
    //img = (unsigned char *)malloc(3*w*h);
    try{
    img = new unsigned char[3*w*h];
    }
    catch(std::exception& e)
    {
        printf("can't allocate character array with dimensions %i, %i\n",w,h);
        return;
    }
    memset(img,0,sizeof(img));

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            switch(tile[i][j])
            {
            case 0:
                r = g = b = 0;
                break;
            case 1:
                r = g = b = 255;
                break;
            default:
                r = tile[i][j]-5;
                b = 255 - r;
                g = 0;
                break;
            }

            if (r > 255) r=255;
            if (g > 255) g=255;
            if (b > 255) b=255;
            img[(i+j*w)*3+2] = (unsigned char)(r);
            img[(i+j*w)*3+1] = (unsigned char)(g);
            img[(i+j*w)*3+0] = (unsigned char)(b);
        }
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);
    char name[15];
    for(int i=2;i<11;++i)
        name[i] = rand()%26 + 65 + rand()%2 * 32;
    name[0] = 'i'; name[1] = '/'; name[11] = '.'; name[12] = 'b'; name[13] = 'm'; name[14] = 'p';
    printf("saving image as %s...\n",name);

    f = fopen(name,"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<h; i++)
    {
        fwrite(img+(w*(h-i-1)*3),3,w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }
    fclose(f);
}

int** img::loadImage(std::string filename,int& width, int& height)
{
    FILE* f = fopen(filename.c_str(), "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    //int filesize = *(int*)&info[2];
    //int dataOffset = *(int*)(&info[10]);
    width = *(int*)&info[18];
    height = *(int*)&info[22];
    //int depth = *(int*)&info[28];
    if(*(int*)&info[28] != 24 || *(int*)(&info[10]) != 54)
    {
        printf("Invalid image formatting -- use 24-bit and minimal bmp header\n");
        width = height = 0;
        return NULL;
    }
    int row_length = ((width * 3 + 3) & (~3));
    //int fs2 = (width*depth/8 + ((4-(width*depth/8)%4)%4))*height;
    printf("Loading file %s..\nwidth: %i\nheight: %i\n",filename.c_str(),width,height);

    int i,j,k;
    int r,g,b;
    int** Values = new int*[width];
    for(i = 0; i<width;++i)
    {
        Values[i] = new int[height];
        for(j = 0; j<height;++j)
            Values[i][j] = 0;
    }

    unsigned char data[row_length];

    for(i=0;i<height;++i)
    {
        fread(data, sizeof(unsigned char), row_length, f);
        for(j=0;j<width;++j)
        {
            k = j*3;

            r = data[k+2];
            g = data[k+1];
            b = data[k];

            if(r == b)
                if(r == 0)
                    Values[j][height-1-i] = 0; //Wall
                else
                    Values[j][height-1-i] = 1; //Passage
            else
                if(r == 255)
                    Values[j][height-1-i] = 2; //Start
                else
                    Values[j][height-1-i] = 3; //End
        }
    }

    //printf("filesize = %i, offset = %i\nfs1 = %i\n",filesize,dataOffset,fs1+dataOffset);
    //printf("read additional header, formatting: (B,G,R)\n");
    //unsigned char tempC[dataOffset-54];
    //fread(tempC, sizeof(unsigned char), dataOffset-54, f);
    //for(int i=0;i<dataOffset-54;++i)
    //    printf("%s ",tempC[i] == 0 ? "  0":"255");
    //printf("\nReading a few pixels:\n");
    fclose(f);
    return Values;
}
