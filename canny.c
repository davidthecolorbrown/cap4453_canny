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
$ ./canny garb34.pgm magnitude.pgm peaks.pgm [sigma==1]
// part 2: output for garb34.pgm
$ ./canny garb34.pgm magnitude.pgm peaks.pgm [sigma==1]
// part 3: output for garb34.pgm 
$ ./canny garb34.pgm magnitude.pgm peaks.pgm final_edges.pgm [sigma==1] [lo==35.0] [hi==100.0]
// part 4: output for garb34.pgm
$ ./canny garb34.pgm magnitude.pgm peaks.pgm final_edges.pgm [sigma==1] [percent]

*/


 
#include <stdio.h>                 
#include <stdlib.h>   
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
double x_conv[PICSIZE][PICSIZE];
double y_conv[PICSIZE][PICSIZE];
double ival[PICSIZE][PICSIZE], mag[PICSIZE][PICSIZE], maxival;

double peaks[256][256]; 
double final[256][256]; 
double edges[256][256]; 
int    histogram[PICSIZE];
//int    histogram[PICSIZE][PICSIZE];
//double histogram[PICSIZE][PICSIZE];
//int edges[256][256]; // peaks[][] == edges[][]???
//char edges[PICSIZE][PICSIZE]; // peaks[][] == edges[][]???
//int    t_pic[PICSIZE][PICSIZE];

// main method taking command-line args
main(int argc, char **argv)
{
        // initialize variables
        int     i,j,p,q,s,t,mr,centx,centy;
        //int     lo, hi;
        double  lo, hi, percent;
        double  x_maskval,y_maskval,x_sum,y_sum,sig,maxival,minmag,maxval;
        double  slope;
        //double sum,ZEROTOL;
        FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
        char    *foobar;

        // print command line args
        printf("COMMAND-LINE ARGS\n");
        printf("%d\n",argc);
        printf("%s\n", argv[0]);
        printf("%s\n", argv[1]);
        printf("%s\n", argv[2]);
        printf("%s\n", argv[3]);
        printf("%s\n", argv[4]);
        printf("%s\n", argv[5]);
        printf("%s\n", argv[6]);
        // for(int k=1;k<argc;k++)
        // {
        //         printf("%s", argv[i]);
        // }

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

        argc--; argv++;
        foobar = *argv;
        fo3=fopen(foobar,"wb");

        argc--; argv++;
        foobar = *argv;
        //sig = atof(foobar);
        sig = atoi(foobar); // convert to float, windows
        //sig = 1.0;
        printf("sig: %f\n", sig);

        argc--; argv++;
        foobar = *argv;
        percent = atof(foobar);
        //percent = atoi(foobar); // convert to float, windows
        //percent = 0.00005;
        //percent = 0.0005;
        //percent = 0.005;
        //percent = 0.05;
        //percent = 0.1;
        //percent = 0.5;
        //printf("percent: %f\n", foobar);
        //printf("percent: %c\n", (char) ((int) percent));
        printf("percent: %f\n", percent);

        //    argc--; argv++;
        //    foobar = *argv;
        //    ZEROTOL = atof(foobar);

        //  initialize low threshold by convert to float
        // argc--; argv++;
	// foobar = *argv;
	// //lo= atof(foobar); // convert to float, linux/mac
        // lo = atoi(foobar); // convert to float, windows
        //lo = 35.0;

        // //  initialize high threshold by convert to float
        // argc--; argv++;
	// foobar = *argv;
	// //int hi = atof(foobar); // convert to float, linux/mac
        // hi = atoi(foobar); // convert to float, windows
        //hi = 100.0;
        
        // save .pgm header at top of file so image can be viewed in .pgm viewer
        fprintf(fo1, "P5\n256 256\n255\n");
        fprintf(fo2, "P5\n256 256\n255\n");
        fprintf(fo3, "P5\n256 256\n255\n");

        // get mask size based on sigma
        //mr = (int)(sig * 6) + 1;
        mr = (int)(sig * 3);
        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);
        
        // garb34.pgm has header, avoid copying this to pic[][]
        fseek(fp1, 32, SEEK_SET ); // move to offest 32 from begin of file

        // read in image
        for (i=0;i<256;i++)
        {
                for (j=0;j<256;j++)
                {
                        pic[i][j] = getc(fp1);
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
                        // calculate first derivative for x
                        x_maskval = q * (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
                        x_mask[p+centy][q+centx] = x_maskval;

                        // calculate first derivative for y
                        y_maskval = p * (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
                        y_mask[p+centy][q+centx] = y_maskval;
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

                        // save values into x/y axis convolution arr
                        x_conv[i][j] = x_sum;
                        y_conv[i][j] = y_sum;
                }
        }
 
        // calculate gradient (ival) for each output iivale pixel, calculate greatest value for scaling
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
                        mag[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
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
        //for (i=0;i<256-mr;i++)
        {
                // loop through gradient array cols
                for (j=0;j<256;j++)
                //for (j=0;j<256-mr;j++)
                {
                        // re-scale gradient value so it fits between 0-255
                        ival[i][j] = (ival[i][j] / maxival) * 255;  

                        // write pixel to ivalnitude_file        
                        fprintf(fo1,"%c",(char)((int)(ival[i][j])));

                        mag[i][j] = ival[i][j];
                }
        }
        

        /* part 2 -- find the peaks using gradient direction Max Tests (non-maxima supression) */
        // loop through rows
        for(i=mr;i<256-mr;i++){

                // loop through columns
                for(j=mr;j<256-mr;j++){

                        // avoid divide by zero errors
                        if((x_conv[i][j]) == 0.0) {
                                x_conv[i][j] = 0.00001;
                        }
                        
                        // get slope value
                        slope = y_conv[i][j] / x_conv[i][j];

                        // horizontal test (<= 22.5 and > -22.5)
                        if((slope <= 0.4142)&&(slope > -0.4142)){
                                // compare adjacent cols
                                if((mag[i][j] > mag[i][j-1])&&(mag[i][j] > mag[i][j+1])){
                                        edges[i][j] = 255;
                                }
                        }
                        // upward diagonal test (<= 67.5 and > 22.5)
                        else if( (slope <= 2.4142)&&(slope > 0.4142)){
                                // compare northeast and southwest
                                if((mag[i][j] > mag[i-1][j-1])&&(mag[i][j] > mag[i+1][j+1])){
                                        edges[i][j] = 255;
                                }
                        }
                        // downward diagonal test (<= -22.5 and > -67.5)
                        else if( (slope <= -0.4142)&&(slope > -2.4142)){
                                // compare northwest and southeast
                                if((mag[i][j] > mag[i+1][j-1])&&(mag[i][j] > mag[i-1][j+1])){
                                        edges[i][j] = 255;
                                }
                        }
                        // vertical test (slope > 67.5 and 90 or slope > -67.5 and -90)
                        else { 
                                // compare adjacent rows
                                if((mag[i][j] > mag[i-1][j])&&(mag[i][j] > mag[i+1][j])){
                                        edges[i][j] = 255;
                                }
                        }
                }
        }

        // loop through edges array rows
        for (i=0;i<256;i++)
        //for (i=0;i<256-mr;i++)
        {
                // loop through edge array cols
                for (j=0;j<256;j++)
                //for (j=0;j<256-mr;j++)
                {
                        // write pixel to magnitude_file        
                        fprintf(fo2,"%c",(char)((int)(edges[i][j])));

                        // troubleshooting
                        //printf("peaks: %c\n", (char) ((int) edges[i][j]));
                }
        }

        /* part 4 -- calculate hi/lo threshold values */

        // compute histogram of scaled magnitudes 
        // histogram x-axis: the possible scaled magnitude values (1-255 brightness)
        // histogram y-axis: the number of times each brightness value (1-255) appears in scaled magnitude array
        int idx = 0;
         // loop through rows
        for(i=mr;i<256-mr;i++){

                // loop through columns
                for(j=mr;j<256-mr;j++){
                        // increment the number of scaled magnitudes by 1 each time it appears
                        //(histogram[mag[i][j]])++;
                        //idx = mag[i][j];
                        (histogram[(int) ival[i][j]])++;

                        
                }
        }       

        // compute the cutoff value for threshold (cutoff == percent*rows*cols)
        // tells you what percentage of the total pixels in image should meet your threshold criteria
        //double cutoff = 0.005 * PICSIZE * PICSIZE;
        double cutoff = percent * PICSIZE * PICSIZE;
        printf("cutoff: %f\n", cutoff);

        // variable which holds area of large magnitudes (number of pixels that are above threshold)
        int areaOfTops = 0;

        // loop through histogram array until you've reached the cutoff level for "hi" threshold pixels
        for(i=PICSIZE-1; i>1; i--) {
                // sum total number of magnitudes based on their brightness value
                areaOfTops += histogram[i];

                // check whether we've reached the cutoff point (max number of pixels we allow above hi threshold)
                if (areaOfTops > cutoff) {
                        // variable which stores the min brightness level above threshold
                        hi = i;

                        // if we've reached maximum number of allowable pixels above threshold, we've found hi threshold value, break out of for loop
                        break;
                }

        }   

        // calculate the lo threshold given hi threshold
        lo = 0.35*hi;
        printf("hi: %f\n", hi);
        printf("lo: %f\n", lo);      


        /* part 3 -- find peaks above hi threshold or adjacent to hi threshold peaks */
        // loop through rows
        for(i=mr;i<256-mr;i++){

                // loop through columns
                for(j=mr;j<256-mr;j++){

                        // check whether pixel is a peak
                        if (edges[i][j] == 255) {
                                // check whether this peak is > than HI threshold
                                if (ival[i][j] > hi) {
                                //if (mag[i][j] > hi) {
                                        // remove from temporary edges array ("off")
                                        edges[i][j] = 0;
                                        // pixel is "on" stored in final output array 
                                        final[i][j] = 255;
                                }
                                // else if mag[ij] < lo threshold, ignore further consideration of this pixel
                                else {
                                        // set this pixel to "off" for both arrays
                                        edges[i][j] = 0;
                                        final[i][j] = 0;
                                }
                        }
                }
        }

        // set finished flag to true (1) to check for peaks inbetween thresholds 
        int moretodo = 1;

        // check peaks with magnitudes between hi/lo thresholds
        while (moretodo == 1) {
                // turn moretodo flag off/false (0)
                moretodo = 0;

                // loop through rows
                for(i=mr;i<256-mr;i++){

                        // loop through columns
                        for(j=mr;j<256-mr;j++){

                                // check whether pixel is a peak
                                if (edges[i][j] == 255) {

                                        // loop through adjacent rows
                                        for (p=-mr;p<=mr;p++)
                                        //for (p=-1; p <= 1; p++)
                                        {
                                                // loop through adjacent columns
                                                for (q=-mr;q<=mr;q++)
                                                //for (q=-1; q <= 1; q++)
                                                {
                                                        // check if this pixel is adjacent to a true peak (pixel which is above hi threshold)
                                                        if (final[i+p][j+q] == 255) {
                                                                // remove from temporary edges array ("off") to avoid checking again
                                                                edges[i][j] = 0;
                                                                // pixel is "on" and stored in final output array 
                                                                final[i][j] = 255;
                                                                // reset peak search, turn flag on to continue looping until all possible peaks/pixels have been declared
                                                                moretodo = 1;
                                                        }
                                                }
                                        }
                                }

                        }
                }
        }
                
        
        // loop through final array rows to print output
        for (i=0;i<256;i++)
        //for (i=0;i<256-mr;i++)
        {
                // loop through final array cols
                for (j=0;j<256;j++)
                //for (j=0;j<256-mr;j++)
                {
                        // write pixel to final.pgm      
                        fprintf(fo3,"%c",(char)((int)(final[i][j])));
                }
        }
}


