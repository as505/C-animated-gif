#include <stdlib.h>
#include <stdio.h>

#define FILENAME "created.gif"



// Write gif header
int write_header(FILE* fp) {
    // Version
    char signature[] = "GIF87a";
    fwrite(signature, sizeof(char), 7, fp);

    // Global descriptor
    // Height and width
    short int widthHeight[] = {128, 256};
    int a = fwrite(widthHeight, sizeof(short int), 2, fp);
    printf("\n WH: %d\n", a);
    return 1;
}

int test_header() {
    FILE* fp = fopen(FILENAME, "r");
    char signature[7];
    short int WH[2];
    int test = fread(signature, 7, 1, fp);
    int test2 = fread(WH, 2, 2, fp);
    printf("\n test: %d, %d", test, test2);

    printf("\nSig: %s\n, HW: %d %d\n", signature, WH[0], WH[1]);
    return 1;
}

int main() {

    FILE* fp = fopen(FILENAME, "w");
    // write to file
    printf("Writing header");
    write_header(fp);
    fclose(fp);

    printf("File created!\n");
    printf("Testing...\n");
    test_header();

    return 0;
}
