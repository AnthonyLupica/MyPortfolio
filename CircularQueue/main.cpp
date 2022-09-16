/*
    main.cpp

    This is the driver and test case file for the circular queue and bounded queue classes.

    known bugs: when a circular queue is enqueued and displayed in a loop, with the
                initial value of iFront != 0, the loop crashes, and incorrect updates
                to data members are made.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "CircularQueue.hpp"
#include "BoundedQueue.hpp"

int main()
{
    using std::setw;

    // values for use in test cases
    int testCapacity, testVal, testValUser = 0;

    // Adjust test values here (not testValUser)
    testCapacity = 6;
    testVal = 3;

    // B-QUEUE DECLARATION
    BoundedQueue bQueue(testCapacity);
    std::cout << "B_O_U_N_D_E_D__Q_U_E_U_E\n\n";
    std::cout << "Declare 'bQueue' with initial capacity " << testCapacity << " ...\n";
    std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n\n";

    // C-QUEUE DECLARATION
    CircularQueue cQueue(testCapacity);
    std::cout << "C_I_R_C_U_L_A_R__Q_U_E_U_E\n\n";
    std::cout << "Declare 'cQueue' with initial capacity " << testCapacity << " ...\n";
    std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n\n";

    // ENQUEUE B-QUEUE
    bQueue.enqueue(testVal);
    std::cout << "T_E_S_T__A_N_D__C_O_M_P_A_R_E\n\n";
    std::cout << "Enqueue 'bQueue' with value " << testVal << " ...\n";
    bQueue.display();
    std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n";

    // ENQUEUE C-QUEUE
    cQueue.enqueue(testVal);
    std::cout << "Enqueue 'cQueue' with value " << testVal << " ...\n";
    cQueue.display();
    std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n";

    // DEQUEUE B-QUEUE
    bQueue.dequeue();
    std::cout << "Dequeue 'bQueue' ...\n";
    bQueue.display();
    std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n";

    // DEQUEUE C-QUEUE
    cQueue.dequeue();
    std::cout << "Dequeue 'cQueue' ...\n";
    cQueue.display();
    std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n";

    // USER ENQUEUE B-QUEUE
    std::cout << "Get three enqueue values for 'bQueue' ...\n";
    std::cout << "(Invalid input defaults to 0)\n\n";

    int iCount = 0;
    do
    {
        std::cout << "bQueue[" << bQueue.getBack() + 1 << "] -> ";
        std::cin >> testValUser;

        // test for bad input
        if (std::cin.fail())
        {
            testValUser = 0;
            std::cin.clear();
            std::cin.ignore();
        }

        // enqueue user input
        bQueue.enqueue(testValUser);
        ++iCount;

        // display queue after input
        bQueue.display();
        std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
        std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
        std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
        std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n";
    }while (iCount < 3);

    // DEQUEUE USER ENQUEUE B-QUEUE
    std::cout << "Dequeue user enqueues for 'bQueue' ...\n";
    bQueue.dequeue();
    bQueue.display();
    std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n";

    bQueue.dequeue();
    bQueue.display();
    std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n";

    bQueue.dequeue();
    bQueue.display();
    std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n";

    // USER ENQUEUE C-QUEUE
    std::cout << "Get three enqueue values for 'cQueue' ...\n";
    std::cout << "(Invalid input defaults to 0)\n\n";

    iCount = 0;
    do
    {
        std::cout << "cQueue[" << cQueue.getBack() + 1 << "] -> ";
        std::cin >> testValUser;

        // test for bad input
        if (std::cin.fail())
        {
            testValUser = 0;
            std::cin.clear();
            std::cin.ignore();
        }

        // enqueue user input
        cQueue.enqueue(testValUser);
        ++iCount;

        // display queue after input
        cQueue.display();
        std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
        std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
        std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
        std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n";
    }while (iCount < 3);

    // DEQUEUE USER ENQUEUE C-QUEUE
    std::cout << "Dequeue user enqueues for 'cQueue' ...\n";
    std::cout << "(Dequeue on circular queue causes front index to drift. Empty queue resets Front and Back to -1)\n\n";
    cQueue.dequeue();
    cQueue.display();
    std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n";

    cQueue.dequeue();
    cQueue.display();
    std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n";

    cQueue.dequeue();
    cQueue.display();
    std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
    std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
    std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
    std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n";

    // USER ENQUEUE LOOP B-QUEUE
    std::cout << "Loop input for 'bQueue' ...\n";
    std::cout << "(Invalid input defaults to 0)\n";
    std::cout << "Enter -1 to quit\n\n";

    do
    {
        std::cout << "bQueue[" << bQueue.getBack() + 1 << "] -> ";
        std::cin >> testValUser;

        // break on -1
        if (testValUser == -1)
        {
            std::cout << "\n";
            break;
        }

        // test for bad input
        if (std::cin.fail())
        {
            testValUser = 0;
            std::cin.clear();
            std::cin.ignore();
        }

        // enqueue user input
        bQueue.enqueue(testValUser);

        // display queue after input
        bQueue.display();
        std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
        std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
        std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
        std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n";
    }while (bQueue.sizeOf() <= bQueue.capacityOf());

    // USER ENQUEUE LOOP C-QUEUE
    std::cout << "Loop input for 'cQueue' ...\n";
    std::cout << "(Invalid input defaults to 0)\n";
    std::cout << "Enter -1 to quit\n\n";

    do
    {
        std::cout << "cQueue[" << cQueue.getBack() + 1 << "] -> ";
        std::cin >> testValUser;

        // break on -1
        if (testValUser == -1)
        {
            std::cout << "\n";
            break;
        }

        // test for bad input
        if (std::cin.fail())
        {
            testValUser = 0;
            std::cin.clear();
            std::cin.ignore();
        }

        // enqueue user input
        cQueue.enqueue(testValUser);

        // display queue after input
        cQueue.display();
        std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
        std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
        std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
        std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n";
    }while (cQueue.sizeOf() <= cQueue.capacityOf());

    // USER DEQUEUE LOOP B-QUEUE
    std::cout << "Loop dequeue for 'bQueue' ...\n";

    do
    {
        // dequeue user input
        bQueue.dequeue();

        // display queue after input
        bQueue.display();
        std::cout << setw(20) << "Front Index: " << bQueue.getFront() << "\n";
        std::cout << setw(20) << "Back Index: " << bQueue.getBack() << "\n";
        std::cout << setw(20) << "Size: " << bQueue.sizeOf() << "\n";
        std::cout << setw(20) << "Capacity: " << bQueue.capacityOf() << "\n\n";
    }while (bQueue.getFront() != -1);

    // USER DEQUEUE LOOP C-QUEUE
    std::cout << "Loop dequeue for 'cQueue' ...\n";

    do
    {
        // dequeue user input
        cQueue.dequeue();

        // display queue after input
        cQueue.display();
        std::cout << setw(20) << "Front Index: " << cQueue.getFront() << "\n";
        std::cout << setw(20) << "Back Index: " << cQueue.getBack() << "\n";
        std::cout << setw(20) << "Size: " << cQueue.sizeOf() << "\n";
        std::cout << setw(20) << "Capacity: " << cQueue.capacityOf() << "\n\n";
    }while (cQueue.getFront() != -1);

    return 0;
}
