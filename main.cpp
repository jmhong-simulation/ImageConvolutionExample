#include <stdio.h>
#include <stdlib.h>
#include "BMPImage.h"
#include <stdbool.h>
#include <assert.h>

#define _CRT_SECURE_NO_WARINGS

RGB getPixelColor(const RGB* rgb_array, const int res_x, const int res_y, int i, int j)
{
    // repeating boundary
    if (i < 0) i += res_x;
    if (j < 0) j += res_y;

    i = i % res_x;
    j = j % res_y;

    const int ix = i + res_x * j;

    assert(ix >= 0);
    assert(ix < res_x*res_y);

    return rgb_array[ix];
}

void main()
{   
    int res_x, res_y;

    //unsigned char* rgb_array = NULL;
    RGB *rgb_array = NULL;

    readBMP24("CMIP.bmp", &res_x, &res_y, &rgb_array);

    RGB *rgb_temp = (RGB*)malloc(sizeof(RGB)*res_x*res_y);

    float conv_mat[3][3] = { -1, -1, -1,
                            -1, 8, -1,
                            -1, -1, -1 };

    //for (int sub_j = 0; sub_j < 3; sub_j++)
    //for (int sub_i = 0; sub_i < 3; sub_i ++)
    //{ 
    //    conv_mat[sub_i][sub_j] /= 16.0f;
    //}

    for (int sub = 0; sub < 9; sub++)
        (&conv_mat[0][0])[sub] /= 16.0f;


    for (int r = 0; r < 1; r++)// repeated smoothing
    {
        for (int j = 0; j < res_y; j++)
            for (int i = 0; i < res_x; i++)
            {
                RGB color_temp = { 0.0f, 0.0f, 0.0f };

                for (int sub_j = 0; sub_j < 3; sub_j++)
                    for (int sub_i = 0; sub_i < 3; sub_i++)
                    {
                        RGB color_neighbor = getPixelColor(rgb_array, res_x, res_y, i + sub_i - 1, j + sub_j - 1);

                        color_neighbor.red_ *= conv_mat[sub_i][sub_j];
                        color_neighbor.green_ *= conv_mat[sub_i][sub_j];
                        color_neighbor.blue_ *= conv_mat[sub_i][sub_j];

                        color_temp.red_ += color_neighbor.red_;
                        color_temp.green_ += color_neighbor.green_;
                        color_temp.blue_ += color_neighbor.blue_;
                    }

                rgb_temp[i + res_x * j] = color_temp;
            }

        for (int j = 0; j < res_y; j++)
            for (int i = 0; i < res_x; i++)
            {
                rgb_array[i + res_x * j] = rgb_temp[i + res_x * j];
            }
    }

    writeBMP24("output.bmp", res_x, res_y, rgb_temp);
  

    free(rgb_array);
    free(rgb_temp);
}
