/*
    treap.cpp

    Project 2 - Treaps
    The implementation file for a character treap class.

    The class features default construction of Treap objects, insertion of nodes
    with char data elements and randomly assigned int priorities, and display.

    The treap is self-balancing, according to the BST and min-heap properties,
    and a treap object keeps a running tally of the total bytes needed to maintain the structure.

    Author: Anthony Lupica <arl127@uakron.edu>
*/

#include "treap.hpp"
#include <iostream>
#include <algorithm>

// insert() called by overloaded wrapper insert()
void Treap::insert(TreapNode* &nodePtr, char data)
{
    // base case: insert location is found at null child.
    // assign newNode to nodePtr
    if (nodePtr == nullptr)
    {
        TreapNode *newNode = nullptr; // pointer for a new node

        // create new node, store data, and assign priority
        newNode = new TreapNode;
        // set data member to data
        newNode->data = data;
        // set priority to random integer x in: 0 <= x <= PRIORITY_RANGE
        newNode->priority = rand() % PRIORITY_RANGE;
        // set children to nullptr
        newNode->left = newNode->right = nullptr;

        // assign to nodePtr
        nodePtr = newNode;

        // update bytes overhead (priority + child pointers)
        bytesOverhead += sizeof(nodePtr->priority) + sizeof(nodePtr->left) + sizeof(nodePtr->right);

        // update total bytes stored (priority + data + child pointers)
        bytesTotal += sizeof(nodePtr->priority) + sizeof(nodePtr->data) + sizeof(nodePtr->left) + sizeof(nodePtr->right);

        return;
    }

    // if data is less than or equal to that in root,
    // recursive call with left subtree
    if (data < nodePtr->data)
    {
        insert(nodePtr->left, data);

        // maintain heap property with right rotate
        // if insertion as left child was successful, and it's priority is higher than (min-heap)
        // it's parent, call right Rotate
        if (nodePtr->left != nullptr && nodePtr->left->priority < nodePtr->priority )
        {
            rightRotate(nodePtr);
        }
    }

    // otherwise (data is greater than or equal to that in root), recursive call with right subtree
    else
    {
        insert(nodePtr->right, data);

        // maintain heap property with left rotate
        if (nodePtr->right != nullptr && nodePtr->right->priority < nodePtr->priority)
        {
            leftRotate(nodePtr);
        }
    }
}

// search() called by overloaded wrapper search()
// returns true if the data is found in the tree and false otherwise
bool Treap::search(TreapNode* nodePtr, char data, int &priorityHolder) const
{
    if (nodePtr == nullptr)
    {
        // the data could not be found
        return false;
    }

    if (nodePtr->data == data)
    {
        // the data was found
        priorityHolder = nodePtr->priority;
        return true;
    }

    // recursive calls

    // call with left subtree
    if (data < nodePtr-> data)
    {
        return search(nodePtr->left, data, priorityHolder);
    }

    // call with right subtree
    if (data > nodePtr-> data)
    {
        return search(nodePtr->right, data, priorityHolder);
    }
}

// overloaded wrapper method search() calls private search()
// returns the priority of the data if found in the tree, and -1 otherwise
int Treap::search(char data) const
{
    int priorityHolder;

    // if data was found
    if(search(root, data, priorityHolder))
    {
        return priorityHolder;
    }

    else
    {
        return -1;
    }
}

// helper method right rotate to maintain heap
void Treap::rightRotate(TreapNode* &nodePtr)
{
    // tempLeft (or nodePtr->left) is the newly inserted node that needs rotation
    TreapNode* tempLeft = nodePtr->left;
    // tempLeftRight (or tempLeft->right) is the right child (currently null) of the
    // newly inserted node.
    TreapNode* tempLeftRight = nodePtr->left->right;

    // do the rotation

    // new nodes right child points to old root
    tempLeft->right = nodePtr;
    // the "root pointer's" left child points to the new node's old right child
    nodePtr->left = tempLeftRight;

    // reassign nodePtr
    nodePtr = tempLeft;
}

// helper method left rotate to maintain heap
void Treap::leftRotate(TreapNode* &nodePtr)
{
    // tempRight (or nodePtr->right) is the newly inserted node that needs rotation
    TreapNode* tempRight = nodePtr->right;
    // tempRightLeft (or tempRight->left) is the left child (currently null) of the
    // newly inserted node.
    TreapNode* tempRightLeft = nodePtr->right->left;

    // do the rotation

    // new nodes left child points to old root
    tempRight->left = nodePtr;
    // the "root pointer's" right child points to the new node's old left child
    nodePtr->right = tempRightLeft;

    // reassign nodePtr
    nodePtr = tempRight;
}

// display() called by overloaded wrapper display()
// outputs the nodes in right->node->left order with constant height spacing
void Treap::display(TreapNode *root, int width) const
{
    if (root)
    {
        width += WIDTH_OFFSET;

        // visit right subtree
        display(root->right, width);

        // display formatted node (in right->node->left order)
        std::cout << std::endl;
        for (int i = WIDTH_OFFSET; i < width; ++i)
        {
            // prints (WIDTH_OFFSET + width) spaces
            std::cout << " ";
        }
        std::cout << root->priority << "|" << root->data << std::endl;

        // visit left subtree
        display(root->left, width);
    }
}
