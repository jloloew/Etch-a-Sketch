//
//  main.cpp
//  EtchASketch
//
//  Created by Bliss Chapman on 2/7/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include "EtchASketch.hpp"

using std::cout;
using std::endl;
using std::string;

/// Print usage and exit.
__attribute__((noreturn))
static void
usage(void)
{
    cout << "Usage: etch -i /path/to/input/image.bmp -w 800 -h 600" << endl;
    exit(1);
}

static void
validateArgs(const string &inFile, long imgWidth, long imgHeight)
{
    if (inFile.size() <= 0) {
        goto fail;
    }
    if (imgWidth <= 0) {
        goto fail;
    }
    if (imgHeight <= 0) {
        goto fail;
    }
    // Everything checks out.
    return;
    
fail:
    usage();
}

int
main(int argc, char * const argv[])
{
    cout << "Welcome to etch ❤️" << endl;

    // Parse arguments.
    string inFile;
    long imgWidth = -1, imgHeight = -1;
    int ch;
    while ((ch = getopt(argc, argv, "i:w:h:")) != -1) {
        switch (ch) {
        case 'i':
            inFile = string(optarg);
            break;
        case 'w':
            imgWidth = strtol(optarg, nullptr, 0);
            break;
        case 'h':
            imgHeight = strtol(optarg, nullptr, 0);
            break;
        case '?':
        default:
            usage();
        }
    }
    validateArgs(inFile, imgWidth, imgHeight);
    // Make sure we can open the input file.
    FILE *inImageFd = fopen(inFile.c_str(), "r");
    if (!inImageFd) {
        perror("Can't open input image");
        exit(1);
    }
    
    // Read in the input image.
    etchasketch::Image::Pixel *rawInputImage = new etchasketch::Image::Pixel[imgWidth * imgHeight];
    if (!rawInputImage) {
        perror("Out of memory");
        exit(1);
    }
    if (fread(rawInputImage, sizeof(etchasketch::Image::Pixel), imgWidth * imgHeight, inImageFd) != imgWidth * imgHeight) {
        perror("Unable to read input image");
        exit(1);
    }
    // Copy the input image into an etchasketch::Image.
    etchasketch::Image inputImg = etchasketch::Image(imgWidth, imgHeight, rawInputImage);
    delete [] rawInputImage;
    rawInputImage = nullptr;
    
    // Create an ImageFlow.
    etchasketch::ImageFlow inputImgFlow = etchasketch::ImageFlow(inputImg);

    return 0;
}

