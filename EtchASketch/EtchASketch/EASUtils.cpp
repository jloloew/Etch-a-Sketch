//
//  EASUtils.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "EASUtils.hpp"
#include "EASUtils+Private.hpp"
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/time.h>
#include <png.h>

#define USEC_PER_SEC 1.0e6

using std::endl;
using std::string;
using std::stringstream;

bool
writeOrderedEdgePointsToFile(
  string const & file_name,
  std::vector<etchasketch::KDPoint<2>> orderedEdgePoints,
  long imgWidth,
  long imgHeight)
{
    FILE * fp = fopen(file_name.c_str(), "wb");
    if (!fp)
    {
        return false;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fclose(fp);
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);

    // write header
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }
    png_set_IHDR(png_ptr, info_ptr, imgWidth, imgHeight,
            8,
            PNG_COLOR_TYPE_RGB_ALPHA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // write image
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    int bpr = png_get_rowbytes(png_ptr, info_ptr);
    png_byte * row = new png_byte[bpr];
    for (size_t y = 0; y < imgHeight; y++)
    {
        for (size_t x = 0; x < imgWidth; x++)
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

double
etchasketch::utils::timeFunction(void (function)(void))
{
	// Get the start time.
	struct timeval startTime, endTime;
	if (gettimeofday(&startTime, NULL) != 0) {
		// error
		return -1.0;
	}
	// Run the function.
	function();
	// Get the end time and compute the time difference.
	if (gettimeofday(&endTime, NULL) != 0) {
		// error
		return -1.0;
	}
	double timeTaken = static_cast<double>(endTime.tv_sec - startTime.tv_sec);
	double usecTaken = static_cast<double>(endTime.tv_usec - startTime.tv_usec);
	timeTaken += usecTaken / USEC_PER_SEC; // Convert usec to sec.
	return timeTaken;
}

double
etchasketch::utils::timeFunctionAndPrint(void (function)(void), string funcName)
{
	// Get elapsed time.
	double elapsedTime = timeFunction(function);
	stringstream resultStr;
	resultStr << "'" << funcName << "' took " << elapsedTime << "s to run.\n";
	EASLog(resultStr.str());
	return elapsedTime;
}

void
etchasketch::utils::_EAS_Log(string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stdout, fmt.c_str(), args);
	va_end(args);
}

#pragma mark - Private

void
etchasketch::utils::prettyPrintJSON(std::ostream &out, std::istream &in)
{
	// Set up new line function to add correct indentation.
	auto newLine = [&in, &out](size_t nestingLevel) mutable {
		out << endl;
		// Add indentation if necessary.
		if (!in.eof()) {
			for (size_t i = 0; i < nestingLevel; i++) {
				out << '\t';
			}
			// Remove leading spaces from the new line.
			while (in.peek() == ' ') {
				in.get();
			}
		}
	};

	size_t nestingLevel = 0;
	bool isWithinSquareBrackets = false;
	while (!in.eof()) {
		// Read in next char.
		char c = static_cast<char>(in.get());
		switch (c) {
			case '{':
				out << c;
				newLine(++nestingLevel);
				break;
			case '}':
				newLine(--nestingLevel);
				out << c;
				break;
			case '[':
				out << c;
				isWithinSquareBrackets = true;
				break;
			case ']':
				out << c;
				isWithinSquareBrackets = false;
				break;
			case ',':
				out << c;
				// Add a newline if we're not within an array.
				if (!isWithinSquareBrackets) {
					newLine(nestingLevel);
				}
				break;
			case '"':
				out << c;
				// Finish off the string.
				do {
					c = static_cast<char>(in.get());
					out << c;
				} while (c != '"');
				break;
			case ' ':
				// Turn multiple spaces into just one.
				out << c;
				while (in.peek() == ' ') {
					in.get();
				}
				break;
			case '\n':
				// Silently discard newlines.
				break;
			default:
				// Just print all other characters.
				out << c;
				break;
		}
	}
}
