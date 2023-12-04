#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int EXPECTED_ARGUMENTS = 2;
const int ALPHABET_SIZE = 26;

bool only_digits(string key_arg);
char rotate(char ch, int key);
char get_ascii_base(char ch);

int main(int argc, string argv[])
{
    string key_arg = argv[1];

    if (argc != EXPECTED_ARGUMENTS || !only_digits(key_arg))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int k = atoi(key_arg);
    string text = get_string("plaintext: ");
    int text_len = strlen(text);

    printf("ciphertext: ");
    for (int i = 0; i < text_len; ++i)
    {
        char curr_char = text[i];
        char encrypted_char = rotate(curr_char, k);
        printf("%c", encrypted_char);
    }
    printf("\n");

    return 0;
}

bool only_digits(string key_arg)
{
    int key_len = strlen(key_arg);

    for (int i = 0; i < key_len; ++i)
    {
        int ch = key_arg[i];
        if (!isdigit(ch))
        {
            return false;
        }
    }

    return true;
}

char rotate(char ch, int key)
{
    if (isalpha(ch))
    {
        char base = get_ascii_base(ch);
        return (ch - base + key) % ALPHABET_SIZE + base;
    }

    return ch;
}

char get_ascii_base(char ch)
{
    if (islower(ch))
    {
        return 'a';
    }

    return 'A';
}
