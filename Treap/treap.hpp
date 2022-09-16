/*
    treap.hpp

    Project 2 - Treaps
    The include file for a character treap class.

    Author: Anthony Lupica <arl127@uakron.edu>
*/

#ifndef TREAP_HPP_INCLUDED
#define TREAP_HPP_INCLUDED

#include <cstdlib> // cstdlib and ctime for random priority generation
#include <ctime>

const int PRIORITY_RANGE = 51; // const value sets the range of possible priorities for x in: 0 <= x <= PRIORITY_RANGE - 1
const int WIDTH_OFFSET = 10;   // offset for width spacing used by display method

class Treap
{
    private:
        // define a struct node for the treap
        struct TreapNode
        {
            char data;         // the "key" or data stored within a node
            int priority;      // the priority value of a node (randomly generated)
            TreapNode *left;   // pointer to the left child
            TreapNode *right;  // pointer to the right child
        };

        TreapNode *root;        // pointer to the root of the tree
        long bytesOverhead = 0; // bytes of space overhead
        long bytesTotal = 0;    // total bytes of space required

        // PRIVATE METHODS

        // insert() called by overloaded wrapper insert()
        void insert(TreapNode* &nodePtr, char data);

        // search() called by overloaded wrapper search()
        bool search(TreapNode* nodePtr, char data, int &priorityHolder) const;

        // display() called by overloaded wrapper display()
        void display(TreapNode *root, int width) const;

        // helper method right rotate to maintain heap
        void rightRotate(TreapNode* &nodePtr);

        // helper method left rotate to maintain heap
        void leftRotate(TreapNode* &nodePtr);

    public:
        // CONSTRUCTORS

        // default constructor
        Treap()
        {
            // seed rand once per tree
            srand(time(0));

            // assign nullptr to root
            root = nullptr;
        }

        // PUBLIC METHODS

        // overloaded wrapper method insert() calls private insert()
        void insert(char data)
        { insert(root, data); }

        // overloaded wrapper method search() calls private search()
        int search(char data) const;

        // overloaded wrapper method display() calls private display()
        // passes in the root pointer and 0 for initial output width spacing
        void display() const
        { display(root, 0); }

        // getter for total space overhead in bytes
        long spaceOverhead() const
        { return bytesOverhead; }

        // getter for total space required in bytes
        long spaceTotal() const
        { return bytesTotal; }

        // compute the space overhead to data fraction
        long double overheadFraction() const
        { return bytesOverhead / static_cast<double>(bytesTotal); }
};

#endif // TREAP_HPP_INCLUDED
