#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Lenght of generated datastream, used for development/debugging
#define LENGHT 100
#define COLOR_TABLE_SIZE 5
// Debug file for LZW to write to
#define DEBUG_OUTPUT_STREAM "LZW_debug_out.txt"



/*
Gif files compress image data by using a variant of LZW compression.
Instead of storing RBG values, Gif files are encoded with indexes into a color table. 
The variant LZW algorithm can then be used to compress this stream if indexes into a stream of patterns.
*/


/*
    Generate a random stream of uncompressed Color Table indexes
    CTsize is the amount of colors in the color table
    Len is the number of indexes to generate
    
    ! The returned array must be freed by the consumer !
*/
unsigned char* create_random_index_stream(unsigned char CTsize, unsigned short len, unsigned int seed){
    // Set seed
    srand(seed);

    // Allocate memory for data
    unsigned char* index_stream = calloc(len, 1);
    // Always check for NULL after memory allocation
    assert(index_stream != NULL);
    
    // Populate array with random index values
    for(int i = 0; i < len; i++){    
        unsigned char index_val = (rand() % CTsize);
        index_stream[i] = index_val;
    }
    
    return index_stream;
}; 

/*
    Based on the number of entries in the color table, get the initial LZW minimum code size value
*/
unsigned char compute_LZW_minimum_code_size(unsigned char CTsize){

    // Minimum code size is 2 bits, even for black and white gifs with only 2 colors
    unsigned char num_bits;
    // Left-bitshift 1 to get largest possible number with 'N' bits until we have enough data for 'M' colors
    for(num_bits = 2; (1<<num_bits) < CTsize; num_bits++);

    return num_bits;
}


int LZW_compress(FILE* output_stream, unsigned char *input_stream, int num_chars){

}


main(){

    unsigned char* uncompressed_stream = create_random_index_stream(COLOR_TABLE_SIZE, LENGHT, 404);
    
    printf("Uncompressed stream:\n");
    for(int i = 0; i < LENGHT; i++){
        printf("%d", uncompressed_stream[i]);
        // Print formating
        if((i+1) % 16 == 0){
            printf("\n");
        }
    }
    printf("\n");

    compute_LZW_minimum_code_size(4);
    compute_LZW_minimum_code_size(8);
    compute_LZW_minimum_code_size(16);
    compute_LZW_minimum_code_size(32);
    compute_LZW_minimum_code_size(64);
    compute_LZW_minimum_code_size(128);
    compute_LZW_minimum_code_size(256);

    // Create debugg output stream for LZW output
    // Replaced with gif file pointer from gif.c 
    //FILE *d_fp = fopen(DEBUG_OUTPUT_STREAM, "w");



    // Free allocated memmory
    free(uncompressed_stream);
};