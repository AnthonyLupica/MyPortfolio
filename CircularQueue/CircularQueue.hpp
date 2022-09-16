/*
    CircularQueue.hpp

    This is the include file for the circular queue class.
    This queue allows the front element to be arbitrarily placed,
    eliminating the need for an element shift.
*/

#ifndef CIRCULARQUEUE_HPP_INCLUDED
#define CIRCULARQUEUE_HPP_INCLUDED

class CircularQueue
{
    private:
        int iFront; // indexes the front of the queue (dequeue)
        int iBack; // indexes the back of the queue (enqueue)
        std::size_t qSize; // number of elements enqueued
        std::size_t qCapacity; // capacity of the queue
        int* qArray; // pointer for the underlying dynamic array

        // PRIVATE METHODS
        bool isFull();
        void grow(); // doubles capacity of given queue

    public:
        // CONSTRUCTORS
        CircularQueue();
        CircularQueue(std::size_t);
        CircularQueue(std::size_t, int);
        CircularQueue(const CircularQueue &);
        ~CircularQueue();

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

#endif // CIRCULARQUEUE_HPP_INCLUDED
