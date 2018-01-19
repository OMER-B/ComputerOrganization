// 313264053 OMER BARAK
#include "ex1.h"

/*
 * I assign a unsigned long i = 1, and check by the following:
 * Little Endian should look like this:   [ 01 00 00 00 00 00 00 00 ]
 * and Big Endian should look like this:  [ 00 00 00 00 00 00 00 01 ]
 * I check if the first cell (c[0]) contains anything. if not, then it is
 * compiled on a Big Endian machine, and if it does, then it's a Little Endian.
 */
int is_little_endian() {
  unsigned long i = 1;
  char *c = (char *) &i;
  if (c[0]) {
    return 1;
  }
  return 0;
}

/*
 * I shift the bytes of the first number 2 nibbles right then 2 nibbles
 * left to clear the LSB, and 'or' it with the 2 LSBs of the second number,
 * which I got by 'and'ing them with a two-bytes number to get the new merged
 * number. Since I work with bitwise operations, Endianness doesn't matter.
 */
unsigned long merge_bytes(unsigned long x, unsigned long int y) {
  unsigned long mask = 0xff;
  x >>= 8;    // Shift the first number right one byte.
  x <<= 8;    // Shift the first number left one byte.
  y &= mask;  // 'AND' the second number with the mask.
  return x | y;
}

/*
 * With the example input of: put_byte(0x12345678CDEF3456, 0xAB, 4)
 * I create a mask of 0xff and shift it to the i'th byte:
 * [ 00 00 00 00 ff 00 00 00 ]
 * Then I move the replacement to the i'th byte as well:
 * [ 00 00 00 00 ab 00 00 00 ]
 * After that I free up the i'th byte by 'and'ing it with the 'not' of the mask:
 * [ 56 34 ef cd 00 56 34 12 ]
 * Then I 'or' it (the variable with the free byte) with the input replacement:
 * [ 56 34 ef cd ab 56 34 12 ]
 * Endianness doesn't matter since I use bitwise operations.
 */
unsigned long put_byte(unsigned long x, unsigned char b, int i) {
  unsigned long mask = 0xff, orig = x, rplc = b;
  mask <<= (8 * i);
  rplc <<= 8 * i;
  orig &= ~mask;
  rplc |= orig;
  return rplc;
}