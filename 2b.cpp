// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:    The code assumes that the image is of size 256 x 256 and is in the
//          RAW format. You will need to make corresponding changes to
//          accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <map>
using namespace std;

int main(int argc, char *argv[])
{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 1;
    int width = 750;
    int height = 500;

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is grayscale or color
    if (argc < 4){
        BytesPerPixel = 1; // default is grey image
    }
    else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5){
            width = atoi(argv[4]);
            height = atoi(argv[5]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[height][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE ////////////////////////
    
    // int i,j,k;
    // unsigned char grayscale[height][width];
    // for(i=0;i<height;i++)
    // {   for(j=0;j<width;j++)
    //         grayscale[i][j]=((float)Imagedata[i][j][0]+(float)Imagedata[i][j][1]+(float)Imagedata[i][j][2])/3;
    // }
        //converting from RGB to gray image
    unsigned char gray_image[height][width];
    int i,j,k;
    for (i=0;i<height;i++)
    {   for (j=0;j<width;j++)
        {
            gray_image[i][j]  = ceil(0.2989*Imagedata[i][j][0] + 0.5870*Imagedata[i][j][1] + 0.1140*Imagedata[i][j][2]);
        }
    }
    unsigned char extend_boundary[height+2][width+2][BytesPerPixel];
    //add a boundary on top
    for(j=1;j<width+2;j++)
    {   for(k=0;k<1;k++)
            extend_boundary[0][j][k] = extend_boundary[2][j][k];  
    }
    //adding a boundary on left
    for(i=0;i<height+2;i++)
    {   for(k=0;k<1;k++)
            extend_boundary[i][0][k] = extend_boundary[i][2][k];
    }
    //adding a boundary below
    for(j=1;j<width+2;j++)
    {   for(k=0;k<1;k++)
            extend_boundary[height+1][j][k] = extend_boundary[height-1][j][k];
    }
    //adding a boundary to the right
    for(i=0;i<height+2;i++)
    {   for(k=0;k<1;k++)
            extend_boundary[i][width+1][k] = extend_boundary[i][width-1][k];
    }
    //get extended image
    for(i = 0;i<height;i++)
    {   for (j=0;j<width;j++)
        {
            extend_boundary[i+1][j+1][1] = Imagedata[i][j][0];
        }
    }
    //FS
    char Image_fs[height][width];
    int Thresh = 128;
    for(i=1;i<height+1;i++)
    {   if (i%2==0)
        {
            for(j=width;j>=1;j--)
            {
                if(extend_boundary[i][j][1]>Thresh)
                    Image_fs[i-1][j-1] = 255;
                else
                    Image_fs[i-1][j-1]=0;
                extend_boundary[i+1][j-1][1]+=(extend_boundary[i][j][1]-Image_fs[i-1][j-1])/16;
                extend_boundary[i][j-1][1]+=5*(extend_boundary[i][j][1]-Image_fs[i-1][j-1])/16;
                extend_boundary[i+1][j+1][1]+=3*(extend_boundary[i][j][1]-Image_fs[i-1][j-1])/16;
                extend_boundary[i+1][j][1]+=7*(extend_boundary[i][j][1]-Image_fs[i-1][j-1])/16;
            }
        }
        else
        {
            for(j=1;j<width+1;j++)
            {
                if(extend_boundary[i][j][1]>Thresh)
                    Image_fs[i-1][j-1] = 255;
                else
                    Image_fs[i-1][j-1]=0;
                extend_boundary[i+1][j-1][1]+=3*(extend_boundary[i][j][1]-Image_fs[i-1][j-1])/16;
                extend_boundary[i][j + 1][1]+=5*(extend_boundary[i][j][1]-Image_fs[i-1][j-1])/16;
                extend_boundary[i+1][j+1][1]+=(extend_boundary[i][j][1]-Image_fs[i-1][j-1])/16;
                extend_boundary[i+1][j][1]+=7*(extend_boundary[i][j][1]-Image_fs[i-1][j-1])/16;
            }}}
//JJN
char Image_jjn[height][width];
int Thresh = 140;
    for(i=2;i<height+2;i++)
    {   if((i-2)%2==0)
        {   for(j=width+1;j>=2;j--)
            {
                if(extend_boundary[i][j][1]>Thresh)
                    Image_jjn[i-1][j-1] = 255;
                else
                    Image_jjn[i-1][j-1]=0;
                extend_boundary[i+1][j+1][1]+=5*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j+2][1]+=(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i][j-1][1]+=7*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+1][j-2][1]+=3*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+1][j][1]+=7*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+1][j+2][1]+=3*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i][j-2][1]+=5*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j-2][1]+=1*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j][1]+=5*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+1][j-1][1]+=5*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j-1][1]+=3*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j+1][1]+=3*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
            }
        }
        else
        { for(j=2;j<width+2;j++)
            {
                if(extend_boundary[i][j][1]>Thresh)
                    Image_jjn[i-1][j-1] = 255;
                else
                    Image_jjn[i-1][j-1]=0;
                extend_boundary[i+1][j+1][1]+=5*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i][j+1][1]+=7*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j-1][1]+=3*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+1][j-2][1]+=3*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+1][j-1][1]+=5*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+1][j][1]+=7*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j+2][1]+=(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i][j+2][1]+=5*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+1][j+2][1]+=3*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j-2][1]+=1*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j][1]+=5*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
                extend_boundary[i+2][j+1][1]+=3*(extend_boundary[i][j][1]-Image_jjn[i-2][j-2])/48.0;
            } 
        }
    }
    //stucki
    char Image_stucki[height][width];
    int Thresh = 150;
    for(i=2;i<height+2;i++)
    {   if((i-2)%2==0)
        {   for (j=width+1;j>=2;j--)
            {
                if(extend_boundary[i][j][1]>Thresh)
                    Image_stucki[i-1][j-1] = 255;
                else
                    Image_stucki[i-1][j-1]=0;
                extend_boundary[i+2][j+2][1]+=(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j-2][1]+=(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j-2][1]+=2*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j+2][1]+=2*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j-1][1]+=2*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j+1][1]+=2*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i][j-2][1]+=4*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j+1][1]+=4*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j][1]+=4*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j-1][1]+=4*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i][j-1][1]+=8*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j][1]+=8*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                
            }
        }
        else
        {
            for (j=2;j<width+2;j++)
            {   if(extend_boundary[i][j][1]>Thresh)
                    Image_stucki[i-1][j-1] = 255;
                else
                    Image_stucki[i-1][j-1]=0;
                extend_boundary[i][j+1][1]+=8*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j-2][1]+=(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j+2][1]+=(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i][j+2][1]+=4*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j][1]+=8*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j+1][1]+=4*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j-2][1]+=2*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j-1][1]+=4*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+1][j+2][1]+=2*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j-1][1]+=2*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j][1]+=4*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0;
                extend_boundary[i+2][j+1][1]+=2*(extend_boundary[i][j][1]-Image_stucki[i-2][j-2])/42.0rr;
            }
        }
    }

        // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Image_stucki, sizeof(unsigned char), width*height*3, file);
    fclose(file);
    return 0;
}