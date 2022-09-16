/*
    CircularQueue.cpp

    University of Akron - 2022
    Anthony Lupica <arl127@uakron.edu>

    This is the implementation file for the circular queue class.
*/

#include <cstddef>
#include <iostream>
#include "CircularQueue.hpp"

// C_O_N_S_T_R_U_C_T_O_R_S

/* DEFAULT CONSTRUCTOR

   Pre -- Called upon declaration of a CircularQueue object
          with no arguments passed.

   Post -- Front and back are set to sentinel value -1,
           size is initialized to 0, and capacity is initially
           allocated to 3.
*/
CircularQueue::CircularQueue() : iFront(-1), iBack(-1), qSize(0), qCapacity(3)
{
    qArray = new int[qCapacity];
}

/* SIZE DECLARATOR CONSTRUCTOR

   Pre -- Called upon declaration of a CircularQueue object
          with a size argument passed.
   Post -- Front and back are set to sentinel value -1,
           size is initialized to 0, and capacity is initialized
           to the capacity argument.
*/
CircularQueue::CircularQueue(std::size_t initCapacity) : iFront(-1), iBack(-1), qSize(0), qCapacity(initCapacity)
{
    qArray = new int[qCapacity];
}

/* SIZE DECLARATOR CONSTRUCTOR WITH INITIALIZER

   Pre -- Called upon declaration of a CircularQueue object
          with a size argument and initializer value passed.
   Post -- Front is set to 0 and back is set to inititCapacity - 1,
           size and capacity are initialized to initCapacity.
           A loop iterates through the array
           and initializer value is enqueued for each index.
*/
CircularQueue::CircularQueue(std::size_t initCapacity, int initializer) : iFront(0), iBack(initCapacity - 1), qSize(initCapacity), qCapacity(initCapacity)
{
    qArray = new int[qCapacity];

    for (int i = 0; i < qCapacity; ++i)
    {
        qArray[i] = initializer;
    }
}

/* COPY CONSTRUCTOR


*/
CircularQueue::CircularQueue(const CircularQueue &obj)
{
    // copy data members
    iFront = obj.iFront;
    iBack = obj.iBack;
    qSize = obj.qSize;
    qCapacity = obj.qCapacity;

    // allocate new array
    qArray = new int[qCapacity];

    // copy elements over
    // NEEDS WORK
}

/* DESTRUCTOR

   Pre -- N/A
   Post -- heap is freed and underlying pointer is deleted
*/
CircularQueue::~CircularQueue()
{
    delete[] qArray;
    delete qArray;
}

// P_R_I_V_A_T_E__M_E_T_H_O_D_S

/* ISFULL

   Pre -- isFull is called by the enqueue method to signal a need to resize the queue
   Post -- Returns 1 if the queue has no remaining space for additional elements,
           -1 otherwise.
*/
bool CircularQueue::isFull()
{
    // Ex.) if queue capacity is 6, then iBack indexes in the range [0 - 5].
    // if iBack == 5, (iBack + 1) % qCapacity == 6 % 6 == 0 == iFront

    // if iBack == 3, (iBack + 1) % qCapacity == 4 % 6 == 4 == iFront
    // in a full queue, iFront is always adjacent to iBack
    // In this case, the queue has no excess capacity

    return (iBack + 1) % qCapacity == iFront;
}

/* GROW

   Pre -- grow is called by the enqueue method when the calling queue object is full.
   Post -- A temporary pointer (tempQ) is used to allocate an array with twice the capacity of the old array.
           The contents of the old array are copied, and normalized into the new array,
           and the pointer to the old array is reassigned to the new array.
*/
void CircularQueue::grow()
{
    // allocate a temp array that is twice the capacity of the old array
    int *tempQ = new int[qCapacity * 2];

    int i = 0; // indexer for tempQ; begin at 0
    int j = iFront; // indexer for old queue; initialize to old position of front element

    // post test loop is necessary because traversal of old queue begins at front index,
    // but also needs to end at front index (meaning the traversal has completed a full loop)
    do
    {
        // copy into tempQ; normalize elements (iFront == 0)
        *(tempQ + i) = *(qArray + j);

        j = (j + 1) % qCapacity; // Advance index of old queue for next loop, accounting for circular structure
        ++i; // increment i for tempQ
    } while( j != iFront && j < qSize );

    // update members
    qCapacity *= 2; // new capacity is twice the old capacity
    iFront = 0; // normalize front to position 0, iBack still same as before

    // memory clean-up and reassignment of old pointer
    delete[] qArray; // free old heap
    qArray = tempQ; // reassign pointer

    return;
}

// M_E_M_B_E_R__F_U_N_C_T_I_O_N_S

/* ENQUEUE

   Pre -- enqueue receives an int by value to be inserted at the rear of the queue.
   Post -- If the calling queue is full, the capacity is first doubled through the grow member.
           If the calling queue is empty, both front and back are incremented to index 0, and
           the argument is inserted.
           Otherwise iFront is incremented with circular behavior included, and the popped element
           is returned.
*/
void CircularQueue::enqueue(int queueVal)
{
    // if the queue is full, we call the grow helper method
    // to add capacity
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
        qArray[iBack] = queueVal;
        qSize = 1;

        return;
    }

    // if iBack + 1 is less than qCapacity - 1, iBack is incremented.
    // if iBack + 1 = qCapacity  - 1, iBack loops around to index 0
    iBack = (iBack + 1) % qCapacity;
    *(qArray + iBack) = queueVal;
    ++qSize;

    return;
}

/* DEQUEUE

   Pre -- N/A
   Post -- If the calling queue is empty, a std::cerr message is issued and -1 is returned.
           If the calling queue contains exactly one element, both front and back are reset to -1
           to signal the queue is now empty following dequeue.
           The front element is popped and iFront is incremented with circular behavior included.
*/
int CircularQueue::dequeue()
{
    // test if queue contains an element
    if (isEmpty() == 1)
    {
        std::cerr << "Queue is already empty\n";
        return -1;
    }

    // store value at front to be returned to the call
    int returnFront = *(qArray + iFront);

    // if front and back are equal, the element being dequeued is the only element.
    // Restore iFront and iBack to sentinel -1 to signal the empty queue.
    if (iFront == iBack)
    {
        iFront = iBack = -1;
    }
    // otherwise, increment front index
    else
    {
        iFront = (iFront + 1) % qCapacity;
    }

    --qSize; // update size member
    return returnFront;
}

/* ISEMPTY

   Pre -- N/A
   Post -- Returns 1 if the queue has no elements enqueued,
           and -1 otherwise.
*/
bool CircularQueue::isEmpty()
{
    return iFront == -1;
}

/* SIZEOF

   Pre -- N/A
   Post -- Accessor method for the size data member
*/
std::size_t CircularQueue::sizeOf() const
{
    return qSize;
}

/* CAPACITYOF

   Pre -- N/A
   Post -- Accessor method for the size data member
*/
std::size_t CircularQueue::capacityOf() const
{
    return qCapacity;
}

/* GETFRONT

   Pre -- N/A
   Post -- Accessor method for the front index data member
*/
int CircularQueue::getFront() const
{
    return iFront;
}

/* GETBACK

   Pre -- N/A
   Post -- Accessor method for the back index data member
*/
int CircularQueue::getBack() const
{
    return iBack;
}

void CircularQueue::display() const
{
    if (iFront == -1)
    {
        std::cout << "[x]\n";
        return; // there is nothing to display
    }

    int i = iFront; // indexer to traverse the queue, beginning at the front element
    int passCount = 0; // counts number of elements that have been displayed

    // do while traverses as long as i is not the size of the queue
    std::cout << "| ";
    do
    {
        std::cout << *(qArray + i) << " | ";
        ++passCount;

        // Advance index of queue for next loop, accounting for circular structure
        i = (i + 1) % qCapacity;

        // while condition breaks loop once i loops around to iFronts value
        // or if i is equal to the amount of elements in the queue (i.e. all elements displayed)
    } while( i != iFront && passCount < qSize );

    std::cout << "\n";
}







