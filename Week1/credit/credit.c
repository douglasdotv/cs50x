#include <cs50.h>
#include <stdio.h>

int get_leading_digits(long card_number);
int get_card_length(long card_number);
int luhn(long card_number);
void print_card_type(int leading_digits, int card_length, int checksum);

int main(void)
{
    long card_number = get_long("Enter credit card number: ");
    int leading_digits = get_leading_digits(card_number);
    int card_length = get_card_length(card_number);
    int checksum = luhn(card_number);

    print_card_type(leading_digits, card_length, checksum);
}

int get_leading_digits(long card_number)
{
    while (card_number >= 100)
    {
        card_number /= 10;
    }

    return (int) card_number;
}

int get_card_length(long card_number)
{
    int length = 0;

    while (card_number > 0)
    {
        card_number /= 10;
        length++;
    }

    return length;
}

int luhn(long card_number)
{
    int sum = 0;
    int is_second_digit = 0;

    while (card_number > 0)
    {
        int digit = card_number % 10;

        if (is_second_digit)
        {
            digit *= 2;
            sum += digit / 10 + digit % 10;
        }
        else
        {
            sum += digit;
        }

        card_number /= 10;
        is_second_digit = !is_second_digit;
    }

    return sum % 10;
}

void print_card_type(int leading_digits, int card_length, int checksum)
{
    if (checksum != 0)
    {
        printf("INVALID\n");
        return;
    }

    if ((leading_digits == 34 || leading_digits == 37) && card_length == 15)
    {
        printf("AMEX\n");
    }
    else if ((leading_digits >= 51 && leading_digits <= 55) && card_length == 16)
    {
        printf("MASTERCARD\n");
    }
    else if ((leading_digits / 10 == 4) && (card_length == 13 || card_length == 16))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

    return;
}
