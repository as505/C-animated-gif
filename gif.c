#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define FILENAME "created.gif"


// Writes a color to a color table pointed to by fp
// Colors are defined as 3 bytes R, G, and B
// Each byte can store a value between 0 and 255
int write_color(unsigned char red, unsigned char green, unsigned char blue, FILE* fp) { 
    // Order for color bytes are Red, Greed, Blue
    unsigned char colors[] = {red, green, blue};
    int written = fwrite(colors, sizeof(unsigned char), 3, fp);
    if (written == 3){
        return 1;
    } else {
        return 0;
    };
}

/* 
Creates a global color table with 5 colors: Red, Green, Blue, Black, White
- fp is the Gif file being created
*/
int create_RGB_global_color_table(FILE* fp) {
    int count = 0;
    // Red
    count += write_color(255, 0, 0, fp);
    // Green
    count += write_color(0, 255, 0, fp);
    // Blue
    count += write_color(0, 0, 255, fp);
    // Black
    count += write_color(255, 255, 255, fp);
    // White
    count += write_color(0, 0, 0, fp);

    assert(count == 5);
    return 1;
}


/*
    Creates image descriptor for first frame/image
    Likely re-used for future frames
    
    For each image, exactly one image descriptor is required

    Left, top, width, and height should be short, as they each have 16 bits of data

*/
int write_image_descriptor(FILE* fp, unsigned short left, unsigned short top, unsigned short width, unsigned short height) {
    // Fixed value, marks begining of new image descriptor
    char image_seperator = 0x2c;
    /*
    unsigned short left;
    unsigned short top;
    unsigned short width;
    unsigned short height;
    */


    return 1;
}

// Write gif header
int write_header(FILE* fp, unsigned short width, unsigned short height) {
    // --------------------- Signature ---------------------
    char signature[] = "GIF";
    char version[] = "89a";
    fwrite(signature, sizeof(char), 3, fp);
    fwrite(version, sizeof(char), 3, fp);

    // --------------------- Screen descriptor ---------------------

    // Height and width
    unsigned short int widthHeight[] = {width, height};
    fwrite(widthHeight, sizeof(short int), 2, fp);
    
    // Packed fields
    /*
    <Packed Fields>     

        Global Color Table Flag       1 Bit
        Color Resolution              3 Bits
        Sort Flag                     1 Bit
        Size of Global Color Table    3 Bits
    */

    // 1 111 0 101
    // Global color table is present
    // 8 (7 + 1) bits of color data per pixel
    // Color table not sorted
    // 5 colors in table

    // 11110101 is B5 in hex 
    char packed_fields = 0xF5;
    fwrite(&packed_fields, 1, 1, fp);

    // Background color index
    // Indicates which color in the Global Color Table should be used as the background color
    char background_color_index = 0x01;
    fwrite(&background_color_index, 1, 1, fp);

    // Pixel aspect ratio
    // No aspect ratio data given
    char pixel_aspect_ratio = 0x00;
    fwrite(&pixel_aspect_ratio, 1, 1, fp);

    
    return 1;
}

// The trailer indicates the end of the Gif file
int write_trailer(FILE* fp) {
    // 0x3B is the fixed value used to mark the end of a GIF datastream
    char end = 0x3B;
    fwrite(&end, sizeof(char), 1, fp);

    return 1;
};

int debug_print_gif_file_data() {
    FILE* fp = fopen(FILENAME, "r");
    char signature[6];
    short int WH[2];
    char PF = 0;
    char bgci = 0;
    char pixelAR = 0;

    fread(signature, 6, 1, fp);
    fread(WH, sizeof(short), 2, fp);
    fread(&PF, 1, 1, fp);
    fread(&bgci, 1, 1, fp);
    fread(&pixelAR, 1, 1, fp);

    printf("\nSig: %s\nHW: %d %d\n", signature, WH[0], WH[1]);
    printf("Packed Fields: 0x%x\n", PF & 0xff);
    printf("Background color index: %d\n", bgci);
    printf("Pixel aspect ratio: %d\n", pixelAR);
    return 1;
}

int main() {

    // write to file
    printf("Writing header...\n");
    
    FILE* fp = fopen(FILENAME, "w");

    write_header(fp, 128, 256);
    // Use 5 color GCT
    create_RGB_global_color_table(fp);
    // write_data(fp);

    write_trailer(fp);

    fclose(fp);

    printf("File created!\n");
    printf("Displaying file information:\n");
    debug_print_gif_file_data();

    return 0;
}
