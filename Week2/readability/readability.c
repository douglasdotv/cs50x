#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float get_coleman_liau_index(int letters, int words, int sentences);
void print_result(int index);

int main(void)
{
    string text = get_string("Text: ");

    int total_letters = count_letters(text);
    int total_words = count_words(text);
    int total_sentences = count_sentences(text);

    float coleman_liau_index = get_coleman_liau_index(total_letters, total_words, total_sentences);

    print_result(round(coleman_liau_index));
}

int count_letters(string text)
{
    int letter_count = 0;

    for (int i = 0; text[i] != '\0'; ++i)
    {
        if (isalpha(text[i]))
        {
            letter_count++;
        }
    }

    return letter_count;
}

int count_words(string text)
{
    int word_count = 0;

    if (isalpha(text[0]))
    {
        word_count++;
    }

    for (int i = 0; text[i] != '\0'; ++i)
    {
        if ((text[i] == ' ' || text[i] == '"') && isalpha(text[i + 1]))
        {
            word_count++;
        }
    }

    return word_count;
}

int count_sentences(string text)
{
    int sentence_count = 0;

    for (int i = 0; text[i] != '\0'; ++i)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentence_count++;
        }
    }

    return sentence_count;
}

float get_coleman_liau_index(int letters, int words, int sentences)
{
    float L = ((float) letters / (float) words) * 100;
    float S = ((float) sentences / (float) words) * 100;
    return 0.0588 * L - 0.296 * S - 15.8;
}

void print_result(int index)
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

    return;
}
