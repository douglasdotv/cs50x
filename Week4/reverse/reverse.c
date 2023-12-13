#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

typedef uint8_t BYTE;

const int HEADER_SIZE = 44;

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse INPUT.wav OUTPUT.wav");
        return 1;
    }

    // Open input file for reading
    char *input_wav_file_name = argv[1];

    FILE *input_wav_file_ptr = fopen(input_wav_file_name, "r");
    if (input_wav_file_ptr == NULL)
    {
        printf("Could not open %s\n.", input_wav_file_name);
        return 1;
    }

    // Read header
    WAVHEADER wav_header;
    fread(&wav_header, sizeof(BYTE), HEADER_SIZE, input_wav_file_ptr);

    // Use check_format to ensure WAV format
    if (!check_format(wav_header))
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    char *reversed_wav_file_name = argv[2];

    FILE *reversed_wav_file_ptr = fopen(reversed_wav_file_name, "w");
    if (reversed_wav_file_ptr == NULL)
    {
        printf("Could not create %s\n.", reversed_wav_file_name);
        return 1;
    }

    // Write header to file
    fwrite(&wav_header, sizeof(BYTE), HEADER_SIZE, reversed_wav_file_ptr);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(wav_header);

    // Write reversed audio to file
    BYTE buffer[block_size];
    fseek(input_wav_file_ptr, -block_size, SEEK_END);

    while (ftell(input_wav_file_ptr) >= HEADER_SIZE)
    {
        fread(buffer, sizeof(BYTE), block_size, input_wav_file_ptr);
        fwrite(buffer, sizeof(BYTE), block_size, reversed_wav_file_ptr);
        fseek(input_wav_file_ptr, -block_size * 2, SEEK_CUR);
    }

    fclose(input_wav_file_ptr);
    fclose(reversed_wav_file_ptr);

    return 0;
}

int check_format(WAVHEADER header)
{
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 1;
    }

    return 0;
}

int get_block_size(WAVHEADER header)
{
    return header.numChannels * header.bitsPerSample / 8;
}
