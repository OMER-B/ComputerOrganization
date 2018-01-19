typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} pixel;

typedef struct {
  int red;
  int green;
  int blue;
  int num;
} pixel_sum;

/*
 * The kernel is [1 1 1]
 *				 [1 1 1]
 *				 [1 1 1]
 * So we dont need to multiply, only sum the pixels. For this I summed the first two colums
 * and saved them, and in the inner loop calculate the third. This way for the next pixels we
 * save calculations for the next pixels since columns 2 and 3 are already calculated, and only
 * need to calculate the third column.
 */
void blur(pixel *src, char *dst) {
  int dim = m;
  int indexI = ((dim) << 1) + dim + 3;
  int i, j;
  int red0, green0, blue0, red1, green1, blue1;
  /* Variables that are used often (inner loop) are saved in registers */
  register int red2, green2, blue2;
  register int first = 0, second = dim - 1, third = (second) << 1;
  register pixel pixie;

  for (i = 1; i < dim - 1; ++i) {

    pixie = *(src + first);
    red0 = (int) pixie.red;
    green0 = (int) pixie.green;
    blue0 = (int) pixie.blue;

    ++first;
    pixie = *(src + first);

    red1 = (int) pixie.red;
    green1 = (int) pixie.green;
    blue1 = (int) pixie.blue;

    pixie = *(src + second);

    red0 += (int) pixie.red;
    green0 += (int) pixie.green;
    blue0 += (int) pixie.blue;

    ++second;
    pixie = *(src + second);

    red1 += (int) pixie.red;
    green1 += (int) pixie.green;
    blue1 += (int) pixie.blue;

    pixie = *(src + third);

    red0 += (int) pixie.red;
    green0 += (int) pixie.green;
    blue0 += (int) pixie.blue;

    ++third;
    pixie = *(src + third);

    red1 += (int) pixie.red;
    green1 += (int) pixie.green;
    blue1 += (int) pixie.blue;

    ++second;
    third += 2;

    for (j = 1; j < dim - 1; ++j) {
      ++first;
      ++second;
      ++third;
      pixie = *(src + first);

      red2 = (int) pixie.red;
      green2 = (int) pixie.green;
      blue2 = (int) pixie.blue;

      pixie = *(src + second);

      red2 += (int) pixie.red;
      green2 += (int) pixie.green;
      blue2 += (int) pixie.blue;

      pixie = *(src + third);

      red2 += (int) pixie.red;
      green2 += (int) pixie.green;
      blue2 += (int) pixie.blue;

      /* Sum and divide by the kernel scale */
      *(dst + indexI++) = (unsigned char) ((red0 + red1 + red2) / 9);
      *(dst + indexI++) = (unsigned char) ((green0 + green1 + green2) / 9);
      *(dst + indexI++) = (unsigned char) ((blue0 + blue1 + blue2) / 9);

      /* Prepare for the next iteration */
      red0 = red1;
      green0 = green1;
      blue0 = blue1;

      red1 = red2;
      green1 = green2;
      blue1 = blue2;

      red2 = 0;
      green2 = 0;
      blue2 = 0;

    }
    ++first;
    --third;
    indexI += 6;
  }
}

/*
 * The kernel is [-1 -1 -1]
 *				 [-1  9 -1]
 *				 [-1 -1 -1]
 * This function is harder because the kernel has weights.
 * Every change here is standard, I didn't unroll the loops like in smooth because it was hard.
 * I removed most of the if conditions and left one, and the higher probability if is first.
 * I changed the pixels with a pointer instead of recalculating the next position
 * in every iteration, so it's quicker access to memory. 
 */
void sharp(pixel *src, char *dst) {
  int sharpKernel[3][3] = {{-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1}};
  int dim = m;
  int indexI = (dim << 1) + dim + 3;
  int i, j, iiend;

  register int ii, jj;
  register pixel_sum sum;
  for (i = 1; i < dim - 1; ++i) {
    iiend = ((i + 1) < dim - 1 ? (i + 1) : dim - 1);
    for (j = 1; j < dim - 1; ++j) {
      sum.red = sum.green = sum.blue = sum.num = 0;
      for (ii = ((i - 1) > 0 ? (i - 1) : 0); ii <= iiend; ++ii) {
        int iidim = ii * dim;
        int jjend = ((j + 1) < dim - 1 ? (j + 1) : dim - 1);

        for (jj = ((j - 1) > 0 ? (j - 1) : 0); jj <= jjend; ++jj) {

          pixel p = src[iidim + jj];

          if (ii != i || jj != j) {
            sum.red = sum.red + ((int) p.red) * (-1);
            sum.green = sum.green + ((int) p.green) * (-1);
            sum.blue = sum.blue + ((int) p.blue) * (-1);
          } else {
            sum.red += ((int) p.red) * 9;
            sum.green += ((int) p.green) * 9;
            sum.blue += ((int) p.blue) * 9;
          }
        }
      }

      // truncate each pixel's color values to match the range [0,255]
      *(dst + indexI++) = (unsigned char) (((sum.red > 0 ? sum.red : 0) < 255 ? (sum.red > 0 ? sum.red : 0) : 255));
      *(dst + indexI++) = (unsigned char) (((sum.green > 0 ? sum.green : 0) < 255 ? (sum.green > 0 ? sum.green : 0) : 255));
      *(dst + indexI++) = (unsigned char) (((sum.blue > 0 ? sum.blue : 0) < 255 ? (sum.blue > 0 ? sum.blue : 0) : 255));
    }
    indexI += 6;
  }
}

/*
 * Removed most of the functions and only left two of them. Write directly to memory and only allocate once is better
 * than the copy functions from before.
 * In general I removed all functions calls and used immediate values instead of variables, moved iterations by addresses
 * in the pixels struct and removed many calculations by remembering previous calculations (in smooth).
 */
void myfunction(Image *image, char *srcImgpName, char *blurRsltImgName, char *sharpRsltImgName) {
  unsigned long size = sizeof(pixel) * m * m;
  pixel *backupOrg = malloc(size);
  memcpy(backupOrg, image->data, size);

  // blur image
  blur(backupOrg, image->data);

  // write result image to file
  writeBMP(image, srcImgpName, blurRsltImgName);

  memcpy(backupOrg, image->data, size);

  // sharpen the resulting image
  sharp(backupOrg, image->data);


  // write result image to file
  writeBMP(image, srcImgpName, sharpRsltImgName);
  free(backupOrg);
}
