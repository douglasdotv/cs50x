#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int min_height = 1;
    int max_height = 8;
    int height;

    do
    {
        height = get_int("Height: ");
    }
    while (height < min_height || height > max_height);

    int space_counter = height - 1;
    int hash_counter = 1;

    for (int i = 1; i <= height; ++i)
    {
        for (int j = 1; j <= space_counter; ++j)
        {
            printf(" ");
        }

        for (int k = 1; k <= hash_counter; ++k)
        {
            printf("#");
        }

        printf("\n");

        space_counter--;
        hash_counter++;
    }
}
