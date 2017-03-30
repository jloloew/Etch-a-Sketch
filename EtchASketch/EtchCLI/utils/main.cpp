#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include "image.h"

using namespace std;

#define SHAPE_IMAGE_SIZE 512

void
usage()
{
    cout << "Usage: etch-convert -i /path/to/input/image.png [-o /path/to/output/image.etch]" << endl;
    cout << "Usage: etch-convert -p {circle} -o /path/to/output/image.etch" << endl;
    return;
}

int
main(int argc, char * const argv[])
{
    cout << "Welcome to etch-convert, the one and only .png ➡️  .etch file converter!" << endl;

    // Parse arguments.
    string inputFile, outputFile;
    string selectedPattern;
    int ch;
    // i = input file
    // o = output file
    // p = pattern
    while ((ch = getopt(argc, argv, "i:o:p:")) != -1) {
        switch (ch) {
            case 'i':
                if (selectedPattern.length() > 0) { // Can't do both image and pattern
                    usage();
                    exit(EXIT_FAILURE);
                }
                inputFile = string(optarg);
                break;
            case 'o':
                outputFile = string(optarg);
                break;
            case 'p':
                if (inputFile.length() > 0) { // Can't do both image and pattern
                    usage();
                    exit(EXIT_FAILURE);
                }
                selectedPattern = string(optarg);
                break;
            case '?':
            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }

    // Validate arguments.
    if (inputFile.length() > 0) {
        // We're using the inputFile.
        if (inputFile.length() <= 4) { // .png = 4 chars
            usage();
            exit(EXIT_FAILURE);
        }
        // Set outputFile if not yet set.
        if (outputFile.length() == 0) {
            outputFile = inputFile.substr(0, inputFile.find("."));
            outputFile.append(".etch");
        }
    } else if (selectedPattern.length() > 0) {
        // We're using a pattern.
        if (selectedPattern == "circle") {
            // We know how to draw circles, we're good.
        } else {
            // Unknown pattern.
            usage();
            exit(EXIT_FAILURE);
        }
    } else {
        // Neither inputFile nor pattern given.
        usage();
        exit(EXIT_FAILURE);
    }
    // Check the output file.
    if (outputFile.length() <= 5) { // .etch = 5 chars
        usage();
        exit(EXIT_FAILURE);
    }

    // Create the image.
    Image* inputImage;
    if (inputFile.length() > 0) {
        // Attempt to create image from input file name.
        inputImage = new Image();
        inputImage->readFromFile(inputFile);
    } else {
        // Use pattern
        inputImage = new Image(SHAPE_IMAGE_SIZE, SHAPE_IMAGE_SIZE);
        if (selectedPattern == "circle") {
            int radius = SHAPE_IMAGE_SIZE * 2 / 5;
            int radius2 = radius * radius;
            int center = SHAPE_IMAGE_SIZE / 2;
            // x^2 + y^2 = r^2
            for (int y = 0; y <= radius; ++y) {
                int x2 = radius2 - (y * y);
                int x = static_cast<int>(sqrtf(x2));
                // Shade the pixel at this (x,y) coord and its mirrors.
                RGBAPixel *pixels[4];
                pixels[0] = (*inputImage)(center + x, center + y);
                pixels[1] = (*inputImage)(center + x, center - y);
                pixels[2] = (*inputImage)(center - x, center + y);
                pixels[3] = (*inputImage)(center - x, center - y);
                for (int i = 0; i < 4; ++i) {
                    pixels[i]->red   = 0;
                    pixels[i]->green = 0;
                    pixels[i]->blue  = 0;
                }
            }
        }
    }

    // Attempt to etch image to file.
    inputImage->etchToFile(outputFile);

    cout << "Your etched file has been written to " << outputFile << " 🎉" << endl;

    return EXIT_SUCCESS;
}
