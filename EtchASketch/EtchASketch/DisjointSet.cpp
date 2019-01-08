//
//  DisjointSet.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/15/18.
//  Copyright © 2018 Justin Loew. All rights reserved.
//

#include "DisjointSet.hpp"

template <class Element>
etchasketch::DisjointSet<Element>::DisjointSet(Element &value)
{
  this->value = value;
  this->parent = *this;
}

template <class Element>
etchasketch::DisjointSet<Element> & etchasketch::DisjointSet<Element>::root() const
{
  // Update our root to point to the current highest-level node
  if (parent != *this) {
    parent = parent.root();
  }
  
  return parent;
}

template <class Element>
void etchasketch::DisjointSet<Element>::unionWith(DisjointSet<Element> &newParent)
{
  DisjointSet<Element> &currentRoot = root();
  currentRoot.setParent(newParent);
}

template <class Element>
void etchasketch::DisjointSet<Element>::setParent(DisjointSet<Element> &newParent)
{
  static_assert(isRoot(), "Attempting to set parent on non-root node is a logical error");
  parent = newParent;
}
