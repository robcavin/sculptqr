//
//  encode.h
//  PicQR
//
//  Created by Rob on 7/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef PicQR_encode_h
#define PicQR_encode_h

#define QR_CODE_SIZE  37

unsigned char* encode(unsigned char qrCode[][QR_CODE_SIZE], char* message, bool bits[16][16]);


#endif
