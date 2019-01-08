//
//  DisjointSet.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/15/18.
//  Copyright © 2018 Justin Loew. All rights reserved.
//

#ifndef DisjointSet_hpp
#define DisjointSet_hpp

#include <unordered_set>

namespace etchasketch {
  
  /**
   * TODO: Add docs.
   */
  template <class Element>
  class DisjointSet {
  public:
    
    DisjointSet(Element &value);
    
    /// Access the contained value.
    Element & element() const {
      return this->value;
    }
    
    /// Access the root node of the tree, updating others' values along the way.
    DisjointSet<Element> & root() const;
    
    /// Whether this node is the root of its tree.
    bool isRoot() const {
      return root() == *this;
    }
    
    /**
     * Combine this DisjointSet with another.
     *
     * @param newParent The tree which will become the root of the combined
     * tree.
     */
    void unionWith(DisjointSet<Element> &newParent);
    
  private:
    Element &value;
    DisjointSet<Element> &parent;
    
    void setParent(DisjointSet<Element> &newParent);
  };
  
  
  /**
   * A collection of DisjointSets.
   */
  template <class Element>
  class DisjointForest {
  public:
    DisjointForest(std::unordered_set<Element> &values);
    
    DisjointForest()
    : DisjointForest(std::unordered_set<Element>())
    {}
    // TODO: Finish describing this class.
    
  private:
    std::unordered_set<DisjointSet<Element>> djs;
    
  public:
    class iterator {
    public:
      iterator(typename std::unordered_set<DisjointSet<Element>>::iterator wrappedIter)
      : wrappedIter(wrappedIter) { }
      
      iterator operator++() {
        return iterator(wrappedIter++);
      }
      
    private:
      typename std::unordered_set<DisjointSet<Element>>::iterator wrappedIter;
    };
    
    iterator begin() {
      return iterator(djs.begin());
    }
    iterator end() {
      return iterator(djs.end());
    }
  };
}

#endif /* DisjointSet_hpp */
