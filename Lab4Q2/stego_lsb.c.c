#include <stdio.h>
#include <stdlib.h>

#define WIDTH 512
#define HEIGHT 512

// Function to read a PGM image in text format.
// 'pixels' stores the pixel values of the image specified by 'filename'.
int readPGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    char buffer[256];
    fscanf(fp, "%s", buffer);  // Magic number
    fgets(buffer, sizeof(buffer), fp);  // Comment line
    fgets(buffer, sizeof(buffer), fp);  // Width Height
    fgets(buffer, sizeof(buffer), fp);  // Max gray value

    for (int i = 0; i < width * height; i++) {
        if (fscanf(fp, "%hhu", &pixels[i]) != 1) {
            fclose(fp);
            perror("Error reading pixel values");
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

// Function to write a PGM image in text format.
// The dereferenced values of 'pixels' are stored in the PGM image specified by 'filename'.
int writePGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    fprintf(fp, "P2\n");
    fprintf(fp, "# Created by Steganography LSB\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");

    for (int i = 0; i < width * height; i++) {
        fprintf(fp, "%d ", pixels[i]);
        if ((i + 1) % 15 == 0) fprintf(fp, "\n");
    }

    fclose(fp);
    return 0;
}

// Function to write a PGM image in binary format.
// The dereferenced values of 'pixels' are stored in the PGM image specified by 'filename' in binary format.
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    fprintf(fp, "P5\n");
    fprintf(fp, "# Created by Steganography LSB\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");

    if (fwrite(pixels, sizeof(unsigned char), width * height, fp) != width * height) {
        fclose(fp);
        perror("Error writing pixels");
        return -1;
    }

    fclose(fp);
    return 0;
}

// Function to hide a secret image using the 4-bit LSB steganography algorithm.
// The dereferenced values of 'coverPixels' (cover image) are updated with the dereferenced values of 
// 'secretPixels' (secret image) based on the 4-bit LSB steganography algorithm
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        coverPixels[i] &= 0xF0;  // Clear 4 least significant bits of cover image
        unsigned char secretNibble = (secretPixels[i] & 0xF0) >> 4;
        coverPixels[i] |= secretNibble;  // Combine cover and secret pixels
    }
}

// Function to extract the secret image using 4-LSB steganography algorithm.
// The extracted pixel values, derived from the stego image referenced by 'coverPixels',
// are stored in the memory space pointed to by 'outputPixels' 
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        unsigned char secretNibble = (coverPixels[i] & 0x0F) << 4;
        outputPixels[i] = secretNibble;  // Store extracted secret pixel with zero least significant bits
    }
}

int main() {
    char cover_image[]="baboon.pgm";
    char secret_image[]="farm.pgm";
    char stego_image[]="stego_image_bin.pgm";
    char extracted_secret[]="extracted_secret.pgm";
    
    unsigned char *coverPixels, *secretPixels, *outputPixels;
    int coverWidth=WIDTH, coverHeight=HEIGHT, secretWidth=WIDTH, secretHeight=HEIGHT;

    // Using malloc, dynnamically request extact memory space,referenced by coverPixels, to store the cover image pixel values. 
    coverPixels = malloc(coverWidth * coverHeight * sizeof(unsigned char));
    if (!coverPixels) {
        perror("Memory allocation failed for cover pixels");
        return -1;
    }
    
    // Read the cover image file (text PGM) and store its pixels in 'coverPixels' referenced memory space.
    if (readPGMText(cover_image, coverPixels, coverWidth, coverHeight) != 0) {
        free(coverPixels);
        return -1;
    }

    //  Using malloc, dynamically request extact memory space, referenced by 'secretPixels', to store the secret image pixel values.
    secretPixels = malloc(secretWidth * secretHeight * sizeof(unsigned char));
    if (!secretPixels) {
        free(coverPixels);
        perror("Memory allocation failed for secret pixels");
        return -1;
    }
    
    // Read the secret image file (text PGM) and store its pixels in 'secretPixels' referenced memory space.
    if (readPGMText(secret_image, secretPixels, secretWidth, secretHeight) != 0) {
        free(coverPixels);
        free(secretPixels);
        return -1;
    }

    // Check if the dimensions of both images match, exit if they don't.
    if (coverWidth != secretWidth || coverHeight != secretHeight) {
        fprintf(stderr, "Image dimensions do not match\n");
        free(coverPixels);
        free(secretPixels);
        return -1;
    }
 
    // Call the function embedLSB to embed the secret image into the cover image.
    embedLSB(coverPixels, secretPixels, coverWidth, coverHeight);
    
    // The stego image pixels are stored in 'coverPixels' referenced memory space.
    // Save the stego image into binary format to stego_image_bin.pgm
    if (writePGMBinary(stego_image, coverPixels, coverWidth, coverHeight) != 0) {
        free(coverPixels);
        free(secretPixels);
        return -1;
    }

    //  Using malloc, dynamically request exact memory space, referenced by 'outputPixels', to store the extracted secret image    
    outputPixels = malloc(coverWidth * coverHeight * sizeof(unsigned char));
    if (!outputPixels) {
        free(coverPixels);
        free(secretPixels);
        perror("Memory allocation failed for output pixels");
        return -1;
    }

    // Call the function extractLSB to extract the secret image from the stego image
    extractLSB(coverPixels, outputPixels, coverWidth, coverHeight);
    
    // Save the extracted secret image in a text format to extracted_secret.pgm
    if (writePGMText(extracted_secret, outputPixels, coverWidth, coverHeight) != 0) {
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Free allocated memory
    free(coverPixels);
    free(secretPixels);
    free(outputPixels);
   
    return 0;
}