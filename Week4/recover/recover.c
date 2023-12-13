#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }

    char *raw_file_name = argv[1];
    FILE *raw_file = fopen(raw_file_name, "r");

    if (raw_file == NULL)
    {
        printf("Could not open file %s.\n", raw_file_name);
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    FILE *jpg_file = NULL;
    char *jpg_file_name;
    int jpg_file_count = 0;

    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpg_file != NULL)
            {
                fclose(jpg_file);
                free(jpg_file_name);
            }

            jpg_file_name = malloc(8 * sizeof(char));

            if (jpg_file_name == NULL)
            {
                printf("Memory allocation for jpg_file_name failed.\n");
                fclose(raw_file);
                return 1;
            }

            sprintf(jpg_file_name, "%03i.jpg", jpg_file_count++);

            jpg_file = fopen(jpg_file_name, "w");

            if (jpg_file == NULL)
            {
                printf("Could not create a new jpg file.\n");
                fclose(raw_file);
                free(jpg_file_name);
                return 1;
            }
        }

        if (jpg_file != NULL)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, jpg_file);
        }
    }

    fclose(jpg_file);
    free(jpg_file_name);
    fclose(raw_file);

    return 0;
}
