//
//  EASUtils.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef EASUtils_hpp
#define EASUtils_hpp

#include <string>
#include <vector>
#include "Image.hpp"

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
		* Utility function to write a vector of ordered edge points to a png.
		*/
    /*
    bool
		writeOrderedEdgePointsToFile(
		  std::string const & file_name,
		  std::vector<etchasketch::KDPoint<2>> orderedEdgePoints,
		  long imgWidth,
		  long imgHeight);
    */
	}
}

#endif /* EASUtils_hpp */
