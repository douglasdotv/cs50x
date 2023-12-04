#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);
void print_as_bin(int dec);

int main(void)
{
    string message = get_string("Enter a message: ");
    int message_length = strlen(message);
    int dec_char;

    for (int i = 0; i < message_length; ++i)
    {
        dec_char = message[i];
        print_as_bin(dec_char);
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}

void print_as_bin(int dec)
{
    int bits[BITS_IN_BYTE] = {0};

    for (int i = BITS_IN_BYTE - 1; i >= 0; --i)
    {
        bits[i] = dec % 2;
        dec /= 2;
    }

    for (int i = 0; i < BITS_IN_BYTE; ++i)
    {
        print_bulb(bits[i]);
    }
    printf("\n");

    return;
}
