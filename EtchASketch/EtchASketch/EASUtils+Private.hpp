//
//  EASUtils+Private.hpp
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef EASUtils_Private_h
#define EASUtils_Private_h

#include "EASUtils.hpp"

namespace etchasketch {
	namespace utils {
		
		/**
		 * Times the execution of a given function.
		 * @return The execution time in seconds, or -1 on failure.
		 */
		double timeFunction(void (function)(void))
		__attribute__((warn_unused_result));

		/**
		 * Same as `timeFunction`, but also prints how long the function took.
		 */
		double timeFunctionAndPrint(void (function)(void),
									std::string funcName);

		/// The implementation of the @c EASLog macro.
		void _EAS_Log(std::string fmt, ...);

		/**
		 * Pretty print a JSON string.
		 * @note: This does not cover the complete set of valid JSON. It only
		 * covers what's needed for this project.
		 */
		void prettyPrintJSON(std::ostream &out, std::istream &in);

	}
}


#endif /* EASUtils_Private_h */
