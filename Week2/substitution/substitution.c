#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool validate_key(string key, int key_len);
string to_ciphertext(string text, string key);
char get_ascii_base(char ch);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];
    int key_len = strlen(key);

    if (!validate_key(key, key_len))
    {
        return 1;
    }

    string plaintext = get_string("plaintext: ");
    string ciphertext = to_ciphertext(plaintext, key);

    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

bool validate_key(string key, int key_len)
{
    if (key_len != 26)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }

    for (int i = 0; i < key_len; ++i)
    {
        if (!isalpha(key[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return false;
        }
    }

    for (int i = 0; i < key_len - 1; ++i)
    {
        for (int j = i + 1; j < key_len; ++j)
        {
            if (tolower(key[i]) == tolower(key[j]))
            {
                printf("Key must not contain repeated characters.\n");
                return false;
            }
        }
    }

    return true;
}

string to_ciphertext(string text, string key)
{
    int text_len = strlen(text);

    for (int i = 0; i < text_len; ++i)
    {
        char curr_char = text[i];
        if (isalpha(curr_char))
        {
            char base = get_ascii_base(curr_char);
            int char_index = curr_char - base;
            int encrypted_char = key[char_index];
            text[i] = islower(curr_char) ? tolower(encrypted_char) : toupper(encrypted_char);
        }
    }

    return text;
}

char get_ascii_base(char ch)
{
    if (islower(ch))
    {
        return 'a';
    }

    return 'A';
}
