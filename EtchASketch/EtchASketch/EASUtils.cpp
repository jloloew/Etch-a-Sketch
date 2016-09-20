//
//  EASUtils.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "EASUtils.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <sys/time.h>

using std::string;


double EtchASketch::timeFunction(void (function)(void))
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
	double timeTaken = double(endTime.tv_sec - startTime.tv_sec);
	timeTaken += double(endTime.tv_usec - startTime.tv_usec) / 1000.0;
	return timeTaken;
}

void EtchASketch::timeFunctionAndPrint(void (function)(void), string funcName)
{
	// Get elapsed time.
	double elapsedTime = timeFunction(function);
	std::stringstream resultStr;
	resultStr << "'" << funcName << "' took " << elapsedTime << "s to run.\n";
	EASLog(resultStr.str());
}
