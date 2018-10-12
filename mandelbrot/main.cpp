#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<cmath>
#include<complex>

/*	Calculates and prints an image
//	border	determines min_x,max_x,min_y,max_y
//	DIM		determines image resolution by width,height
//	x,y		determines the exact position that should be calculated
*/
int* GeneratePixel(float* border,int* DIM, double x, double y)
{
    int r = 0,g = 0,b = 0;
    //r = (int)(510*(x*x+y*y))%255;
    //r = std::abs((int)(255*(x*x+y*y))%510 - 255);
    //r = (int)(255.0*(x-y))%255;
    //b = (int)(255.0*x)%255;
    //g = (int)(255.0*y)%255;
    //setting up midpoint; maximal bounds
    int maxIterations = 100;

    double bounds = 2.5;
    std::complex<double> z = 0, c = x + y * 1i;
    int i = 0;
    for(i=0;i < maxIterations;i++)
        if(std::abs(z) > bounds)
            break;
        else
            z = z*z+c;
    if(i!=maxIterations)
    {
        r = i %  8 * 32;
        g = i % 16 * 16;
        b = i % 32 *  8;
    }
    //Show axis?
    if(false)
    {
        float epsilonx = (border[1]-border[0])/(float)DIM[0];
        float epsilony = (border[3]-border[2])/(float)DIM[1];
        if(std::abs(x-(border[0]+border[1])/2.0)<epsilonx || std::abs(y-(border[2]+border[3])/2.0)<epsilony)
            r = g = b = 0;
    }
    int rgb[3] = {r,g,b};
    return rgb;
}

/*	Calculates and prints an image
//	border	 determines min_x,max_x,min_y,max_y
//	DIM		 determines image resolution by width,height
//	FILENAME determines path/filename
*/
void printImage(float* border,int* DIM, std::string FILENAME)
{
    //Initialise Filestream
    FILE *f;
    unsigned char* image;
    image = (unsigned char *)std::malloc(3*DIM[0]*DIM[1]);
    std::memset(image,0,sizeof(image));

    for(int w = 0; w<DIM[0];w++)
        for(int h=0;h<DIM[1];h++)
        {
            //Calculate Pixel
            double x = border[0] + ((double)w/(double)(DIM[0]-1))*(border[1]-border[0]);
            double y = border[2] + ((double)h/(double)(DIM[1]-1))*(border[3]-border[2]);
            int* RGB = GeneratePixel(border,DIM,x,y);

            //Save Pixel
            image[(w+h*DIM[0])*3+2] = RGB[0]; // red
            image[(w+h*DIM[0])*3+1] = RGB[1]; // green
            image[(w+h*DIM[0])*3+0] = RGB[2]; // blue
        }
    //write File
    f = fopen(FILENAME.c_str(),"wb");
    for(int i=0; i<DIM[1]; i++)
    {
        fwrite(image+(DIM[0]*(DIM[1]-i-1)*3),3,DIM[0],f);
        //fwrite(bmppad,1,(4-(DIM[0]*3)%4)%4,f);
    }
    fclose(f);
}

/* Zooms from [border] to [newborder] in [Amount] steps
//	border		determines min_x,max_x,min_y,max_y		of start image
//	newborder	determines min_x,max_x,min_y,max_y	of final image
//	DIM			determines width,height
//	Amount		how many images are generated
*/
void ZoomTo(float* border,float* newborder,int* DIM,int Amount)
{
    switch (Amount)
    {
    case 1:
        printImage(border,DIM,"test/0.raw");
        printf("Done!\n");
        break;
    case 2:
        printImage(border,DIM,"test/0.raw");
        printf("Done with image 0!\n");
        printImage(newborder,DIM,"test/1.raw");
        printf("Done with image 1!\n");
        break;
    default:
        if(Amount < 1)
        {
            printf("Error; %i <1 !",Amount);
            break;
        }
        float P[3]    = {(border[0]+border[1])/2.0,(border[2]+border[3])/2.0,(border[1]-border[0])/2.0};
        float newP[3] = {(newborder[0]+newborder[1])/2.0,(newborder[2]+newborder[3])/2.0,(newborder[1]-newborder[0])/2.0};
        for(int i = 0; i<Amount;i++)
        {
            /*float tempborder[4] = {
                border[0]+(float)i/(float)(Amount-1)*(newborder[0]-border[0]),
                border[1]+(float)i/(float)(Amount-1)*(newborder[1]-border[1]),
                border[2]+(float)i/(float)(Amount-1)*(newborder[2]-border[2]),
                border[3]+(float)i/(float)(Amount-1)*(newborder[3]-border[3]),
            };*/
            float tempP[3] = {
                P[0]+(float)i/(float)(Amount-1)*(newP[0]-P[0]),
                P[1]+(float)i/(float)(Amount-1)*(newP[1]-P[1]),
                P[2]+(float)i/(float)(Amount-1)*(newP[2]-P[2])};
            float tempborder[4] = {
                tempP[0]-tempP[2],tempP[0]+tempP[2],
                tempP[1]-tempP[2],tempP[1]+tempP[2],
            };

            char buffer[Amount/10];
            itoa(i,buffer,10);
            std::string Filename = std::string(buffer)+".raw";
            printImage(tempborder,DIM,Filename);
            printf("Done with image %i!\n",i);
        }
        break;
    }

}

int main()
{
    char str[20];
    float p[3];
    int Amount;
    printf("Use defaults? (y/n)\n");
    scanf("%s",str);
    if(str[0] == 'y' || str[0] == 'Y' || str[0] == 'j' || str[0] == 'J' || str[0] == '%c')
    {
        p[0]    = -0.904;
        p[1]    =  0.2485;
        p[2]    =  0.002;
        Amount =  2;
    }
    else
    {
        printf("X-coordinate to zoom into: ");
        scanf("%f",&p[0]);
        printf("Y-coordinate to zoom into: ");
        scanf("%f",&p[1]);
        printf("height/width of final zoom: ");
        scanf("%f",&p[2]);
        printf("Steps: ");
        scanf("%i",&Amount);
    }
	p[2] /= 2.0;
    int dimensions[2]  = {1000,1000};
    float border[4]    = {-2.0, 0.5,-1.25,1.25};
    float newborder[4] = {p[0]-p[2],p[0]+p[2],p[1]-p[2],p[1]+p[2]};
    ZoomTo(border,newborder,dimensions,Amount);
    //std::string Filename = "test/test.raw";
    //printImage(newborder,dimensions,Filename);

    return 0;
}
