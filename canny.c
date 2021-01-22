/* 
David Brown
CAP4453 
Spring 2021
Assignment 2

canny.c 

compile:
$ gcc canny.c -o canny

command-line arg:
// part 1: output for garb34.pgm
$ ./canny garb34.pgm magnitude.pgm peaks.pgm [sigma]
// part 2: output for garb34.pgm
$ ./canny garb34.pgm magnitude.pgm peaks.pgm [sigma] [lo] [hi]
// part 3: output for garb34.pgm 
$ ./canny garb34.pgm magnitude.pgm peaks.pgm final_edges.pgm [sigma] [lo] [hi]
// part 4: output for garb34.pgm
$ ./canny garb34.pgm magnitude.pgm peaks.pgm final_edges.pgm [sigma] [percent]

 // read write write sigma 
*/


 
#include <stdio.h>                 
#include <math.h>
#define  PICSIZE 256
#define  MAXMASK 100
 
// initialize arrays
int    pic[PICSIZE][PICSIZE];
double outpicx[PICSIZE][PICSIZE];
double outpicy[PICSIZE][PICSIZE];
int    edgeflag[PICSIZE][PICSIZE];
double x_mask[MAXMASK][MAXMASK];
double y_mask[MAXMASK][MAXMASK];
double conv[PICSIZE][PICSIZE];
double ival[256][256],maxival;
 
// main method taking command-line args
main(int argc, char **argv)
{
        // initialize variables
        int     i,j,p,q,s,t,mr,centx,centy;
        int lo, hi;
        double  maskval,x_sum,y_sum,sig,maxival,minival,maxval;
        //double sum,ZEROTOL;
        FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
        //FILE    *fo1, *fo2,*fp1, *fopen();
        char    *foobar;

        // get command-line args
        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"wb");

        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb");

        // argc--; argv++;
        // foobar = *argv;
        // fo3=fopen(foobar,"wb");

        argc--; argv++;
        foobar = *argv;
        //sig = atof(foobar);
        sig = atoi(foobar); // convert to float, windows

        //    argc--; argv++;
        //    foobar = *argv;
        //    ZEROTOL = atof(foobar);
        
        //  initialize low threshold by convert to float
        // argc--; argv++;
	// foobar = *argv;
	// //low_threshold = atof(foobar); // convert to float, linux/mac
        // lo = atoi(foobar); // convert to float, windows

        // //  initialize high threshold by convert to float
        // argc--; argv++;
	// foobar = *argv;
	// //int hi = atof(foobar); // convert to float, linux/mac
        // hi = atoi(foobar); // convert to float, windows
        
        // save .pgm header at top of file so image can be viewed in .pgm viewer
        fprintf(fo1, "P5\n256 256\n255\n");
        fprintf(fo2, "P5\n256 256\n255\n");
        //fprintf(fo3, "P5\n256 256\n255\n");

        // troubleshooting
        printf("sigma: %f\n", sig);

        // get mask size based on sigma
        mr = (int)(sig * 3);
        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);
 
        // read in image
        for (i=0;i<256;i++)
        {
                for (j=0;j<256;j++)
                {
                        pic[i][j]  =  getc(fp1);
                }
        }
 

        /* part 1 -- gaussian smoothener to find gradient magnitude */        
        // get derivative for x/y, store in mask
        // loop through rows
        for (p=-mr;p<=mr;p++)
        { 
                // through cols
                for (q=-mr;q<=mr;q++)
                {
                        // second derivative
                        //maskval = ((2-(((p*p)+(q*q))/(sig*sig))) * (exp(-1*(((p*p)+(q*q))/(2*(sig*sig))))));
                        //(mask[p+centy][q+centx]) = maskval;

                        // calculate first derivative for x
                        maskval = p * (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
                        x_mask[p+centy][q+centx] = maskval;

                        // calculate first derivative for y
                        maskval = q * (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
                        y_mask[p+centy][q+centx] = maskval;
                }
        }

        // convolution
        // loop through rows
        for (i=mr;i<=255-mr;i++)
        {
        // loop through columns
        for (j=mr;j<=255-mr;j++)
        {
                // set/reset sum for mask equal
                y_sum = 0;
                x_sum = 0;

                // loop through rows in x mask
                for (p=-mr;p<=mr;p++)
                {
                // loop through cols in x mask
                for (q=-mr;q<=mr;q++)
                {
                        // add x derivative/gradient to total
                        x_sum += pic[i+p][j+q] * x_mask[p+centy][q+centx];
                }
                }

                // loop through rows in y mask
                for (p=-mr;p<=mr;p++)
                {
                // loop through cols in y mask
                for (q=-mr;q<=mr;q++)
                {
                        // add y derivative/gradient to total
                        y_sum += pic[i+p][j+q] * y_mask[p+centy][q+centx];
                }
                }

                // save values for current pixel
                outpicx[i][j] = x_sum;
                outpicy[i][j] = y_sum;
                //conv[i][j] = sum;
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
                }
        }

        /* part 2 -- find the peaks using gradient direction Max Tests */
        /* part 4 -- calculate hi/lo threshold values */
        /* part 3 -- find peaks above hi threshold or adjacent to hi threshold peaks */
}
