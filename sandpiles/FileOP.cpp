#include <windows.h>
#include <cstdlib>
#include <fstream>


#include "FileOP.h"

void writeImage(int** tile,int w, int h,float maxpertile)
{
    int r,g,b;
    float current;
    FILE *f;
    unsigned char *img = NULL;
    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
    if( img )
        free( img );
    img = (unsigned char *)malloc(3*w*h);
    memset(img,0,sizeof(img));

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            //x=i; y=(h-1)-j;
            //r = red[i][j]*255;
            //g = green[i][j]*255;
            //b = blue[i][j]*255;
            /*switch(tile[i][j])
            {
            case 0:
                r = 255;
                g = 255;
                b = 255;
                break;
            case 1:
                r = 255;
                g = 204;//204;
                b = 0;
                break;
            case 2:
                r = 255;//255;
                g = 102;//102;
                b = 0;
                break;
            case 3:
                r = 255;//255;
                g = 0;
                b = 0;//0;
                break;
            default:
                r = 0;
                g = 0;
                b = 0;
                break;
            }*/

            current = tile[i][j];
            if(current/maxpertile > 1)
                printf("ERROR - tile %i,%i contains %i!\n",i,j,(int)current);

            r = 255*(1.0-current/maxpertile);
            g = r;
            b = r;

            if (r > 255) r=255;
            if (g > 255) g=255;
            if (b > 255) b=255;
            img[(i+((h-1)-j)*w)*3+2] = (unsigned char)(r);
            img[(i+((h-1)-j)*w)*3+1] = (unsigned char)(g);
            img[(i+((h-1)-j)*w)*3+0] = (unsigned char)(b);
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

void writeImageCompressed(int** square,int* sidebar, int Root, int length, float maxpertile)
{
    int r,g,b;
    int totallength = 2*length+1;
    float current;
    FILE *f;
    unsigned char *img = NULL;
    int filesize = 54 + 3*totallength*totallength;  //w is your image width, h is image height, both int
    if( img )
        free( img );
    img = (unsigned char *)malloc(3*totallength*totallength);
    memset(img,0,sizeof(img));

    for(int i=0; i<totallength; i++)
    {
        for(int k=0; k<totallength; k++)
        {
            if(i<length)
            {
                if(k<length)
                    current = square[length-1-i][length-1-k];
                else if(k==length)
                    current = sidebar[length-1-i];
                else
                    current = square[length-1-i][k-length-1];
            }
            else if(i==length)
            {
                if(k<length)
                    current = sidebar[length-1-k];
                else if(k==length)
                    current = Root;
                else
                    current = sidebar[k-length-1];
            }
            else
            {
                if(k<length)
                    current = square[i-length-1][length-1-k];
                else if(k==length)
                    current = sidebar[i-length-1];
                else
                    current = square[i-length-1][k-length-1];
            }
            if(current/maxpertile > 1)
                printf("ERROR - tile %i,%i contains %i!\n",i,k,current);

            r = 255*(1.0-current/maxpertile);
            g = r;
            b = r;

            if (r > 255) r=255;
            if (g > 255) g=255;
            if (b > 255) b=255;
            img[(i+((totallength-1)-k)*totallength)*3+2] = (unsigned char)(r);
            img[(i+((totallength-1)-k)*totallength)*3+1] = (unsigned char)(g);
            img[(i+((totallength-1)-k)*totallength)*3+0] = (unsigned char)(b);
        }
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       totallength    );
    bmpinfoheader[ 5] = (unsigned char)(       totallength>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       totallength>>16);
    bmpinfoheader[ 7] = (unsigned char)(       totallength>>24);
    bmpinfoheader[ 8] = (unsigned char)(       totallength    );
    bmpinfoheader[ 9] = (unsigned char)(       totallength>> 8);
    bmpinfoheader[10] = (unsigned char)(       totallength>>16);
    bmpinfoheader[11] = (unsigned char)(       totallength>>24);
    char name[15];
    for(int i=2;i<11;++i)
        name[i] = rand()%26 + 65 + rand()%2 * 32;
    name[0] = 'i'; name[1] = '/'; name[11] = '.'; name[12] = 'b'; name[13] = 'm'; name[14] = 'p';
    printf("saving image as %s...\n",name);

    f = fopen(name,"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<totallength; i++)
    {
        fwrite(img+(totallength*(totallength-i-1)*3),3,totallength,f);
        fwrite(bmppad,1,(4-(totallength*3)%4)%4,f);
    }
    fclose(f);
}

void writeToFile(int** square,int* sidebar,int Root, int length)
{
    int i,k;

    std::string name = "";
    for(int i=0;i<10;++i)
        name += rand()%26 + 65 + rand()%2 * 32;
    std::string opath = "t/" + name + ".txt";
    printf("Saving grid as %s...\n",opath.c_str());
    std::ofstream ofile (opath.c_str());
    if(ofile.is_open())
    {
        ofile << length << std::endl;
        for(i=0;i<2*length+1;++i)
        {
            if(i < length)
            {
                for(k=0;k<length;++k)
                {
                    ofile << square[length-1-i][length-1-k] << " ";
                }
                ofile << sidebar[length-i-1] << " ";
                for(k=0;k<length;++k)
                    ofile << square[length-1-i][k] << " ";
            }
            else
            if(i==length)
            {
                for(k=0;k<length;++k)
                {
                    ofile << sidebar[length-1-k] << " ";
                }
                ofile << Root << " ";
                for(k=0;k<length;++k)
                    ofile << sidebar[k] << " ";
            }
            else
            {
                for(k=0;k<length;++k)
                {
                    ofile << square[i-length-1][length-1-k] << " ";
                }
                ofile << sidebar[i-length-1] << " ";
                for(k=0;k<length;++k)
                    ofile << square[i-length-1][k] << " ";
            }
            ofile << std::endl;
        }
        ofile.close();
    }
}

void writeToFileCompresed(int** square,int* sidebar,int Root, int length, int buffer)
{
    //Check buffer
    int i = length,k = 0;
    while(sidebar[--i] == 0)
        ++k;

    buffer -= k; // in- or decrease size

    std::string name = "";
    for(int i=0;i<10;++i)
        name += rand()%26 + 65 + rand()%2 * 32;
    std::string opath = "t/" + name + ".txt";
    printf("Saving grid as %s...\n",opath.c_str());
    std::ofstream ofile (opath.c_str());
    if(ofile.is_open())
    {
        ofile << length + buffer << std::endl;
        ofile << Root << std::endl;
        for(i = 0;i<length+buffer;++i)
            if(i < length)
                ofile << sidebar[i] << " ";
            else
                ofile << 0 << " ";
        ofile << std::endl;
        for(i=0;i<length+buffer;++i)
        {
            for(k=0;k<length+buffer;++k)
                if(i < length && k < length)
                    ofile << square[i][k] << " ";
                else
                    ofile << 0 << " ";
            ofile << std::endl;
        }
        ofile.close();
    }
}

void writeToFileCompresed2(int** triangle,int* sidebar,int Root, int length, int buffer)
{
    //Check buffer
    int i = length,k = 0;
    while(sidebar[--i] == 0)
        ++k;
    buffer -= k; //in- or decrease size
    std::string name = "";
    for(int i=0;i<10;++i)
        name += rand()%26 + 65 + rand()%2 * 32;
    std::string opath = "t/" + name + ".txt";
    printf("Saving grid as %s...\n",opath.c_str());
    std::ofstream ofile (opath.c_str());
    if(ofile.is_open())
    {
        ofile << length + buffer << std::endl;
        ofile << Root << std::endl;
        for(i = 0;i<length+buffer;++i)
            if(i < length)
                ofile << sidebar[i] << " ";
            else
                ofile << 0 << " ";
        ofile << std::endl;
        for(i=0;i<length+buffer;++i)
        {
            for(k=0;k<=i;++k)
                if(i < length)
                    ofile << triangle[i][k] << " ";
                else
                    ofile << 0 << " ";
            ofile << std::endl;
        }
        ofile.close();
    }
}

void printpile(int** pile,int x,int y,bool numbers)
{
    printf("\n");
    int i,k;
    for(i=0;i<x;++i)
    {
        for(k=0;k<y;++k)
            if(numbers)
                printf("%i ",pile[i][k]);
            else
                switch(pile[i][k])
                {
                case 0:
                    printf("%c%c",176,176);
                    break;
                case 1:
                    printf("%c%c",177,177);
                    break;
                case 2:
                    printf("%c%c",178,178);
                    break;
                case 3:
                    printf("%c%c",219,219);
                    break;
                default:
                    printf("XX");
                    break;
                }
    printf("\n");
    }
}

void loadFromText(int**& square,int*& sidebar, int& Root, int& length, std::string filepath)
{
    int i,k;

    std::ifstream ifile (filepath.c_str());
    if(ifile.is_open())
    {
        ifile >> length;
        ifile >> Root;
        sidebar = new int[length];
        for(i=0;i<length;++i)
            ifile >> sidebar[i];

        square = new int*[length];
        for(i=0;i<length;++i)
        {
            square[i] = new int[length];
            for(k=0;k<length;++k)
                ifile >> square[i][k];
        }
        ifile.close();
        printf("loading successful!\n");
    }
    else
        printf("Can't open file %s!\n",filepath.c_str());
}

void loadFromText2(int**& triangle,int*& sidebar, int& Root, int& length, std::string filepath, int buffer)
{
    int i,k;

    std::ifstream ifile (filepath.c_str());
    if(ifile.is_open())
    {
        ifile >> length;
        ifile >> Root;
        sidebar = new int[length+buffer];
        for(i=0;i<length+buffer;++i)
            if(i<length)
                ifile >> sidebar[i];
            else
                sidebar[i] = 0;

        triangle = new int*[length+buffer];
        for(i=0;i<length+buffer;++i)
        {
            triangle[i] = new int[i+1];
            for(k=0;k<=i;++k)
                if(i<length)
                    ifile >> triangle[i][k];
                else
                    triangle[i][k] = 0;
        }
        length += buffer;
        ifile.close();
        printf("loading successful!\n");
    }
    else
        printf("Can't open file %s!\n",filepath.c_str());
}
