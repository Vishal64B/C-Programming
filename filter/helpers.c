#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE avg = round((float)(image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / (float)3);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for(int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE copy = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = copy;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE img_copy [height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sr = 0;
            int sb = 0;
            int sg = 0;

            int count = 0;
            int k = 0, l = 0, m = 0, n = 0;

            if (i == 0)
            {
                k = i;
            }
            else
            {
                k = i - 1;
            }

            if (i == height - 1)
            {
                m = i;
            }
            else
            {
                m = i + 1;
            }


            if (j == 0)
            {
                l = j;
            }
            else
            {
                l = j - 1;
            }

            if (j == width - 1)
            {
                n = j;
            }
            else
            {
                n = j + 1;
            }

            int copy = l;

            for (; k <= m; k++)
            {
                for (l = copy; l <= n; l++)
                {
                    sr = sr + image[k][l].rgbtRed;
                    sb = sb + image[k][l].rgbtBlue;
                    sg = sg + image[k][l].rgbtGreen;
                    count++;
                }
            }

            sr = round((float) sr / (float) count);
            sb = round((float) sb / (float) count);
            sg = round((float) sg / (float) count);

            sr = (sr < 255 ? sr : 255);
            sb = (sb < 255 ? sb : 255);
            sg = (sg < 255 ? sg : 255);

            img_copy[i][j].rgbtRed = sr;
            img_copy[i][j].rgbtBlue = sb;
            img_copy[i][j].rgbtGreen = sg;
        }
    }

    //COPYING BACK

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
           image[i][j].rgbtRed = img_copy[i][j].rgbtRed;
           image[i][j].rgbtBlue = img_copy[i][j].rgbtBlue;
           image[i][j].rgbtGreen = img_copy[i][j].rgbtGreen;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
        RGBTRIPLE img_copy [height][width];
        int gx [3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
        int gy [3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};
        int copy, copyb;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int srx = 0, sbx = 0, sgx = 0, sry = 0, sby = 0, sgy = 0;
            int k = 0, l = 0, m = 0, n = 0, a = 0, b = 0, c = 2, d = 2;
            double R = 0, G = 0, B = 0;

            if (i == 0)
            {
                k = i;
                a = 1;
            }
            else
            {
                k = i - 1;
            }

            if (i == height - 1)
            {
                m = i;
                c = 1;
            }
            else
            {
               m = i + 1;
            }


            if (j == 0)
            {
                l = j;
                b = 1;
            }
            else
            {
                l = j - 1;
            }

            if (j == width - 1)
            {
                n = j;
                d = 1;
            }
            else
            {
                n = j + 1;
            }

            copy = l;
            copyb = b;

            for (; k <= m; k++)
            {
                if(a > c)
                {
                    break;
                }
                for (l = copy; l <= n; l++)
                {
                    if(b > d)
                    {
                        b = copyb;
                    }
                    srx = srx + image[k][l].rgbtRed * gx[a][b];
                    sbx = sbx + image[k][l].rgbtBlue * gx[a][b];
                    sgx = sgx + image[k][l].rgbtGreen * gx[a][b];

                    sry = sry + image[k][l].rgbtRed * gy[a][b];
                    sby = sby + image[k][l].rgbtBlue * gy[a][b];
                    sgy = sgy + image[k][l].rgbtGreen * gy[a][b];

                    b++;
                }
                a++;
            }

            R = sqrt( (double) ((srx * srx) + (sry * sry)));
            B = sqrt( (double) ((sbx * sbx) + (sby * sby)));
            G = sqrt( (double) ((sgx * sgx) + (sgy * sgy)));

            R = round(R);
            B = round(B);
            G = round(G);


            R = (R < 255 ? R : 255);
            B = (B < 255 ? B : 255);
            G = (G < 255 ? G : 255);

            img_copy[i][j].rgbtRed = R;
            img_copy[i][j].rgbtBlue = B;
            img_copy[i][j].rgbtGreen = G;
        }
    }

    //COPYING BACK

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
           image[i][j].rgbtRed = img_copy[i][j].rgbtRed;
           image[i][j].rgbtBlue = img_copy[i][j].rgbtBlue;
           image[i][j].rgbtGreen = img_copy[i][j].rgbtGreen;
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
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            sepiaRed = (sepiaRed < 255 ? sepiaRed : 255);
            sepiaBlue = (sepiaBlue < 255 ? sepiaBlue : 255);
            sepiaGreen = (sepiaGreen < 255 ? sepiaGreen : 255);

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
        }
    }
    return;
}