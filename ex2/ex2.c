// OMER BARAK 313264053
#include <stdio.h>
#include <string.h>

#define SIZE_OF_CHAR sizeof(char) // For mission 2.
#define NUM_OF_ELEMENTS 2         // For mission 2. Two bytes in size of char.

void copyFile(FILE *, FILE *);
void changeCoding(FILE *, FILE *, char *, char *, char *);
void swap(char *);

/*
 * I check for files and arguments validity here and then send to the current
 * mission with a switch-case.
 */
int main(int argc, char *argv[]) {
  /* Check for files validity. */
  if ((argv[1] == NULL) || (argv[2] == NULL) || !(fopen(argv[1], "rb"))) {
    return 0;
  }

  /* If cannot open source file, close file and exit program. */
  FILE *source = fopen(argv[1], "rb");
  if (source == NULL) {
    fclose(source);
    return 0;
  }

  /* If cannot new copy file, close file and exit program. */
  FILE *copy = fopen(argv[2], "wb");
  if (copy == NULL || (argv[3] && !argv[4])) {
    fclose(copy);
    fclose(source);
    return 0;
  }

  /* Check for arguments validity */
  if (argv[3] && argv[4]) {
    if ((strcmp(argv[3], "-unix") != 0 && strcmp(argv[3], "-win") != 0
        && strcmp(argv[3], "-mac") != 0) || (strcmp(argv[4], "-unix") != 0
        && strcmp(argv[4], "-win") != 0 && strcmp(argv[4], "-mac") != 0)) {
      fclose(copy);
      fclose(source);
      return 0;
    }
  }

  /* Choose task based on number of arguments given. */
  switch (argc) {
    case 3:copyFile(source, copy);
      break;

    case 5:changeCoding(source, copy, argv[3], argv[4], "-keep");
      /* Same function to prevent code duplication. */
      break;

    case 6:changeCoding(source, copy, argv[3], argv[4], argv[5]);
      break;

    default:break;
  }

  /* Close both files after finishing with everything. */
  fclose(copy);
  fclose(source);
  return 0;
}

/*
 * MISSION 1
 * Read a buffer of 2 characters every loop and put it in copy. It's simple.
 */
void copyFile(FILE *src, FILE *dst) {
  char buffer[2];
  size_t c;

  /* Read first two bytes */
  c = fread(buffer, SIZE_OF_CHAR, NUM_OF_ELEMENTS, src);
  while (c) {
    /* Write the bytes */
    fwrite(buffer, SIZE_OF_CHAR, c, dst);

    /* Read next two bytes */
    c = fread(buffer, SIZE_OF_CHAR, NUM_OF_ELEMENTS, src);
  }
}

/*
 * MISSION 3
 * Given two files, two OS names and a flag, this function copies the content
 * of the first file to the second file, with end-line encoding of the second
 * OS that is given. If a third flag is given as -swap, also changed from Big
 * Endian to Little Endian or LE to BE.
 */
void changeCoding(FILE *src, FILE *dst, char *os1, char *os2, char *flag) {
  char buffer[2];
  size_t c;
  int endian;
  /* Read first char and check for Endianness of the file. */
  c = fread(buffer, SIZE_OF_CHAR, NUM_OF_ELEMENTS, src);

  /* Start while loop until end of file. */
  while (c == NUM_OF_ELEMENTS) {

    /* If the read word is \n or \r. */
    if ((buffer[0] == '\n' && buffer[1] == '\0')
        || (buffer[0] == '\r' && buffer[1] == '\0')
        || (buffer[0] == '\0' && buffer[1] == '\r')
        || (buffer[0] == '\0' && buffer[1] == '\n')) {
      if ((buffer[0] == '\n' && buffer[1] == '\0')
          || (buffer[0] == '\r' && buffer[1] == '\0')) {
        endian = 0;
      } else {
        endian = 1;
      }
      /* If the source OS is Windows, read another char. */
      if (strcmp(os1, "-win") == 0) {
        c = fread(buffer, SIZE_OF_CHAR, NUM_OF_ELEMENTS, src);
      }

      /* If the destination OS is Linux, set the first or second byte as \n, */
      /* depending on endiannes                                              */
      if (strcmp(os2, "-unix") == 0) {
        buffer[endian] = '\n';
        buffer[1 - endian] = '\0';

        /* If the destination OS is Mac, set the read char as \r. */
      } else if (strcmp(os2, "-mac") == 0) {
        buffer[endian] = '\r';
        buffer[1 - endian] = '\0';

        /* If the destination OS is Windows, set the read char as \r, swap   */
        /* Endianness if flagged, write it and then set the read char as \n. */
      } else if (strcmp(os2, "-win") == 0) {
        buffer[endian] = '\r';
        buffer[1 - endian] = '\0';

        /* Swap the chars */
        if (strcmp(flag, "-swap") == 0) {
          swap(buffer);
        }
        fwrite(buffer, SIZE_OF_CHAR, c, dst);

        /* Swap the chars again [00][0d][0a][00] -> [00][00d][00][00a] */
        if (strcmp(flag, "-swap") == 0) {
          swap(buffer);
        }

        /* Write the \n */
        buffer[endian] = '\n';
        buffer[1 - endian] = '\0';
      }
    }

    /* If Endianness swap is flagged, swap bytes. */
    if (strcmp(flag, "-swap") == 0) {
      swap(buffer);
    }

    /* Write the char */
    fwrite(buffer, SIZE_OF_CHAR, c, dst);

    /* Read next char */
    c = fread(buffer, SIZE_OF_CHAR, NUM_OF_ELEMENTS, src);
  }
}

/*
 * Swaps the first element of the array with the second element of the array.
 */
void swap(char *s) {
  char temp;
  temp = s[0];
  s[0] = s[1];
  s[1] = temp;
}