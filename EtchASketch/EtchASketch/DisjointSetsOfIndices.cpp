//
//  DisjointSetsOfIndices.cpp
//  EtchASketch
//
//  Created by Justin Loew on 1/8/19.
//  Copyright © 2019 Justin Loew. All rights reserved.
//

#include "DisjointSetsOfIndices.hpp"

etchasketch::DisjointSetsOfIndices::DisjointSetsOfIndices(const size_t numSets)
: sets(std::vector<int>())
{
  addElements(numSets);
}

void
etchasketch::DisjointSetsOfIndices::addElements(size_t num)
{
  // Throw the extra elements onto the end.
  sets.insert(sets.end(), num, -1);
}

int
etchasketch::DisjointSetsOfIndices::find(const int elem)
{
  if (elem < 0 || static_cast<size_t>(elem) >= sets.size()) {
    // This is an error.
    // TODO: Handle this properly by throwing an exception or something.
    return elem;
  }
  
  if (sets[elem] < 0 || sets[elem] == elem) {
    // Found the root
    return elem;
  }
  
  // Keep recursing until we find the root
  const int root = find(sets[elem]);
  if (sets[elem] != root) {
    sets[elem] = root;
  }
  return root;
}

void
etchasketch::DisjointSetsOfIndices::unionSets(const int a, const int b)
{
  // Safety first
  if (a < 0 || b < 0 || a == b) {
    return;
  }
  if (static_cast<size_t>(a) >= sets.size()
      || static_cast<size_t>(b) >= sets.size()) {
    return;
  }
  const int rootA = find(a);
  const int rootB = find(b);
  if (rootA == rootB) {
    return;
  }
  
  // Change the root of the smaller disjoint set (in terms of number of nodes)
  // to point at the larger.
  if (subsetSize(a) >= subsetSize(b)) {
    sets[rootB] = a;
  } else {
    sets[rootA] = b;
  }
}

size_t
etchasketch::DisjointSetsOfIndices::subsetSize(const int elem)
{
  const size_t numSets = sets.size();
  if (elem < 0 || static_cast<size_t>(elem) > numSets) {
    return 0;
  }
  
  // Count all elements that share our root
  const int elemRootIdx = find(sets.at(elem));
  size_t count = 1;  // We know that the element at `elem` is in the set
  for (size_t i = 0; i < numSets; ++i) {
    if (i != static_cast<size_t>(elem) && find(i) == elemRootIdx) {
      ++count;
    }
  }
  return count;
}
