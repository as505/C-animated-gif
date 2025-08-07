#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/*
Gif files compress image data by using a variant of LZW compression.
Instead of storing RBG values, Gif files are encoded with indexes into a color table. 
The variant LZW algorithm can then be used to compress this stream if indexes into a stream of patterns.
*/


/*
    Generate a random stream of uncompressed Color Table indexes
    CTsize is the amount of colors in the color table
    Len is the number of indexes to generate
    
    The returned array must be freed by the consumer!
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

#define LENGHT 100

main(){

    unsigned char* uncompressed_stream = create_random_index_stream(5, LENGHT, 404);
    
    printf("Uncompressed stream:\n");
    for(int i = 0; i < LENGHT; i++){
        printf("%d", uncompressed_stream[i]);
    }
    printf("\n");

    // Free allocated memmory
    free(uncompressed_stream);
};