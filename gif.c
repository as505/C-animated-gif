#include <stdlib.h>
#include <stdio.h>

#define FILENAME "created.gif"



// Write gif header
int write_header(FILE* fp) {
    // --------------------- Signature ---------------------
    char signature[] = "GIF";
    char version[] = "89a";
    fwrite(signature, sizeof(char), 3, fp);
    fwrite(version, sizeof(char), 3, fp);

    // --------------------- Screen descriptor ---------------------

    // Height and width
    short int widthHeight[] = {128, 256};
    fwrite(widthHeight, sizeof(short int), 2, fp);
    
    // Packed fields
    /*
    <Packed Fields>     

        Global Color Table Flag       1 Bit
        Color Resolution              3 Bits
        Sort Flag                     1 Bit
        Size of Global Color Table    3 Bits
    */

    // 0 011 0 000
    // No global color table
    // 4 (3 + 1) bits of color data per pixel
    // Color table not sorted
    // No global color table, size 0

    // 0011000 is 40 in hex 
    char packed_fields = 0x40;
    fwrite(&packed_fields, 1, 1, fp);

    // Background color index
    // Since global color table is set to zero, this field should also be zero
    char background_color_index = 0x00;
    fwrite(&background_color_index, 1, 1, fp);

    // Pixel aspect ratio
    // No aspect ratio data given
    char pixel_aspect_ratio = 0x00;
    fwrite(&background_color_index, 1, 1, fp);

    
    return 1;
}

// The trailer indicates the end of the Gif file
int write_trailer(FILE* fp) {
    // 0x3B is the fixed value used to mark the end of a GIF datastream
    char end = 0x3B;
    fwrite(&end, sizeof(char), 1, fp);

    return 1;
};

int test_header() {
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
    printf("PF: %x\n", PF & 0xff);
    printf("Background color index: %d\n", bgci);
    printf("Pixel aspect ratio: %d\n", pixelAR);
    return 1;
}

int main() {

    // write to file
    printf("Writing header...\n");
    
    FILE* fp = fopen(FILENAME, "w");

    write_header(fp);

    // write_data(fp);

    write_trailer(fp);

    fclose(fp);

    printf("File created!\n");
    printf("Displaying file information:\n");
    test_header();

    return 0;
}
