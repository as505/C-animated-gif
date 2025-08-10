#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/*
Gif files compress image data by using a variant of LZW compression.
Instead of storing RBG values, Gif files are encoded with indexes into a color table. 
The variant LZW algorithm can then be used to compress this stream if indexes into a stream of patterns.
*/


// Lenght of generated datastream, used for development/debugging
#define LENGHT 100
#define COLOR_TABLE_SIZE 5
// Debug file for LZW to write to
#define DEBUG_OUTPUT_STREAM "LZW_debug_out.txt"

// Array of color table indexes and compression codes
typedef struct {
    // Array of compression codes or color table indexes
    unsigned char *code_array;
    // Space in array
    unsigned int array_size;
    // Current array index
    unsigned int iter;
} code_table_t;


// Create new code table
code_table_t *create_code_table(int size){
    code_table_t new;
    new.array_size = size;
    new.iter = 0;
    // Start by allocating 'size' bits for the array
    // This is equal to the uncompressed data, and should be more than enough space
    // Dynamic array could be used to reduce memory usage, at a slight runtime cost
    unsigned char c_array = calloc(size, 1);

    // Return pointer to newly created struct
    return &new;
};

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

/*
Main part of LZW compression

    Output stream is where data is written, file pointer should already point to correct location in gif file

    Input stream and num chars describe the uncompressed index data

*/
int LZW_compress(FILE* output_stream, unsigned char *input_stream, int num_chars, unsigned char min_code_size, unsigned char num_colors){
    // Create empty code table
    code_table_t *table = create_code_table(num_chars);



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

    unsigned char min_code_size = compute_LZW_minimum_code_size(COLOR_TABLE_SIZE);
    
    // Create debugg output stream for LZW output
    // Replaced with gif file pointer from gif.c 
    FILE *d_fp = fopen(DEBUG_OUTPUT_STREAM, "w");
    
    LZW_compress(d_fp, uncompressed_stream, LENGHT, min_code_size);


    // Free allocated memmory
    free(uncompressed_stream);
};