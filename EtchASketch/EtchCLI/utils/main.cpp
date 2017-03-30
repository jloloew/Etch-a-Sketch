#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include "image.h"

using namespace std;

#define SHAPE_IMAGE_SIZE 512

enum pattern_e {
    pattern_unknown,
    pattern_circle
};

static enum pattern_e
patternFromString(string str)
{
    if (str == "circle") {
        return pattern_circle;
    } else {
        return pattern_unknown;
    }
}

__attribute__((noreturn))
static void
usageAndExit()
{
    cout << "Usage: etch-convert -i /path/to/input/image.png [-o /path/to/output/image.etch]" << endl;
    cout << "Usage: etch-convert -p {circle} -o /path/to/output/image.etch" << endl;
    exit(EXIT_FAILURE);
}

static void
drawCircle(Image &img)
{
    int radius = SHAPE_IMAGE_SIZE * 2 / 5;
    int radius2 = radius * radius;
    int center = SHAPE_IMAGE_SIZE / 2;
    // x^2 + y^2 = r^2
    for (int y = 0; y <= radius; ++y) {
        int x2 = radius2 - (y * y);
        int x = static_cast<int>(sqrtf(x2));
        // Shade the pixel at this (x,y) coord and its mirrors.
        RGBAPixel *pixels[4];
        pixels[0] = img(center + x, center + y);
        pixels[1] = img(center + x, center - y);
        pixels[2] = img(center - x, center + y);
        pixels[3] = img(center - x, center - y);
        for (int i = 0; i < 4; ++i) {
            pixels[i]->red   = 0;
            pixels[i]->green = 0;
            pixels[i]->blue  = 0;
        }
    }
}

static void
drawPattern(const enum pattern_e pattern, Image &img)
{
    switch (pattern) {
    case pattern_circle:
        drawCircle(img);
        break;
    case pattern_unknown:
    default:
        // Should be unreachable.
        cout << "Can't draw pattern: unknown pattern" << endl;
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char * const argv[])
{
    cout << "Welcome to etch-convert, the one and only .png ➡️  .etch file converter!" << endl;

    // Parse arguments.
    string inputFile, outputFile;
    enum pattern_e selectedPattern = pattern_unknown;
    int ch;
    // i = input file
    // o = output file
    // p = pattern
    while ((ch = getopt(argc, argv, "i:o:p:")) != -1) {
        switch (ch) {
            case 'i':
                if (selectedPattern != pattern_unknown) { // Can't do both image and pattern
                    usageAndExit();
                }
                inputFile = string(optarg);
                break;
            case 'o':
                outputFile = string(optarg);
                break;
            case 'p':
                if (inputFile.length() > 0) { // Can't do both image and pattern
                    usageAndExit();
                }
                selectedPattern = patternFromString(string(optarg));
                if (selectedPattern == pattern_unknown) {
                    usageAndExit();
                }
                break;
            case '?':
            default:
                usageAndExit();
        }
    }

    // Validate arguments.
    if (inputFile.length() > 0) {
        // We're using the inputFile.
        if (inputFile.length() <= 4) { // .png = 4 chars
            usageAndExit();
        }
        // Set outputFile if not yet set.
        if (outputFile.length() == 0) {
            outputFile = inputFile.substr(0, inputFile.find("."));
            outputFile.append(".etch");
        }
    } else {
        // We're using a pattern.
        if (selectedPattern == pattern_unknown) {
            // Unknown pattern or no pattern given.
            usageAndExit();
        }
    }
    // Check the output file.
    if (outputFile.length() <= 5) { // .etch = 5 chars
        usageAndExit();
    }

    // Create the image.
    Image inputImage;
    if (inputFile.length() > 0) {
        // Attempt to create image from input file name.
        inputImage = Image();
        inputImage.readFromFile(inputFile);
    } else {
        // Use pattern.
        inputImage = Image(SHAPE_IMAGE_SIZE, SHAPE_IMAGE_SIZE);
        drawPattern(selectedPattern, inputImage);
    }

    // Attempt to etch image to file.
    inputImage.etchToFile(outputFile);

    cout << "Your etched file has been written to " << outputFile << " 🎉" << endl;

    return EXIT_SUCCESS;
}
