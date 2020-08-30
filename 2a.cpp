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
    // Fixed Threshold
    char Imagedata_ft[height][width][BytesPerPixel];
    int i,j,k;
	for(i=0;i<height;i++)
	{   for(j=0;j<width;j++)
		{   for(k=0;k<1;k++)
			{   if (Imagedata[i][j][k] < 127)
					Imagedata_ft[i][j][k] = 0;
				else 
					Imagedata_ft[i][j][k] = 255;
    }}}
    //Random Thresholding
    char Imagedata_rt[height][width][BytesPerPixel];
	for(i=0;i<height;i++)
	{   for(j=0;j<width;j++)
		{   for(k=0;k<BytesPerPixel;k++)
			{   char rand_num = rand()%256;
				if (Imagedata[i][j][k]<=rand_num)
                    Imagedata_rt[i][j][k] = 0;
				else 
                    Imagedata_rt[i][j][k] = 255;
    }}}
    //Dithering Matrices
    char Imagedata_dit_I2[height][width][BytesPerPixel];
    char Imagedata_dit_I8[height][width][BytesPerPixel];
    char Imagedata_dit_I32[height][width][BytesPerPixel];
    int x;
    char Imagedata_temp2[2][2];
	for(i=0;i<2;i++)
	{   for(j=0;j<2;j++)
			Imagedata_temp2[i][j]=0;
	}
	for(i=0;i<2;i++)
	{   for(j=0;j<2;j++)
		{   if (i<1&&j<1)
				x=1;
			else if(i<1&&j>=1)
				x=2;
			else if(i>=1&&j<1)
				x=3;
			else
				x=0;
			Imagedata_temp2[i][j]=x;
    }}
    char Imagedata_temp8[8][8];
	for(i=0;i<8;i++)
	{   for(j=0;j<8;j++)
			Imagedata_temp8[i][j]=0;
	}
	for(i=0;i<8;i++)
	{   for(j=0;j<8;j++)
		{   if (i<4&&j<4)
				x=1;
			else if(i<4&&j>=4)
				x=2;
			else if(i>=4&&j<4)
				x=3;
			else
				x=0;
			Imagedata_temp2[i][j]=4*4*Imagedata_temp2[i%2][j%2]+x;
    }}
    char Imagedata_temp32[32][32];
	for(i=0;i<32;i++)
	{   for(j=0;j<32;j++)
			Imagedata_temp32[i][j]=0;
	}
	for(i=0;i<32;i++)
	{   for(j=0;j<32;j++)
		{   if (i<16&&j<16)
				x=1;
			else if(i<16&&j>=16)
				x=2;
			else if(i>=16&&j<16)
				x=3;
			else
				x=0;
			Imagedata_temp2[i][j]=4*4*4*4*Imagedata_temp2[i%2][j%2]+x;
    }}

    //output
    for(i=0;i<height;i++)
	{   for(j=0;j<width;j++)
		{   for(k=0;k<BytesPerPixel;k++)
			{   if(((double)(Imagedata[i][j][k]))<=((double)(255*((Imagedata_temp2[i%2][j%2]+0.5)/1024))))         
					Imagedata_dit_I2[i][j][k]=0;
				else
					Imagedata_dit_I2[i][j][k]=255;
            }}}
    for(i=0;i<height;i++)
	{   for(j=0;j<width;j++)
		{   for(k=0;k<BytesPerPixel;k++)
			{   if(((double)(Imagedata[i][j][k]))<=((double)(255*((Imagedata_temp8[i%8][j%8]+0.5)/1024))))         
					Imagedata_dit_I8[i][j][k]=0;
				else
					Imagedata_dit_I8[i][j][k]=255;
            }}}
    for(i=0;i<height;i++)
	{   for(j=0;j<width;j++)
		{   for(k=0;k<BytesPerPixel;k++)
			{   if(((double)(Imagedata[i][j][k]))<=((double)(255*((Imagedata_temp32[i%32][j%32]+0.5)/1024))))         
					Imagedata_dit_I32[i][j][k]=0;
				else
					Imagedata_dit_I32[i][j][k]=255;
            }}}

    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata_dit_I8, sizeof(unsigned char), width*height*3, file);
    fclose(file);
    return 0;
}
