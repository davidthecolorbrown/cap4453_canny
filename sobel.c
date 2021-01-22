/* 
David Brown
CAP4453 
Spring 2021
Assignment 1

Sobel.c 

compile:
$ gcc sobel.c -o sobel

command-line arg:
// output for face05.pgm
$ ./sobel face05.pgm sobelmag.pgm sobelout1.pgm sobelout2.pgm 40 110
// output for garb34.pgm
$ ./sobel garb34.pgm sobel_out.pgm sobel_low.pgm sobel_hi.pgm 40 110
*/

#include <stdio.h>                          
#include <math.h>

// initialize arrays for holding image, sobel masks, etc
int pic[256][256]; // 2D array to hold pixel values from initial image
int outpicx[256][256]; // 2D array to 
int outpicy[256][256]; // 2D array to 
int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; // sobel algorithm mask for x direction
int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}}; // sobel algorithm mask for y direction 
double ival[256][256],maxival;

// main method taking command-line args
main(int argc, char **argv)
{
        // initialize variables, pointers
        int i,j,p,q,mr,sum1,sum2;
        int low_threshold, hi_threshold;
        FILE *fo1, *fo2, *fo3, *fp1, *fopen();
        char *foobar;

        // read in input image 
        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

        // generate output image for total magnitude
	argc--; argv++;
	foobar = *argv;
	fo1=fopen(foobar,"wb");

        // generate output image for low threshold
	argc--; argv++;
	foobar = *argv;
	fo2=fopen(foobar,"wb");

        // generate output image for high threshold
	argc--; argv++;
	foobar = *argv;
	fo3=fopen(foobar,"wb");

        //  initialize low threshold by convert to float
        argc--; argv++;
	foobar = *argv;
	//low_threshold = atof(foobar); // convert to float, linux/mac
        low_threshold = atoi(foobar); // convert to float, windows

        //  initialize high threshold by convert to float
        argc--; argv++;
	foobar = *argv;
	//int hi = atof(foobar); // convert to float, linux/mac
        hi_threshold = atoi(foobar); // convert to float, windows
        
        // save .pgm header at top of file so image can be viewed in .pgm viewer
        fprintf(fo1, "P5\n256 256\n255\n");
        fprintf(fo2, "P5\n256 256\n255\n");
        fprintf(fo3, "P5\n256 256\n255\n");

        // read in file
        // loop through image rows
        for (i=0;i<256;i++)
        { 
                // loop through image cols
                for (j=0;j<256;j++)
                {
                        // get the character for pixel cell from image file and save to 2D array
                        pic[i][j] = getc(fp1);

                        // remove unwanted values from pixel value using bitwise AND operator
                        pic[i][j] &= 0377;
                }
        }

        // perform convolution derivation 
        // set "mask radius [mr]" for xmask of size 3x3
        mr = 1;
        // loop through image rows
        for (i=mr;i<256-mr;i++)
        { 
                // loop through image rows
                for (j=mr;j<256-mr;j++)
                {
                        // get sum for x derivative totals, y derivative totals
                        sum1 = 0;
                        sum2 = 0;

                        // add up 3x3 products
                        // loop through 3x3 rows
                        for (p=-mr;p<=mr;p++)
                        {
                                // loop through 3x3 cols
                                for (q=-mr;q<=mr;q++)
                                {
                                        // add up x values for image[0..2][0..2] * maskx[-1..1 + 1][-1..1 + 1]
                                        sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                                        // add up y values for image[0..2][0..2] * masky[-1..1 + 1][-1..1 + 1]
                                        sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                                }
                        }

                        // save the gradient values for x/y in new array for current position
                        outpicx[i][j] = sum1;
                        outpicy[i][j] = sum2;

                        // move to next image cell to calculate value
                }
        }

        // calculate gradient (ival) for each output image pixel, calculate greatest value for scaling
        maxival = 0;
        // loop through output rows
        for (i=mr;i<256-mr;i++)
        { 
                // loop through ouput cols
                for (j=mr;j<256-mr;j++)
                {
                        // store gradient value for each output pixel in outpic 2D array in ival gradient array
                        ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));
                        
                        // find the max value in gradient array
                        if (ival[i][j] > maxival)
                        {
                                maxival = ival[i][j];
                        }

                }
        }


        // scale the values 
        // loop through gradient array rows
        for (i=0;i<256;i++)
        { 
                // loop through gradient array cols
                for (j=0;j<256;j++)
                {
                        // re-scale gradient value so it fits between 0-255
                        ival[i][j] = (ival[i][j] / maxival) * 255;   

                        // write pixel to magnitude_file         
                        fprintf(fo1,"%c",(char)((int)(ival[i][j])));

                        // check if given re-scaled gradient value for current cell is above low threshold
                        if (ival[i][j]>low_threshold)
                        {
                                // edge, write pixel to low threshold as 255       
                                fprintf(fo2,"%c", (char) 255);
                        }
                        else
                        {
                                // not edge, write pixel to low threshold file as 0
                                fprintf(fo2,"%c", (char) 0);
                        }

                        // check if given re-scaled gradient value for current cell exceeds high threshold
                        if (ival[i][j]>hi_threshold)
                        {
                                // edge, write pixel to high threshold file as 255    
                                fprintf(fo3,"%c", (char) 255);
                        }       
                        else
                        {
                                // not edge, write pixel to high threshold file as 0
                                fprintf(fo3,"%c", (char) 0);
                        } 
                }
        }

}
