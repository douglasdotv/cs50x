// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

#include <cs50.h>
#include <stdio.h>

string replace(string word);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Error: the program accepts only one argument.");
        return 1;
    }

    string original_word = argv[1];
    string word_without_vowels = replace(original_word);
    printf("%s\n", word_without_vowels);
    return 0;
}

string replace(string word)
{
    for (int i = 0; word[i] != '\0'; ++i)
    {
        switch (word[i])
        {
            case 'a':
                word[i] = '6';
                break;
            case 'e':
                word[i] = '3';
                break;
            case 'i':
                word[i] = '1';
                break;
            case 'o':
                word[i] = '0';
                break;
            default:
                break;
        }
    }

    return word;
}
