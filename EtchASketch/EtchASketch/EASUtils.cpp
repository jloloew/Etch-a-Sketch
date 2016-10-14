//
//  EASUtils.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "EASUtils.hpp"
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/time.h>

using std::string;

double
etchasketch::timeFunction(void (function)(void))
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
	timeTaken += static_cast<double>(endTime.tv_usec - startTime.tv_usec) /
			1.0e6; // Convert usec to sec.
	return timeTaken;
}

double
etchasketch::timeFunctionAndPrint(void (function)(void), string funcName)
{
	// Get elapsed time.
	double elapsedTime = etchasketch::timeFunction(function);
	std::stringstream resultStr;
	resultStr << "'" << funcName << "' took " << elapsedTime << "s to run.\n";
	EASLog(resultStr.str());
	return elapsedTime;
}

void
etchasketch::_EAS_Log(string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stdout, fmt.c_str(), args);
	va_end(args);
}
