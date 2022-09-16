/* 
    file to demonstrate a basic test of the treap class
*/

#include <iostream>
#include "treap.hpp"
#include <iomanip>

using std::cout;
using std::cin;
using std::endl;

int main()
{
    Treap myTreap;

    for (int i = 66; i < 72; ++i)
    {
        myTreap.insert(i);
    }

    myTreap.display();
    cout << "\nC has priority: " << myTreap.search('C') << endl;

    return 0;
}



