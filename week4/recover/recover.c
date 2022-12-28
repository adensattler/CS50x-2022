#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;       // casts BYTE to improve readbility!

int main(int argc, char *argv[])
{
    // insures proper usage of the program
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // insures that the file could actually be opened
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // INITIALIZING VARIABLES!
    int jpg_count = 0;                  // counter to keep track of the amount of images found
    int BLOCK_SIZE = 512;               // the size of a jpg image in bytes
    BYTE buffer[BLOCK_SIZE];            // creates an empty array of 512 bytes
    char filename[9];                   // creates an empty array of 9 characters in memory called filename (7 characters for ###.jpg and 1 for the null character)
    FILE *img = NULL;                   // initializes what will be the opened image file that is being written to. initializing it outside of the loop allows for its unconstrained use in the program


    // reads the raw file byte by byte into the buffer array for the size of a jpg file. repeats every jpg block until the end of the memory card.
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // if the start of a new jpg is found...
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff & (buffer[3] & 0xf0) == 0xe0)       // while reading the raw file in 512 byte blocks, if the first four bytes are the same as a jpg header then we found a the start of a new jpg!
        {
            // if the jpg found is the first one to be found...
            if (jpg_count == 0)
            {
                sprintf(filename, "%03i.jpg", jpg_count);           // formats the jpg_count into three digits for use as the filename! // utilizes sprintf to write the name of the current jpg file into the filename array, character by character.
                img = fopen(filename, "w");                         // creates a file with the filename determined above. enables writing to the file.
                fwrite(&buffer, 1, BLOCK_SIZE, img);                // writes each byte from the buffer array to the output file!
                jpg_count++;                                        //increment jpg_counter
            }
            // otherwise, if you find a new jpg that isn't the first...
            else
            {
                // close the current file, open a new file a begin writing to it
                fclose(img);
                sprintf(filename, "%03i.jpg", jpg_count);       // create a new, incremented filename
                img = fopen(filename, "w");                   // creates a file with the filename determined above. enables writing to the file.
                fwrite(&buffer, 1, BLOCK_SIZE, img);
                jpg_count++;                                        //increment jpg_counter
            }
        }
        else if (img != NULL) // if there is already an image being written to...
        {
            fwrite(&buffer, 1, BLOCK_SIZE, img);            // write another black to it!
        }
    }
    fclose(img);
    fclose(raw_file);
}

// rm -f *.jpg