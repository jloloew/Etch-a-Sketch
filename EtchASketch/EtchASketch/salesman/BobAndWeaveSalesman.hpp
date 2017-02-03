//
//  BobAndWeaveSalesman.hpp
//  EtchASketch
//
//  Created by Justin Loew on 2/3/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#ifndef BobAndWeaveSalesman_hpp
#define BobAndWeaveSalesman_hpp

#include "Salesman.hpp"
#include "Image.hpp"

namespace etchasketch {
	namespace salesman {
		
		class BobAndWeaveSalesman : public Salesman {
		public:
			BobAndWeaveSalesman(const Image &img);
			
			virtual ~BobAndWeaveSalesman() { }
			
			void orderPoints();
			
		private:
			const Image &img;
		};
		
	}
}

#endif /* BobAndWeaveSalesman_hpp */
