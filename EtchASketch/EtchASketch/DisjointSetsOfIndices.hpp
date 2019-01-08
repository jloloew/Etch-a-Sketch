//
//  DisjointSetsOfIndices.hpp
//  EtchASketch
//
//  Created by Justin Loew on 1/8/19.
//  Copyright © 2019 Justin Loew. All rights reserved.
//

#ifndef DisjointSetsOfIndices_hpp
#define DisjointSetsOfIndices_hpp

#include <vector>

namespace etchasketch {
  
  class DisjointSetsOfIndices {
  public:
    
    /**
     * Create new disjoint sets, each of which is its own root.
     *
     * @param numSets The number of nodes to create. Defaults to 0.
     */
    DisjointSetsOfIndices(const size_t numSets = 0);
    
    /**
     * Creates @c num unconnected root nodes at the end of the vector.
     *
     * @param num The number of nodes to create.
     */
    void addElements(size_t num);
    
    /**
     * Find the root of a set.
     *
     * @param elem The index of the element whose root we seek.
     * @returns the index of the root of the up-tree in which the parameter
     * element resides.
     */
    int find(const int elem);
    
    /**
     * Union two subsets.
     *
     * @param a The index of the first element to union.
     * @param b The index of the second element to union.
     */
    void unionSets(const int a, const int b);
    
  private:
    std::vector<int> sets;
    
    /**
     * Get the size of an individual disjoint set.
     *
     * @param elem The index of an element in the subset.
     * @returns the size of the subset.
     */
    size_t subsetSize(const int elem);
  };
  
}

#endif /* DisjointSetsOfIndices_hpp */
