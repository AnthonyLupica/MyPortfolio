/*
    BoundedQueue.cpp

    University of Akron - 2022
    Anthony Lupica <arl127@uakron.edu>

    This is the implementation file for the bounded queue class.
*/

#include <cstddef>
#include <iostream>
#include "BoundedQueue.hpp"

// C_O_N_S_T_R_U_C_T_O_R_S

/* DEFAULT CONSTRUCTOR

   Pre -- Called upon declaration of a BoundedQueue object
          with no arguments passed.

   Post -- Front and back are set to sentinel value -1,
           size is initialized to 0, and capacity is defaulted
           to 3.
*/
BoundedQueue::BoundedQueue() : iFront(-1), iBack(-1), qSize(0), qCapacity(3)
{
    qArray = new int[qCapacity];
}

/* SIZE DECLARATOR CONSTRUCTOR

   Pre -- Called upon declaration of a BoundedQueue object
          with a size argument passed.
   Post -- Front an back are set to sentinel value -1,
           size is initialized to 0, and capacity is initialized
           to initCapacity.
*/
BoundedQueue::BoundedQueue(std::size_t initCapacity) : iFront(-1), iBack(-1), qSize(0), qCapacity(initCapacity)
{
    qArray = new int[qCapacity];
}

/* SIZE DECLARATOR CONSTRUCTOR WITH INITIALIZER

   Pre -- Called upon declaration of a BoundedQueue object
          with a size argument and initializer value passed.
   Post -- Front is set to 0 and back is set to initCapacity - 1,
           size and capacity are initialized to initCapacity. A loop iterates through the array
           and initializer value is enqueued for each index.
*/
BoundedQueue::BoundedQueue(std::size_t initCapacity, int initializer) : iFront(0), iBack(initCapacity - 1), qSize(initCapacity), qCapacity(initCapacity)
{
    qArray = new int[qCapacity];

    for (int i = 0; i < qCapacity; ++i)
    {
        qArray[i] = initializer;
    }
}

/* COPY CONSTRUCTOR


*/
BoundedQueue::BoundedQueue(const BoundedQueue &obj)
{
    // copy data members
    iFront = obj.iFront;
    iBack = obj.iBack;
    qSize = obj.qSize;
    qCapacity = obj.qCapacity;

    // copy elements over
    // NEEDS WORK
}

/* DESTRUCTOR

   Pre -- N/A
   Post -- heap is freed and underlying pointer is deleted
*/
BoundedQueue::~BoundedQueue()
{
    delete[] qArray;
    delete qArray;
}

// P_R_I_V_A_T_E__M_E_T_H_O_D_S

/* ISFULL

   Pre -- isFull is called by the enqueue method to issue and error message, or called
          by the user.
   Post -- Returns 1 if the queue has no remaining space for additional elements,
           -1 otherwise.
*/
bool BoundedQueue::isFull()
{
    return iBack == qCapacity - 1; // if capacity is 6, an element at arr[5] is occupying the last position
}

void BoundedQueue::grow()
{
    // allocate a temp array that is twice the capacity of the old array
    int *tempQ = new int[qCapacity * 2];


    int j = iFront; // indexer for old queue; initialize to front element

    // post test loop is necessary because traversal of old queue begins at front index,
    // but also needs to end at front index (meaning the traversal has completed a full loop)
    for (int i = 0, j = iFront; j < qSize; ++i, ++j)
    {
        // copy into tempQ; normalize elements (iFront == 0)
        *(tempQ + i) = *(qArray + j);
    }

    // update members
    qCapacity *= 2; // new capacity is twice the old capacity

    // memory clean-up and reassignment of old pointer
    delete[] qArray; // free old heap
    qArray = tempQ; // reassign pointer

    return;
}

// M_E_M_B_E_R__F_U_N_C_T_I_O_N_S

/* ENQUEUE

   Pre -- enqueue receives an int by value to be inserted at the rear of the queue.
   Post -- if the queue is full, return and issue error message. Otherwise, the argument is
           enqueued.
*/
void BoundedQueue::enqueue(int queueVal)
{
    // if the queue is full, we return and state error to user
    if (isFull())
    {
        grow();
    }

    // else if the queue currently has no elements, queueVal
    // becomes both the front and the rear element, and is inserted
    // at index 0
    else if (isEmpty())
    {
        iFront = iBack = 0;
        qArray[0] = queueVal;
        qSize = 1;

        return;
    }

    // Perform enqueue
    ++iBack;
    qArray[iBack] = queueVal;
    ++qSize;

    return;
}

/* DEQUEUE

   Pre -- N/A
   Post -- If the calling queue is empty, a std::cerr message is issued and -1 is returned.
           If the calling queue contains exactly one element, both front and back are reset to -1
           to signal the queue is now empty following dequeue. The front element is popped and the array's
           elements are shifted down by one.
*/
int BoundedQueue::dequeue()
{
    // test if queue contains an element
    if (isEmpty())
    {
        std::cerr << "Queue is already empty\n";
        return -1;
    }

    // store value at front to be returned to the call
    int returnFront = qArray[iFront];

    // if front and back are equal (and not at -1), the element being dequeued is the only element.
    // Restore iFront and iBack to sentinel -1 to signal the empty queue.
    if ((iFront == iBack) && (iBack != -1))
    {
        iFront = iBack = -1;
        --qSize;
        return returnFront;
    }
    // otherwise, perform shifting of elements
    else
    {
        for (int i = 0; i < qSize; ++i)
        {
            qArray[i] = qArray[i + 1];
        }
        std::cout << "shift performed!\n";
    }

    --qSize; // update size member
    --iBack;
    return returnFront;
}

/* ISEMPTY

   Pre -- N/A
   Post -- Returns 1 if the queue has no elements enqueued,
           and -1 otherwise.
*/
bool BoundedQueue::isEmpty()
{
    return iFront == -1;
}

/* SIZEOF

   Pre -- N/A
   Post -- Accessor method for the size data member
*/
std::size_t BoundedQueue::sizeOf() const
{
    return qSize;
}

/* CAPACITYOF

   Pre -- N/A
   Post -- Accessor method for the size data member
*/
std::size_t BoundedQueue::capacityOf() const
{
    return qCapacity;
}

/* GETFRONT

   Pre -- N/A
   Post -- Accessor method for the front index data member
*/
int BoundedQueue::getFront() const
{
    return iFront;
}

/* GETBACK

   Pre -- N/A
   Post -- Accessor method for the back index data member
*/
int BoundedQueue::getBack() const
{
    return iBack;
}

void BoundedQueue::display() const
{
    if (iFront == -1)
    {
        std::cout << "[x]\n";
        return; // there is nothing to display
    }

    std::cout << "| ";
    for (int i = 0; i < qSize; ++i)
    {
        std::cout << qArray[i] << " | ";
    }
    std::cout << "\n";
}
