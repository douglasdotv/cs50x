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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_copy[height][width];

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double gx_red = 0.0;
            double gy_red = 0.0;

            double gx_green = 0.0;
            double gy_green = 0.0;

            double gx_blue = 0.0;
            double gy_blue = 0.0;

            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        gx_red += (double) image[k][l].rgbtRed * gx[k - i + 1][l - j + 1];
                        gy_red += (double) image[k][l].rgbtRed * gy[k - i + 1][l - j + 1];

                        gx_green += (double) image[k][l].rgbtGreen * gx[k - i + 1][l - j + 1];
                        gy_green += (double) image[k][l].rgbtGreen * gy[k - i + 1][l - j + 1];

                        gx_blue += (double) image[k][l].rgbtBlue * gx[k - i + 1][l - j + 1];
                        gy_blue += (double) image[k][l].rgbtBlue * gy[k - i + 1][l - j + 1];
                    }
                }
            }

            int updated_red = (int) round(sqrt(gx_red * gx_red + gy_red * gy_red));
            int updated_green = (int) round(sqrt(gx_green * gx_green + gy_green * gy_green));
            int updated_blue = (int) round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue));

            image_copy[i][j].rgbtRed = (int) fmin((double) updated_red, 255.0);
            image_copy[i][j].rgbtGreen = (int) fmin((double) updated_green, 255.0);
            image_copy[i][j].rgbtBlue = (int) fmin((double) updated_blue, 255.0);
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
