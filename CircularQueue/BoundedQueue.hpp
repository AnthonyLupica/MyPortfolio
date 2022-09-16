/*
    BoundedQueue.hpp

    This is the include file for the bounded queue class.
    This queue performs shifts to keep the front element stationary
    on index 0
*/

#ifndef BOUNDEDQUEUE_HPP_INCLUDED
#define BOUNDEDQUEUE_HPP_INCLUDED

class BoundedQueue
{
    private:
        int iFront; // indexes the front of the queue (dequeue)
        int iBack; // indexes the back of the queue (enqueue)
        std::size_t qSize; // number of elements enqueued
        std::size_t qCapacity; // capacity for elements
        int* qArray; // pointer for the underlying dynamic "non-drifting" array

        // PRIVATE METHODS
        bool isFull();
        void grow(); // doubles capacity of given queue

    public:
        // CONSTRUCTORS
        BoundedQueue();
        BoundedQueue(std::size_t);
        BoundedQueue(std::size_t, int);
        BoundedQueue(const BoundedQueue &);
        ~BoundedQueue();

        // MEMBER FUNCTIONS
        void enqueue(int);
        int dequeue();
        bool isEmpty();
        std::size_t sizeOf() const;
        std::size_t capacityOf() const;
        int getFront() const;
        int getBack() const;
        void display() const;
};


#endif // BOUNDEDQUEUE_HPP_INCLUDED
