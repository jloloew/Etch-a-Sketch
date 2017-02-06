//
//  EASUtils.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef EASUtils_hpp
#define EASUtils_hpp

#include <iostream>
#include <string>

#define _EASLog2(fmt, ...) do { \
etchasketch::utils::_EAS_Log("%s:%s:%d: ", __FILE__, __PRETTY_FUNCTION__, \
__LINE__); \
etchasketch::utils::_EAS_Log((fmt) , ## __VA_ARGS__); \
etchasketch::utils::_EAS_Log("\n"); \
} while (0)

#define EASLog(fmt, ...) _EASLog2((fmt) , ## __VA_ARGS__)

namespace etchasketch {
	namespace utils {
		
		/**
		 * Times the execution time of a given function.
         * @return The execution time in seconds or -1 on failure.
		 */
		double timeFunction(void (function)(void))
		__attribute__((warn_unused_result));
		
		/**
		 * Same as `timeFunction`, but also prints how long the function took.
		 */
		double timeFunctionAndPrint(void (function)(void),
									std::string funcName);
		
		void _EAS_Log(std::string fmt, ...);
		
	}
}

#endif /* EASUtils_hpp */
