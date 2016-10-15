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

#define USEC_PER_SEC 1.0e6

using std::endl;
using std::string;
using std::stringstream;

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
		char c = in.get();
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
					c = in.get();
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
