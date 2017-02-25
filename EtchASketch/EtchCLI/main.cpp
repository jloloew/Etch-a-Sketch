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
#include <png.h>
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

// Utility function to write a vector of ordered edge points to a png.
bool writeOrderedEdgePointsToFile(string const & file_name, std::vector<etchasketch::KDPoint<2>> orderedEdgePoints)
{
    FILE * fp = fopen(file_name.c_str(), "wb");
    if (!fp)
    {
        epng_err("Failed to open file " + file_name);
        return false;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        epng_err("Failed to create png struct");
        fclose(fp);
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        epng_err("Failed to create png info struct");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        epng_err("Error initializing libpng io");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);

    // write header
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        epng_err("Error writing image header");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }
    png_set_IHDR(png_ptr, info_ptr, _width, _height, 
            8,
            PNG_COLOR_TYPE_RGB_ALPHA, 
            PNG_INTERLACE_NONE, 
            PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // write image
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        epng_err("Failed to write image");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    int bpr = png_get_rowbytes(png_ptr, info_ptr);
    png_byte * row = new png_byte[bpr];
    for (size_t y = 0; y < _height; y++)
    {
        for (size_t x = 0; x < _width; x++)
        {
            png_byte * pix = &(row[x*4]);
            pix[0] = 0;
            pix[1] = 0;
            pix[2] = 0;
            pix[3] = 0;
        }
        png_write_row(png_ptr, row);
    }
    delete [] row;
    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
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
    inputImgFlow.convertToGrayscale();
    inputImgFlow.detectEdges();
    inputImgFlow.generateEdgePoints();
    inputImgFlow.orderEdgePointsForDrawing();
    std::vector<etchasketch::KDPoint<2>> points = inputImgFlow.getOrderedEdgePoints(); 
    writeOrderedEdgePointsToFile("lena_ordered_edge_points.png", points);
    
    return 0;
}

