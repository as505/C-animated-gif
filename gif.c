#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

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
    fwrite(&image_seperator, 1, 1, fp);
    
    // Write image possition and dimentions
    unsigned short image[] = {left, top, width, height};
    fwrite(image, 2, 4, fp);
    
    // Write packed fields
    /*
    1 bit    No local color table
    0 

    1 bit    Image Not interlaced
    0

    1 bit    Local color table not sorted, nor in use
    0

    2 bits   Reserved
    00

    3bits   Size of local color table is 0, since local color table is not in use
    000

    Since local color table is not in use, packed field byte is 00000000
    */
    char packed_fields = 0x00;
    fwrite(&packed_fields, 1, 1, fp);


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
    unsigned char PF = 0;
    char bgci = 0;
    char pixelAR = 0;

    fread(signature, 6, 1, fp);
    fread(WH, sizeof(short), 2, fp);
    fread(&PF, 1, 1, fp);
    fread(&bgci, 1, 1, fp);
    fread(&pixelAR, 1, 1, fp);

    // Use bitmask to unpack packed fields for printing
    unsigned char GCT    = PF & 0b10000000;
    unsigned char colRes = PF & 0b01110000;
    unsigned char sort   = PF & 0b00001000;
    // Double since we use pow()
    double d_size   = PF & 0b00000111;
    // For GCT size, add 1 to the value stored, and raise 2 to the power of this value
    d_size = pow(2, d_size+1);
    int size = (int) d_size;


    printf("Header:\n");
    printf("\nSig: %s\nHW: %d %d\n", signature, WH[0], WH[1]);
    printf("Packed Fields: 0x%x\n", PF);
    // Bitshift packed fields to the right so only relevant field is printed. 
    // Add one to color value to display actual color resolution value
    printf("GCT: %d, Color Resolution: %d, Sort: %d, Size: %d\n", (GCT >> 7), (colRes >> 4) + 1, (sort >> 3), size);
    printf("Background color index: %d\n", bgci);
    printf("Pixel aspect ratio: %d\n", pixelAR);
    printf("---------------------\n");
    printf("Global Color Table:\n\n");

    // Create array to hold Global Color Table
    unsigned char *colors = calloc(1, size);
    assert(colors != NULL);

    // Read Global Color Table
    fread(colors, (5*3), sizeof(unsigned char), fp);
    // Print Table
    for (int i = 0; i < 5*3; i++){
        printf("Col %d:", i/3);
        printf("\t%d", colors[i]);
        i = i++;
        printf("\t%d", colors[i]);
        i = i++;
        printf("\t%d\n", colors[i]);
    }
    // Free malloc
    free(colors);
    printf("---------------------\n");

    printf("Image Descriptor:\n\n");
    
    char image_seperator;
    unsigned short image_params[4]; 
    char packed_fields;

    fread(&image_seperator, 1, 1, fp);
    fread(image_params, 2, 4, fp);
    fread(&packed_fields, 1, 1, fp);

    printf("Image Seperator 0x2c: %x\n", image_seperator);
    printf("Left: %d, Top: %d, Width: %d, Height: %d\n", image_params[0], image_params[1], image_params[2], image_params[3]);
    printf("Packed Fields: 0x%x\n", packed_fields);


    return 1;
}

int main() {

    // write to file
    printf("Writing header...\n");
    
    FILE* fp = fopen(FILENAME, "w");

    write_header(fp, 128, 128);
    // Use 5 color GCT
    create_RGB_global_color_table(fp);
    write_image_descriptor(fp, 0, 0, 128, 128);

    write_trailer(fp);

    fclose(fp);

    printf("File created!\n");
    printf("Displaying file information:\n");
    debug_print_gif_file_data();

    return 0;
}
