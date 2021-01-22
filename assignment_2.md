# assignment 2 - elementary edge detection (module 2)
* objective: use assignment 1 (sobel edge detection) as basis for creating canny edge detector.
* chess picture images: http://www.cs.ucf.edu/courses/cap4453/inputpics
* canny images: http://www.cs.ucf.edu/courses/cap4453/outputpics/
* bases program: http://www.cs.ucf.edu/courses/cap4453/progs

## canny edge detection algorithm

### part 1: 
* canny algorithm is same as sobel, except for the "smoothener"
* canny smoothener: gaussian
* sobel smoothener: four one's
* can use assignment 1 (sobel.c) code and add flexible mask sizes in both x/y direction of gradient and loop through calculating gradient (gauss distribution first derivative) in both x/y direction for mask
### part 2: 
* perform "max test" by moving through magnitude vector array and finding gradient magnitude and direction
* pseudocode:
```code
/*
current pixel: [i, j]
[i-1, j-1]      [i-1, j]    [i-1, j+1]
[i, j-1]        [i, j]      [i, j+1]
[i+1, j-1]      [i+1, j]    [i+1, j+1]

current pixel: [0, 0]
[-1, -1]   [-1, 0]   [-1, 1]
[0, -1]    [0, 0]    [0, 1]
[1, -1]    [1, 0]    [1, 1]
*/
```
### part 3: 
* 
### part 4: generate threshold values automatically given some input percentage
* psuedocode for part 4:
```code
// part 1: compute gradient vector magnitude

// part 2: compute peaks

// part 4 (comes before part 3!): find hi value based on input percentage automatically
// read percent as input
// computer histogram of scaled magnitudes
for i 
    for j
        (histogram[magnitude(i,j)])++

// compute cutof value (point exceeding percent of all, where hi threshold starts)
cutoff = percent*rows*cols

// move down histogram downwards (right to left) to find hi/low threshold values
for (i = histogram.size; i > 1; i--)
    // add total histogram count to area of large magnitudes
    areaOfTops += histogram[i]
    // when the magnitudes reach a cutoff, break out of loop, found threshold
    if (areaOfTops > cutoff)
        break;

// assign the threshold values using i from for loop
hi = i
lo = (0.35) * hi

// part 3: double thresholds
```
## instructions
### part1
* download marrh.c from website and make changes to it specified in lecture by integrating with sobel.c (hw1).
* set sigma value to 1.0
* run code on garb34.pgm
* check: if output image is similar to the magnitude image produced by sobel.c, code is correct
* check: [output should look like cannymag.pgm](http://www.cs.ucf.edu/courses/cap4453/outputpics/)
### part2
* implement peak detection using lectures in part 2 using array names consistent with part 1 code.
* beware of potential illusory errors! this step is important and some images might seem correct but are actually incorrect!
* check: [output should look like cannypeaks.pgm](http://www.cs.ucf.edu/courses/cap4453/outputpics/)
### part3
* implement hi/lo threshold code and manually enter these thresholds (HI == 100, LO == 35)
* check: [output should look like cannyfinal.pgm](http://www.cs.ucf.edu/courses/cap4453/outputpics/)

### part4
* implement hi/lo threshold "calculator" which automatically calculates hi/lo threshold 
* afterwards, you will not have to manually enter hi/lo thresholds, only a percent (use 0.5 or 0.005)
* check: [output should look like cannyfinal.pgm (same as part 3)](http://www.cs.ucf.edu/courses/cap4453/outputpics/)

### submission
* program must be able to read in any picture file (.pgm) and produce three output pictures;
    1) magnitude image (magnitude.pgm)
    2) output for peaks (peaks.pgm)
    3) final full canny algorithm output, with thresholds (Final Edges.pgm)
* run a final check of program by using garb34.pgm and verifying the output images match cannymag, cannypeaks, cannyfinal 
* submit canny.c code 

## random notes i've managed to forget about C
* command-line args
    * argc: number of args passed
    * argv[]: pointer array pointing to each arg passed to program (argv[0] == name of program itself)
    ```bash
    $ ./program.out argv[1] argv[2] .... argv[argc] // argv[0] == 'program.out'
    ```
* compiling C program
    ```bash
    # compile with gcc
    gcc program_name.c -o program_name
    # execute
    ./program_name argv[1] argv[2] 
    ```
* command-line arg for running program:
```bash
# canny.c with single threshold
./canny garb34.pgm output_img.pgm threshold
# canny.c with low/high threshold
./canny garb34.pgm output_img.pgm low_threshold high_threshold
# canny.c with three outputs + low/high threshold
./canny garb34.pgm mag_out.pgm low_out.pgm hi_out.pgm low_threshold high_threshold
# output for face05.pgm
./canny face05.pgm cannymag.pgm cannyout1.pgm cannyout2.pgm 40 110
# output for garb34.pgm
./canny garb34.pgm magnitude.pgm peaks.pgm final_edges.pgm 40 110
// output for garb34.pgm
$ ./canny garb34.pgm magnitude.pgm peaks.pgm final_edges.pgm [percent]
```

git push -u origin main
