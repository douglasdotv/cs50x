#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];

            int original_red = pixel->rgbtRed;
            int original_green = pixel->rgbtGreen;
            int original_blue = pixel->rgbtBlue;

            int avg = (int) round((original_red + original_green + original_blue) / 3.0);

            pixel->rgbtRed = avg;
            pixel->rgbtGreen = avg;
            pixel->rgbtBlue = avg;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];

            int original_red = pixel->rgbtRed;
            int original_green = pixel->rgbtGreen;
            int original_blue = pixel->rgbtBlue;

            int sepia_red = (int) round(0.393 * original_red + 0.769 * original_green + 0.189 * original_blue);
            int sepia_green = (int) round(0.349 * original_red + 0.686 * original_green + 0.168 * original_blue);
            int sepia_blue = (int) round(0.272 * original_red + 0.534 * original_green + 0.131 * original_blue);

            pixel->rgbtRed = (int) fmin((double) sepia_red, 255.0);
            pixel->rgbtGreen = (int) fmin((double) sepia_green, 255.0);
            pixel->rgbtBlue = (int) fmin((double) sepia_blue, 255.0);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double total_red = 0.0;
            double total_green = 0.0;
            double total_blue = 0.0;
            int pixel_count = 0;

            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        total_red += (double) image[k][l].rgbtRed;
                        total_green += (double) image[k][l].rgbtGreen;
                        total_blue += (double) image[k][l].rgbtBlue;
                        pixel_count++;
                    }
                }
            }

            int avg_red = (int) round(total_red / pixel_count);
            int avg_green = (int) round(total_green / pixel_count);
            int avg_blue = (int) round(total_blue / pixel_count);

            image_copy[i][j].rgbtRed = avg_red;
            image_copy[i][j].rgbtGreen = avg_green;
            image_copy[i][j].rgbtBlue = avg_blue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }

    return;
}
