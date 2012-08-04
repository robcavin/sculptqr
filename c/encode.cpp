/* 
 * File:   main.cpp
 * Author: Rob
 *
 * Created on November 5, 2011, 1:54 PM
 */

//#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "encode.h"

#include <gd.h>
//using namespace std;

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

//#define QR_CODE_SIZE  37
//#define QR_CODE_SIZE  29
#define QR_VERSION 3
#define QR_ECC_LEVEL L
#define QR_ALPHANUM_CHARS 154
#define QR_LEN_BITS 8                   // 9 for mode 1 to 9, 11 for mode 10
//#define QR_DATA_BITS 152
//#define QR_DATA_BITS 440                // 152 for mode 1
#define QR_DATA_BITS 864                // 152 for mode 1

#define REMAINDER_BYTE 1
#define REMAINDER_BITS 7

#define QR_PAD_BYTE_1 0xEC
#define QR_PAD_BYTE_2 0x11

//#define QR_ECC_CODE_WORDS 7+1
//#define QR_ECC_CODE_WORDS 15+1
#define QR_ECC_CODE_WORDS 26+1

//#define QR_GEN_EQ_COEFF {0,87,229,146,149,238,102,21};
//#define QR_GEN_EQ_COEFF {0,251,67,46,61,118,70,64,94,32,45};
//#define QR_GEN_EQ_COEFF {0,8,183,61,91,202,37,51,58,58,237,140,124,5,99,105}
#define QR_GEN_EQ_COEFF {0,173,125,158,2,103,182,118,17,145,201,111,28,165,53,161,21,245,142,13,102,48,227,153,145,218,70}

//#define QR_GEN_EQ_COEFF {1,246,51,183,4,136,98,199,152,77,56,206,24,145,40,209,117,233,42,135,68,70,144,146,77,43,94}

#define ALIGNMENT_PATTERN 28

#define MASK0 !((i + j)%2)
#define MASK1 !(i % 2)
#define MASK2 !(j % 3)
#define MASK3 !((i + j)%3)
#define MASK4 !((i/2 + j/3)%2)
#define MASK5 !((i*j)%2+(i*j)%3)
#define MASK6 !(((i*j)%3+i*j)%2)
#define MASK7 !(((i*j)%3+i+j)%2)

#define ECC_L 0x1
#define ECC_M 0x0
#define ECC_Q 0x3
#define ECC_H 0x2

struct pixel_map {
    int i;
    int j;
    int bit;
};

pixel_map pixel_map_array[16*16] = {
    {0,15,291},
    {0,14,292},
    {1,15,293},
    {1,14,294},
    {2,15,295},
    {2,14,296},
    {3,15,297},
    {3,14,298},
    {4,15,299},
    {4,14,300},
    {5,15,301},
    {5,14,302},
    {6,15,303},
    {6,14,304},
    {7,15,305},
    {7,14,306},
    {8,15,307},
    {8,14,308},
    {9,15,309},
    {9,14,310},
    {10,15,311},
    {10,14,312},
    {11,15,313},
    {11,14,314},
    {12,15,315},
    {12,14,316},
    {13,15,317},
    {13,14,318},
    {14,15,319},
    {14,14,320},
    {15,15,321},
    {15,14,322},
    {15,13,363},
    {15,12,364},
    {14,13,365},
    {14,12,366},
    {13,13,367},
    {13,12,368},
    {12,13,369},
    {12,12,370},
    {11,13,371},
    {11,12,372},
    {10,13,373},
    {10,12,374},
    {9,13,375},
    {9,12,376},
    {8,13,377},
    {8,12,378},
    {7,13,379},
    {7,12,380},
    {6,13,381},
    {6,12,382},
    {5,13,383},
    {5,12,384},
    {4,13,385},
    {4,12,386},
    {3,13,387},
    {3,12,388},
    {2,13,389},
    {2,12,390},
    {1,13,391},
    {1,12,392},
    {0,13,393},
    {0,12,394},
    {0,11,435},
    {0,10,436},
    {1,11,437},
    {1,10,438},
    {2,11,439},
    {2,10,440},
    {3,11,441},
    {3,10,442},
    {4,11,443},
    {4,10,444},
    {5,11,445},
    {5,10,446},
    {6,11,447},
    {6,10,448},
    {7,11,449},
    {7,10,450},
    {8,11,451},
    {8,10,452},
    {9,11,453},
    {9,10,454},
    {10,11,455},
    {10,10,456},
    {11,11,457},
    {11,10,458},
    {12,11,459},
    {12,10,460},
    {13,11,461},
    {13,10,462},
    {14,11,463},
    {14,10,464},
    {15,11,465},
    {15,10,466},
    {15,9,507},
    {15,8,508},
    {14,9,509},
    {14,8,510},
    {13,9,511},
    {13,8,512},
    {12,9,513},
    {12,8,514},
    {11,9,515},
    {11,8,516},
    {10,9,517},
    {10,8,518},
    {9,9,519},
    {9,8,520},
    {8,9,521},
    {8,8,522},
    {7,9,523},
    {7,8,524},
    {6,9,525},
    {6,8,526},
    {5,9,527},
    {5,8,528},
    {4,9,529},
    {4,8,530},
    {3,9,531},
    {3,8,532},
    {2,9,533},
    {2,8,534},
    {1,9,535},
    {1,8,536},
    {0,9,537},
    {0,8,538},
    {0,7,579},
    {0,6,580},
    {1,7,581},
    {1,6,582},
    {2,7,583},
    {2,6,584},
    {3,7,585},
    {3,6,586},
    {4,7,587},
    {4,6,588},
    {5,7,589},
    {5,6,590},
    {6,7,591},
    {6,6,592},
    {7,7,593},
    {7,6,594},
    {8,7,595},
    {8,6,596},
    {9,7,597},
    {9,6,598},
    {10,7,599},
    {10,6,600},
    {11,7,601},
    {11,6,602},
    {12,7,603},
    {12,6,604},
    {13,7,605},
    {13,6,606},
    {14,7,607},
    {14,6,608},
    {15,7,609},
    {15,6,610},
    {15,5,651},
    {15,4,652},
    {14,5,653},
    {14,4,654},
    {13,5,655},
    {13,4,656},
    {12,5,657},
    {12,4,658},
    {11,5,659},
    {11,4,660},
    {10,5,661},
    {10,4,662},
    {9,5,663},
    {9,4,664},
    {8,5,665},
    {8,4,666},
    {7,5,667},
    {7,4,668},
    {6,5,669},
    {6,4,670},
    {5,5,671},
    {5,4,672},
    {4,5,673},
    {4,4,674},
    {3,5,675},
    {3,4,676},
    {2,5,677},
    {2,4,678},
    {1,5,679},
    {1,4,680},
    {0,5,681},
    {0,4,682},
    {0,3,723},
    {0,2,724},
    {1,3,725},
    {1,2,726},
    {2,3,727},
    {2,2,728},
    {3,3,729},
    {3,2,730},
    {4,3,731},
    {4,2,732},
    {5,3,733},
    {5,2,734},
    {6,3,735},
    {6,2,736},
    {7,3,737},
    {7,2,738},
    {8,3,739},
    {8,2,740},
    {9,3,741},
    {9,2,742},
    {10,3,743},
    {10,2,744},
    {11,3,745},
    {11,2,746},
    {12,3,747},
    {12,2,748},
    {13,3,749},
    {13,2,750},
    {14,3,751},
    {14,2,752},
    {15,3,753},
    {15,2,754},
    {15,1,795},
    {15,0,796},
    {14,1,797},
    {14,0,798},
    {13,1,799},
    {13,0,800},
    {12,1,801},
    {12,0,802},
    {11,1,803},
    {11,0,804},
    {10,1,805},
    {10,0,806},
    {9,1,807},
    {9,0,808},
    {8,1,809},
    {8,0,810},
    {7,1,811},
    {7,0,812},
    {6,1,813},
    {6,0,814},
    {5,1,815},
    {5,0,816},
    {4,1,817},
    {4,0,818},
    {3,1,819},
    {3,0,820},
    {2,1,821},
    {2,0,822},
    {1,1,823},
    {1,0,824},
    {0,1,825},
    {0,0,826},
};

char *QRasciiMap = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

unsigned char alphaTable[256] = {1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1};
unsigned char revAlphaTable[256] = {0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175};
unsigned char ecc_coeff[QR_ECC_CODE_WORDS] = QR_GEN_EQ_COEFF;

unsigned char* encode(unsigned char qrCode[][QR_CODE_SIZE], char* message, bool bits[16][16]) {
    
    for (int i = 0; i < QR_ECC_CODE_WORDS; i++) {
        printf("%d,", alphaTable[ecc_coeff[i]]);
    }
    printf("\n");
    
    //for (int i = 0; message[i]; i++)
    //    message[i] = toupper(message[i]);

    int message_len = strlen(message);

    if (message_len > QR_ALPHANUM_CHARS) { //(2<<9)) {  // length encoded as 9 bits in alphanumeric v.1-9
        printf("Encoded string is too long");
        return 0;
    }

    // STEP 1 - Convert character pairs to 11bit values
    // XXX - Could combine step 1 and 2 to save memory
    /*char* temp_char;
    unsigned char ascii_val;
    int charPairStreamLen = message_len / 2 + 1;
    short* charPairStream = (short *) malloc(charPairStreamLen); // XXX - name this differently
    short* pCharPairStream = charPairStream;

    for (int i = 0; i < message_len; i++) {
        temp_char = strchr(QRasciiMap, message[i]); // Return val is pointer to char
        if (!temp_char) {
            printf("Character %c is being replaced by .\n", message[i]);
            ascii_val = 42;
        } else {
            ascii_val = (int) (temp_char - QRasciiMap);
        }
        if (i & 0x1) {
            *pCharPairStream = 45 * (*pCharPairStream) + ascii_val;
            pCharPairStream++;
        } else {
            *pCharPairStream = ascii_val;
        }
    }

    // if length is odd, the last ascii value needs to shift left, not a normal *45
    if (message_len & 0x1) {
        *pCharPairStream <<= 5;
    }

    for (int i = 0; i < charPairStreamLen; i++) {
        for (int shift = 10; shift >= 0; shift--) printf("%d", (charPairStream[i] >> shift) & 0x1);
        printf(" ");
    }
    printf("\n");
     */


    // Step 2 - Convert into compact byte stream and pad as necessary
    //
    int modePlusLenBitLength = 4 + QR_LEN_BITS;
    int charPairStreamBitLength = (message_len >> 1) * 11 + (message_len & 0x1) * 6;
    int charPairStream0Pad = MIN(4, QR_DATA_BITS - charPairStreamBitLength);
    int byteStreamLength = modePlusLenBitLength + ((charPairStreamBitLength + charPairStream0Pad + 7) >> 3);

    //char* byteStream = (char *) malloc(byteStreamLength);
    unsigned char polyLen = QR_DATA_BITS / 8 + QR_ECC_CODE_WORDS - 1;
    unsigned char* byteStream = (unsigned char *) malloc(polyLen + REMAINDER_BYTE);

    for (int i = 0; i < polyLen + REMAINDER_BYTE; i++) byteStream[i] = 0; // 0 out all bytes

    unsigned char* pByteStream = byteStream;

    //short *lastPair = pCharPairStream;
    //pCharPairStream = charPairStream;

    int bitPos = 0;
    int prevBitPos = 0;

    // First add type indicator and length bits
    *pByteStream |= 0x40; // 0b0100 in bits 7:4 indicates alphanumeric encoding
    bitPos = 4;
    *pByteStream |= (message_len >> (QR_LEN_BITS - (8 - bitPos)));
    *pByteStream++;
    *pByteStream |= (message_len << (16 - QR_LEN_BITS - bitPos));
    bitPos = bitPos + QR_LEN_BITS - 8;


    /*while (pCharPairStream <= lastPair) {
        *pByteStream |= (*pCharPairStream >> (11 - (8 - bitPos)));
        pByteStream++;
        *pByteStream |= ((16 - 11 - bitPos) >= 0) ? (*pCharPairStream << (16 - 11 - bitPos)) : (*pCharPairStream >> -(16 - 11 - bitPos));

        prevBitPos = bitPos;
        bitPos += 11 - 8;

        if (bitPos >= 8) {
            pByteStream++;
            if (bitPos > 8) {
                *pByteStream |= (*pCharPairStream << (24 - 11 - prevBitPos));
            }
            bitPos -= 8;
        }

        pCharPairStream++;
    }
    */
    for (int i = 0; i < message_len; i++) {
        *pByteStream |= (message[i] >> bitPos);
        pByteStream++;
        *pByteStream |= (message[i] << (8 - bitPos));
    }

    unsigned char padByteToggle = 0;
    while ((++pByteStream - byteStream) < (QR_DATA_BITS / 8)) {
        *pByteStream = (padByteToggle) ? QR_PAD_BYTE_2 : QR_PAD_BYTE_1;
        padByteToggle = !padByteToggle;
    }
    
    // Inject picture bits
    for (int index = 0; index < 16*16; index++) {
        int bit = pixel_map_array[index].bit;
        int j = pixel_map_array[index].j + 11;
        bool set = bits[pixel_map_array[index].i][pixel_map_array[index].j] ^ MASK2;
        unsigned char bitmask = 0x80 >> (bit % 8);
        if (!set) bitmask = ~bitmask;
        int byte_index = bit / 8;
        printf("%d %d %d %d %x %x ", bit, pixel_map_array[index].i, pixel_map_array[index].j, bits[pixel_map_array[index].i][pixel_map_array[index].j], bitmask, byteStream[byte_index]);
        byteStream[byte_index] = set ? (byteStream[byte_index] | bitmask) : (byteStream[byte_index] & bitmask);
        printf("%x \n", byteStream[byte_index]);
    }

    // Here we 
    for (int i = 0; i < QR_DATA_BITS / 8; i++) {
        for (int shift = 7; shift >= 0; shift--) printf("%d", (byteStream[i] >> shift) & 0x1);
        //printf(" ");
    }
    printf("\n\n");

    unsigned char *poly = (unsigned char*) malloc(polyLen);

    for (int i = 0; i < QR_DATA_BITS / 8; i++) poly[i] = byteStream[i];
    for (int i = QR_DATA_BITS / 8; i < polyLen; i++) poly[i] = 0;
//    for (int i = 15; i < polyLen; i++) poly[i] = 0;
    
    for (int i = 0; i < polyLen; i++) {
        printf("%d,", poly[i]);
        //printf(" ");
    }
    printf("\n\n");
    
    unsigned char divisor;
    for (unsigned char divisorIndex = 0; (polyLen - divisorIndex) >= QR_ECC_CODE_WORDS; divisorIndex++) {
        if (!(poly[divisorIndex])) continue;
        divisor = revAlphaTable[poly[divisorIndex]];
        for (int i = 0; i < QR_ECC_CODE_WORDS; i++) {

            poly[i + divisorIndex] = alphaTable[(ecc_coeff[i] + divisor) % 255] ^ poly[i + divisorIndex];
        }
        for (int i = 0; i < polyLen; i++) {
            printf("%d,", poly[i]);
        }
        printf("\n\n");
    }

    for (int i = polyLen - QR_ECC_CODE_WORDS - 1; i < polyLen; i++) byteStream[i] = poly[i];
    free(poly);

    // XXX - This is pretty hardcoded stuff right now
    // Calculate bit stream for ECC level, mask
    char levelMask[5] = {0,1,0,1,0};
    char levelMaskPoly[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // L, mask 0
    for (int i = 0; i < 5; i++) levelMaskPoly[i] = levelMask[i];  // Copy data bits back in
    char levelMaskXOR[15] =  {1,0,1,0,1,0,0,0,0,0,1,0,0,1,0};
    char levelMaskGenPoly[11] = {1,0,1,0,0,1,1,0,1,1,1};
    for (unsigned char divisorIndex = 0; (15 - divisorIndex) >= 11; divisorIndex++) {
        divisor = levelMaskPoly[divisorIndex];  // Should only be 1 or -1?
        for (int i = 0; i < 11; i++) {
            levelMaskPoly[i + divisorIndex] ^= (divisor * levelMaskGenPoly[i]);
        }
        for (int i = 0; i < 15; i++) printf("%d ",levelMaskPoly[i]);
        printf("\n");
    }
    for (int i = 0; i < 5; i++) levelMaskPoly[i] = levelMask[i];  // Copy data bits back in
    for (int i = 0; i < 15; i++) levelMaskPoly[i] = (levelMaskPoly[i] ^ levelMaskXOR[i]) & 1;

    
    for (int i = 0; i < polyLen; i++) {
        printf("%d ", byteStream[i]);
    }
    printf("\n");

    for (int i = 0; i < polyLen; i++) {
        for (int shift = 7; shift >= 0; shift--) printf("%d", (byteStream[i] >> shift) & 0x1);
        //printf("");
    }
    printf("\n");


    //unsigned char qrCode[QR_CODE_SIZE][QR_CODE_SIZE]; // = (unsigned char**) malloc(QR_CODE_SIZE * QR_CODE_SIZE);
    for (int i = 0; i < QR_CODE_SIZE; i++)
        for (int j = 0; j < QR_CODE_SIZE; j++)
            qrCode[i][j] = 0xff;

    // Add edge detectors
    int i = 0;
    int j = 0;

    // Top left and Top right align patterns
    for (i = 0; i < 7; i++) {
        switch (i) {
            case 0:
            case 6:
                for (j = 0; j < 7; j++) qrCode[i][j] = 0;
                for (j = QR_CODE_SIZE - 7; j < QR_CODE_SIZE; j++) qrCode[i][j] = 0;
                break;
            case 2:
            case 3:
            case 4:
                for (j = 2; j < 5; j++) qrCode[i][j] = 0;
                for (j = QR_CODE_SIZE - 5; j < QR_CODE_SIZE - 2; j++) qrCode[i][j] = 0;
                // Intentionally no break here - fill in the first and last col as well
            case 1:
            case 5:
                qrCode[i][0] = 0;
                qrCode[i][6] = 0;
                qrCode[i][QR_CODE_SIZE - 7] = 0;
                qrCode[i][QR_CODE_SIZE - 1] = 0;
                break;
        }
    }


    // Bottom left align pattern 
    for (i = QR_CODE_SIZE - 7; i < QR_CODE_SIZE; i++) {
        switch (i - (QR_CODE_SIZE - 7)) {
            case 0:
            case 6:
                for (j = 0; j < 7; j++) qrCode[i][j] = 0;
                break;
            case 2:
            case 3:
            case 4:
                for (j = 2; j < 5; j++) qrCode[i][j] = 0;
                // Intentionally no break here - fill in the first and last col as well
            case 1:
            case 5:
                qrCode[i][0] = 0;
                qrCode[i][6] = 0;
                break;
        }
    }

    
    
    // Inner align pattern 
    for (i = ALIGNMENT_PATTERN; i < ALIGNMENT_PATTERN + 5; i++) {
        switch (i - ALIGNMENT_PATTERN) {
            case 0:
            case 4:
                for (j = ALIGNMENT_PATTERN; j < ALIGNMENT_PATTERN + 5; j++) qrCode[i][j] = 0;
                break;
            case 2:
                qrCode[i][ALIGNMENT_PATTERN + 2] = 0;
            case 1:
            case 3:
                //for (j = ALIGNMENT_PATTERN + 2; j < ALIGNMENT_PATTERN + 4; j++) qrCode[i][j] = 0;
                qrCode[i][ALIGNMENT_PATTERN] = 0;
                qrCode[i][ALIGNMENT_PATTERN + 4] = 0;
                break;
        }
    }    
    
    // Top timing pattern
    i = 6;
    for (j = 8; j < QR_CODE_SIZE - 8; j++) {
        if (!(j & 0x1)) qrCode[i][j] = 0;
    }

    // Vertical timing pattern
    j = 6;
    for (i = 8; i < QR_CODE_SIZE - 8; i++) {
        if (!(i & 0x1)) qrCode[i][j] = 0;
    }

    // Black pixel
    qrCode[QR_CODE_SIZE - 8][8] = 0;


    // XXX - fixed pattern for Level L, Mask i%2 == 0
    // Top left
    qrCode[8][0] = levelMaskPoly[0] ? 0 : 0xff;
    qrCode[8][1] = levelMaskPoly[1] ? 0 : 0xff;

    qrCode[8][2] = levelMaskPoly[2] ? 0 : 0xff;
    qrCode[8][3] = levelMaskPoly[3] ? 0 : 0xff;
    qrCode[8][4] = levelMaskPoly[4] ? 0 : 0xff;

    qrCode[8][5] = levelMaskPoly[5] ? 0 : 0xff;

    qrCode[8][7] = levelMaskPoly[6] ? 0 : 0xff;
    qrCode[8][8] = levelMaskPoly[7] ? 0 : 0xff;
    qrCode[7][8] = levelMaskPoly[8] ? 0 : 0xff;

    qrCode[5][8] = levelMaskPoly[9] ? 0 : 0xff;
    qrCode[4][8] = levelMaskPoly[10] ? 0 : 0xff;
    qrCode[3][8] = levelMaskPoly[11] ? 0 : 0xff;
    qrCode[2][8] = levelMaskPoly[12] ? 0 : 0xff;
    qrCode[1][8] = levelMaskPoly[13] ? 0 : 0xff;
    qrCode[0][8] = levelMaskPoly[14] ? 0 : 0xff;

    // Bottom left
    qrCode[QR_CODE_SIZE - 1][8] = levelMaskPoly[0] ? 0 : 0xff;
    qrCode[QR_CODE_SIZE - 2][8] = levelMaskPoly[1] ? 0 : 0xff;

    qrCode[QR_CODE_SIZE - 3][8] = levelMaskPoly[2] ? 0 : 0xff;
    qrCode[QR_CODE_SIZE - 4][8] = levelMaskPoly[3] ? 0 : 0xff;
    qrCode[QR_CODE_SIZE - 5][8] = levelMaskPoly[4] ? 0 : 0xff;

    qrCode[QR_CODE_SIZE - 6][8] = levelMaskPoly[5] ? 0 : 0xff;
    qrCode[QR_CODE_SIZE - 7][8] = levelMaskPoly[6] ? 0 : 0xff;

    qrCode[8][QR_CODE_SIZE - 8] = levelMaskPoly[7] ? 0 : 0xff;
    qrCode[8][QR_CODE_SIZE - 7] = levelMaskPoly[8] ? 0 : 0xff;
    qrCode[8][QR_CODE_SIZE - 6] = levelMaskPoly[9] ? 0 : 0xff;
    qrCode[8][QR_CODE_SIZE - 5] = levelMaskPoly[10] ? 0 : 0xff;
    qrCode[8][QR_CODE_SIZE - 4] = levelMaskPoly[11] ? 0 : 0xff;
    qrCode[8][QR_CODE_SIZE - 3] = levelMaskPoly[12] ? 0 : 0xff;
    qrCode[8][QR_CODE_SIZE - 2] = levelMaskPoly[13] ? 0 : 0xff;
    qrCode[8][QR_CODE_SIZE - 1] = levelMaskPoly[14] ? 0 : 0xff;

    // Add bits
    // USING MASK (i+j)%2 == 0
    i = QR_CODE_SIZE - 1;
    j = QR_CODE_SIZE - 1;
    char pattern_toggle = 0;
    char direction = -1; // -1 is up, 1 is down
    int bit = 0;
    
    for (int byte = 0; byte < polyLen + REMAINDER_BYTE; byte++) {
        
        //printf("%d\n", byteStream[byte]);
        for (int shift = 7; shift >= 0; shift--) {
            if (bit >= (polyLen * 8) + REMAINDER_BITS) break;
            
            qrCode[i][j] = (((byteStream[byte] >> shift) ^ MASK2) & 0x1) ? 0x0 : 0xff;
            //qrCode[i][j] = (((byteStream[byte] >> shift)) & 0x1) ? 0x0 : 0xff;
            
            if ((bit > byteStreamLength*8) && (bit < QR_DATA_BITS) && (i >= 11) && (i < 27) && (j >= 11) && (j <27)) {
            //    qrCode[i][j] = 0xcc;
                printf("{%d,%d,%d},\n", i-11,j-11,bit);
            }

            //for (int ii = 0; ii < 16*16; ii++) {
            //    if (pixel_map_array[ii].bit == bit) qrCode[i][j] = 0xaa;
            //}

            bit++;
            
            if (pattern_toggle) {
                i += direction;
                j++;
            } else j--;
            pattern_toggle = !pattern_toggle;

            // Now handle all the special cases
            // Right hand side, avoid position marker
            if (j >= (QR_CODE_SIZE - 8)) {
                if ((i < 9) || (i > QR_CODE_SIZE - 1)) {
                    direction = -direction;
                    i += direction;
                    j -= 2;
                }
                // When we cross to the center domain, need to reset the row to full extremes
                if (j < (QR_CODE_SIZE - 8)) i = (direction == -1) ? (QR_CODE_SIZE - 1) : 0;
            }
            
                // Center section - top to bottom but avoid timing row
            else if (j >= 9) {
                if ((i < 0) || (i > QR_CODE_SIZE - 1)) {
                    direction = -direction;
                    i += direction;
                    j -= 2;
                }
                if (i == 6) i += direction; // Skip the timing row
              
                // When we cross to the left domain, need to reset the row to full extremes
                if (j < 9) i = (direction == -1) ? (QR_CODE_SIZE - 9) : 9;
            }

                // Left section, avoid position markers and vertical timing column
            else {
                if ((i < 9) || (i > QR_CODE_SIZE - 9)) {
                    direction = -direction;
                    i += direction;
                    j -= 2;
                }
                if (j == 6) j -= 1; // Skip the timing row
            }
            
           if ((j >= ALIGNMENT_PATTERN) && (j <= (ALIGNMENT_PATTERN + 4))) {
                if ((j == ALIGNMENT_PATTERN) && (i >= ALIGNMENT_PATTERN) && (i <= (ALIGNMENT_PATTERN + 4))) {
                    j -= 1;
                    pattern_toggle = 1;                    
                }
                else if (i == (ALIGNMENT_PATTERN + 4)) i -= 5;
                else if (i == ALIGNMENT_PATTERN) i += 5;
            }

        }
    }

    printf("Done - i = %d, j = %d\n", i, j);
    //qrCode[i][j] = 0x80;

    for (int i = 0; i < QR_CODE_SIZE; i++) {
        for (j = 0; j < QR_CODE_SIZE; j++) {
            printf("%d", !(qrCode[i][j]));
        }
        printf("\n");
    }

    gdImage* foo = gdImageCreate(QR_CODE_SIZE,QR_CODE_SIZE);
    int white = gdImageColorAllocate(foo,255,255,255);
    int black = gdImageColorAllocate(foo,0,0,0);
    for (int i = 0; i < QR_CODE_SIZE; i++) {
        for (j = 0; j < QR_CODE_SIZE; j++) {
            gdImageSetPixel(foo,j,i,qrCode[i][j] ? white : black);
        }
    }
    FILE *pngFile = fopen("/tmp/foo.png","w");
    gdImagePng(foo,pngFile);

    return &qrCode[0][0];
}

