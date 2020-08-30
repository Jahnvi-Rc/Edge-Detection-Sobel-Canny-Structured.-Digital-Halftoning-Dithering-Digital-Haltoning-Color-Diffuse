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
    int width = 481;
    int height = 321;

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
    {   for(k=0;k<3;k++)
            extend_boundary[0][j][k] = extend_boundary[2][j][k];  
    }
    //adding a boundary on left
    for(i=0;i<height+2;i++)
    {   for(k=0;k<3;k++)
            extend_boundary[i][0][k] = extend_boundary[i][2][k];
    }
    //adding a boundary below
    for(j=1;j<width+2;j++)
    {   for(k=0;k<3;k++)
            extend_boundary[height+1][j][k] = extend_boundary[height-1][j][k];
    }
    //adding a boundary to the right
    for(i=0;i<height+2;i++)
    {   for(k=0;k<3;k++)
            extend_boundary[i][width+1][k] = extend_boundary[i][width-1][k];
    }
    //get extended image
    for(i = 0;i<height;i++)
    {   for (j=0;j<width;j++)
        {
            extend_boundary[i+1][j+1][0] = Imagedata[i][j][0];
            extend_boundary[i+1][j+1][1] = Imagedata[i][j][1];
            extend_boundary[i+1][j+1][2] = Imagedata[i][j][2];
        }
    }
//convert to grayscale
    for (i=0;i<height+2;i++)
    {   for (j=0;j<width;j++)
        {
            gray_image[i][j]  = ceil(0.2989*extend_boundary[i][j][0] + 0.5870*extend_boundary[i][j][1] + 0.1140*extend_boundary[i][j][2]);
        }
    }
float Thresh=0.60; 
int thresh;
float sum=0;
float cdf[256]={0};
char normalized[width][height];
int xg=0; 
int yg=0;
float hist[256]={0}; 
int hist_x[]={0};
int hist_y[]={0};
int grad[width][height];
float min_grad=grad[0][0];
float max_grad=grad[0][0];
for(i=1;i<height+1;i++)
{
  for(j=1;j<width+1;j++)
  {
     xg=-1*gray_image[i][j+1]+1*gray_image[i][j-1]-1*gray_image[i+1][j-1]+1*gray_image[i-1][j-1]-2*gray_image[i-1][j+1]+2*gray_image[i+1][j+1];
     yg=-1*gray_image[i+1][j-1]+2*gray_image[i-1][j]+1*gray_image[i-1][j+1]-2*gray_image[i+1][j]+1*gray_image[i-1][j-1]-1*gray_image[i+1][j+1];
     grad[i-1][j-1]=sqrt(xg*xg+yg*yg);
  }
}
for(i=0;i<height;i++)
{
	for(j=0;j<width;j++)
    {
        if(grad[i][j]<=1020 && grad[i][j]>max_grad)
			max_grad=grad[i][j];
		else if(grad[i][j]<=1020 && grad[i][j]<min_grad)
			min_grad=grad[i][j];
	}
}
for(i=0;i<height;i++)
{
	for(j=0;j<width;j++)
    {
		float temp = float((255/max_grad-min_grad)*(grad[i][j]-min_grad));
		normalized[i][j]=(char)(temp);		
	}
}
for(i=0;i<height;i++)
{
	for(j=0;j<width;j++)
		hist[normalized[i][j]]++;
}
for(i=0;i<256;i++)
	hist[i]=((float)hist[i]/(width*height));
for(i=0;i<256;i++){   
	cdf[i]+=cdf[i-1]+hist[i];
}
for(i=0;i<256;i++){
	if(cdf[i]<=Thresh)
		thresh=i;                
}
for(i=0;i<height;i++)
{                 
	for(j=0;j<width;j++)
    {
		if(normalized[i][j]>=thresh)
			normalized[i][j]=0;
		else if(normalized[i][j]<thresh)
			normalized[i][j]=255;
	}
}
    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(normalized, sizeof(unsigned char), width*height*3, file);
    fclose(file);

    return 0;
}