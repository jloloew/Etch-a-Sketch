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
		 * Pretty print a JSON string.
		 * @note: This does not cover the complete set of valid JSON. It only
		 * covers what's needed for this project.
		 */
		void prettyPrintJSON(std::ostream &out, std::istream &in);
		
	}
}


#endif /* EASUtils_Private_h */
