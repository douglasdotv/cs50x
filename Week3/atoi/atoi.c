#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert(input));
}

int convert(string input)
{
    int input_len = strlen(input);

    if (input_len == 0)
    {
        return 0;
    }

    int last_digit = input[input_len - 1] - '0';
    input[input_len - 1] = '\0';

    return convert(input) * 10 + last_digit;
}
