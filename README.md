# Image-compression-with-Run-Length-Encoding-RLE-

If the same colors are used repeatedly in an image, instead of constantly repeating the code that indicates the color, 
space can be saved by storing the color code and the number of times it is used.

For example, if 5 colors are repeated 10 times in a row, it is more advantageous to store this information as 10 5 instead of 5 5 5 5 5 5 5 5 5 5.

Here, the number 10, which shows how many times the color is used, is called the run length.

In this project, an algorithm is designed to obtain the compressed image by compressing a gray image given in pgm format, in which the colors are shown with numbers between 0-255, with the RLE method, 
and to obtain the original image by decompressing the compressed image when requested.

The project consists of 6 main parts

1- Image reading: decodes the format of the pgm file and reads the row count of the image, the number of columns and the image.

2- Image compression with RLE method: By applying the RLE method to the image, write the compressed information to a file with the number of rows, number of columns, number of consecutive uses and color code order of the image.

3- Unpacking the compressed image with the RLE method: Unzip the compressed image to get the original image. Before doing this, check whether the compressed image is suitable or not. For this, it is necessary to perform the procedures given in article 4. 
If these checks are successful, the compressed image should be decompressed.

4- Check :  
When the file entry information of the compressed image is given, it should be checked whether it is an open compressed data or not:

a. Checking that the number of pixels in the compressed image is as Number of Rows*Number of Columns: 
For example, the example compressed image below consists of 3 rows and 2 columns, ie 6 pixels. Since 5 7 colors and 1 9 colors are used, 
the total number of pixels is 6, so it is a correctly compressed image.

Example : 3 2 5 7 1 9

b. Checking that colors between 0-255 are used in the compressed image

c. Checking that the Run Length of the same color is not found consecutively: For example, the compressed image below consists of 24 pixels. 
For 5 colors, 2 different processes were made one after the other. This is incorrect compression. If a file like this comes
error message should be given and opening operation should not be performed. The last 3 in 5 is correct because it is another color.

Example: 3 8 6 5 7 5 8 1 3 5

5- Image writing : After unzipping the compressed image, write it to a new file in pgm format. 
This file should be open and viewable with any image viewing tool that can open a pgm file.

6- Operations to be performed on the compressed image: Do the following operations without opening the compressed image.

a. Changing the value of a given color: For example, changing all 5 colors to 10 colors in the compressed file.

b. Changing the value of a pixel whose coordinates are given as input

c. Calculate the histogram of image.
