# Assignment 1 (Text Rain) Worksheet

For the conceptual worksheets in this course, we'll provide a Markdown
template from the shared-upstream repository. As described in the Canvas
assignment handouts, you'll pull each Markdown template into your repository,
directly edit your local copy with your answers, and commit and push your
answers to GitHub in your `worksheets` folder of your repository. If you're
unfamiliar with Markdown syntax, [check out this lovely guide provided by
GitHub](https://guides.github.com/features/mastering-markdown/) before you get
started.

_Do not make a copy of the provided Markdown template and submit that instead._
Our grading scripts will be looking for these modified files within your
`worksheets` folder of your repository. Do not change the filenames, simply
modify the contents.

## Background

By default, Processing uses the integer-based `0-255` convention to represent
colors. For instance, bright, full-saturation red is represented as
`color(255, 0, 0)`.  Processing also supports grayscale colors; black is
`color(0)` and white is `color(255)`. You may wish to look at the [color class
documentation](https://processing.org/reference/color_.html) and/or the
[tutorial explaining color in
Processing](https://processing.org/tutorials/color/).

Here are a couple of questions to get you thinking about how to work with
pixel arrays and colors in this format.  Note:  These are very brief questions
in this first worksheet, so this may not take you long at all.  That's ok!


## Q1: Indexing

As mentioned in the assignment handout, accessing/setting pixel data via
Processing's `get()` and `set()` routines is a bit easier to code, but it's
much slower than directly accessing/changing a [PImage
object's](https://processing.org/reference/PImage.html) `pixels[]` array.
Processing stores a 2D image in this 1D array, so getting the proper pixel out
requires a little additional math.

In the code block below, write the equation for obtaining the index in the 1D
array from a (row, column) in the 2D pixels array. Keep in mind you can use
information from `inputImg` to help you.

```
PImage inputImg = loadImage("test.jpg");

// your code should work for any valid values for row and column, we've 
// randomly picked the values (2, 2) here as an exmaple.
int row = 2;
int column = 2;

// write your answer in terms of the row and column defined above
int index1D = (row - 1) * inputImg.width + column - 1;

// e.g. To obtain a pixel located at row 5 col 3, all rows above 5(1,2,3,4) 
// will be added together. Now we're at the begining of row 5, then move 3 
// steps ahead. Now, logically, we're found the goal pixel, but array index
// starts from 0 instead of 1. Thus we need minus 1.
// Now if we want pixel at row 1 col 1, the very first pixel of the image.
// Plug those number in, our 1Dindex will be: (1 - 1) * imgWidth + 1 - 1 = 0
```


## Q2: Thresholding

The image processing technique known as *thresholding* will be useful while
creating your Text Rain. During the thresholding operation, if a pixel's
grayscale value is less than `threshold`, then it becomes black. If the
value is greater than or equal to `threshold`, it becomes white. In the example below,
assume the image has already been converted to grayscale.  This means the
red, green, and blue channels are all equal.  So, you can get the grayscale
value by accessing any one of the color channels red, green, or blue.

In the code block below, write a Java code snippet for thresholding one pixel
(`inputPixel`) to black or white.

```java
final int threshold = 128;

// Returns: thresholded color (black or white)
color thresholdPixel(color inputPixel) {
    // get the value of red(in this case the grayscale value)
    // check if the pixel is less than threshold, if yes, then set to black
    if(red(inputPixel) < threshold){ 
        inputPixel = color(0);
    }
    // else set it to white
    else{
        inputPixel = color(255);
    }
}
```
