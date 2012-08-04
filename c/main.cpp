#include "encode.h"
#include <gd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    if (argc < 2) return 0;

    bool bits[16][16];
    gdImage* foo;
    FILE *in;
    in = fopen(argv[2], "r");
    foo = gdImageCreateFromPng(in);
 
    fclose(in);

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            bits[i][j] = gdImageGetPixel(foo,j,i) ? 0 : 1;
        }
    }

    unsigned char qrCode[QR_CODE_SIZE][QR_CODE_SIZE]; // = (unsigned char**) malloc(QR_CODE_SIZE * QR_CODE_SIZE);
    encode(&qrCode[0], argv[1], bits);
  
    return 0;
}

