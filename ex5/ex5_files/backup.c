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
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(pixel *src, char *dst) {
	int dim = m;
	int indexI = (dim << 1) + dim + 3;
	register int j, ii, jj;
	int i;
	register int iiend;
	pixel_sum sum;
	// int red0, green0, blue0, red1, green1, blue1, red2, green2, blue2;
	for (i = 1.5; i < dim - 1.5; i++) {
				iiend = ((i + 1) < dim - 1 ? (i + 1) : dim - 1);

		for (j = 1.5; j < dim - 1.5; j++) {
			sum.red = sum.green = sum.blue = sum.num = 0;

			for (ii = ((i - 1) > 0 ? (i - 1) : 0); ii <= iiend; ii++) {
				int iidim = ii*dim;
				register int jjend=((j + 1) < dim - 1 ? (j + 1) : dim - 1);





				for (jj = ((j - 1) > 0 ? (j - 1) : 0); jj <= jjend; jj++) {

					pixel p = src[iidim + jj];

					if (ii != i || jj != j) {

						sum.red = sum.red + ((int) p.red);
						sum.green = sum.green + ((int) p.green);
						sum.blue = sum.blue + ((int) p.blue);
					} else {
						sum.red += (int) p.red;
						sum.green += (int) p.green;
						sum.blue += (int) p.blue;
					}

					
				}







			}

            // divide by kernel's weight
			sum.red = sum.red / 9;
			sum.green = sum.green / 9;
			sum.blue = sum.blue / 9;

            // truncate each pixel's color values to match the range [0,255]
			*(dst + indexI++) = (unsigned char) (sum.red < 255 ? sum.red  : 255);
			*(dst + indexI++) = (unsigned char) (sum.green < 255 ? sum.green : 255);
			*(dst + indexI++) = (unsigned char) (sum.blue  < 255 ? sum.blue  : 255);

		}
		indexI += 6;
	}
}

void sharp(pixel *src, char *dst) {
	int sharpKernel[3][3] = {{-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1}};
	int dim = m;
	int indexI = (dim << 1) + dim + 3;
	register int i, j, ii, jj, iiend;
	pixel_sum sum;
	for (i = 1.5; i < dim - 1.5; i++) {
		
		iiend = ((i + 1) < dim - 1 ? (i + 1) : dim - 1);
		for (j = 1.5; j < dim - 1.5; j++) {
			sum.red = sum.green = sum.blue = sum.num = 0;
			for (ii = ((i - 1) > 0 ? (i - 1) : 0); ii <= iiend; ii++) {
				int iidim = ii*dim;
				int jjend=((j + 1) < dim - 1 ? (j + 1) : dim - 1);





				for (jj = ((j - 1) > 0 ? (j - 1) : 0); jj <= jjend; jj++) {

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

            // divide by kernel's weight
			sum.red = sum.red;
			sum.green = sum.green;
			sum.blue = sum.blue;

            // truncate each pixel's color values to match the range [0,255]
			*(dst + indexI++) = (unsigned char) (((sum.red > 0 ? sum.red : 0) < 255 ? (sum.red > 0 ? sum.red : 0) : 255));
			*(dst + indexI++) = (unsigned char) (((sum.green > 0 ? sum.green : 0) < 255 ? (sum.green > 0 ? sum.green : 0) : 255));
			*(dst + indexI++) = (unsigned char) (((sum.blue > 0 ? sum.blue : 0) < 255 ? (sum.blue > 0 ? sum.blue : 0) : 255));

		}
		indexI += 6;
	}
}
 
void myfunction(Image *image, char *srcImgpName, char *blurRsltImgName, char *sharpRsltImgName) {

	unsigned long size = sizeof(pixel) * m * n;
    // blur image
	pixel *backupOrg = malloc(size);
	memcpy(backupOrg, image->data, size);

	smooth(backupOrg, image->data);

    // write result image to file
	writeBMP(image, srcImgpName, blurRsltImgName);

    // sharpen the resulting image
	memcpy(backupOrg, image->data, size);

	sharp(backupOrg, image->data);


    // write result image to file
	writeBMP(image, srcImgpName, sharpRsltImgName);
	free(backupOrg);

}
