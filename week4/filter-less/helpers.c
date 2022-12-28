#include "helpers.h"
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            int gray_pixel = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue)/3.00);
            image[i][j].rgbtRed = gray_pixel;
            image[i][j].rgbtGreen = gray_pixel;
            image[i][j].rgbtBlue = gray_pixel;
        }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            if (sepiaRed > 255)
                sepiaRed = 255;

            if (sepiaGreen > 255)
                sepiaGreen = 255;

            if (sepiaBlue > 255)
                sepiaBlue = 255;

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
        for (int j = 0, w = width/2; j < w; j++)
        {
            // starting at the left of a row, store the pixel's RGB values in temp
            temp.rgbtRed = image[i][j].rgbtRed;
            temp.rgbtGreen = image[i][j].rgbtGreen;
            temp.rgbtBlue = image[i][j].rgbtBlue;

            // change the RBG values of the left pixels to the right pixel
            image[i][j].rgbtRed = image[i][width-j-1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width-j-1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width-j-1].rgbtBlue;

            // change the RGB values of the right pixel to the original values stored in the left pixel
            image[i][width-1-j].rgbtRed = temp.rgbtRed;
            image[i][width-1-j].rgbtGreen = temp.rgbtGreen;
            image[i][width-1-j].rgbtBlue = temp.rgbtBlue;

        }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // every time a valid surrounding pixel is reached (i.e. it is not out of bounds), add its value to the sum, and increment the counter
    // at the end of the loop when all surrpunding pixels are checked, divide the sum by the count to get the average value off all surrounding pixels
    // set the original pixel equal to that value
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redsum = 0;
            int greensum = 0;
            int bluesum = 0;
            int count = 0.0;

            for (int k = (i-1); k < (i+2); k++)
            {
                for (int l = (j-1); l < (j+2); l++)
                {
                    if (k < 0 || k >= height || l < 0 || l >= width)      // if the row or column is out of bounds
                    {
                        continue;           // skip everything else in this for loop (essentially ignore it)
                    }
                    // adds the r, g, or b value of every surrounding pixel to their respective color sum variable
                    redsum += image[k][l].rgbtRed;
                    greensum += image[k][l].rgbtGreen;
                    bluesum += image[k][l].rgbtBlue;
                    // increases the count of pixels contributing to the blur. this will be used when dividing the sums
                    count++;
                }
            }
            // Sets the RBG values for each pixel of the temp image equal to the average of the surrounding pixels in the
            if ((float)bluesum / count)

            temp[i][j].rgbtRed = round((float)redsum / count);
            temp[i][j].rgbtGreen = round((float)greensum / count);
            temp[i][j].rgbtBlue = round((float)bluesum / count);

        }

    }

    // this set of nested loops copies each pixel from the temp "image" and applies them to the corresponding pixel in the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }

    }

    return;
}
