/*
    Data Structures
    Project Hash: Cuckoo Hashing
    Author: Anthony Lupica <arl127@uakron.edu> 2022

    Driver file using the CuckooHash class

    Test Cases are used to display the functionality and analyze the performance
    of a hash table that uses the cuckoo hashing technique.

    The chosen name - year association is different celebrities and their birth years.
*/

#include "CuckooHash.hpp"
#include "Complexity_Timer.hpp"
#include <iostream>
#include <cassert>
#include <string>

using std::cout;

int main()
{
    //----- Testing Section For Correctness -----//

    cout << "\nTESTING FOR CORRECTNESS\n\n";

    // create a hash table object, call it hashTest, and initialize with a name and birth year
    CuckooHash hashTest("Brad Pitt", 1963);
    
    // insert names and birth years
    hashTest.insert("Natalie Portman", 1981);
    hashTest.insert("Johnny Depp", 1963);
    hashTest.insert("Beyonce", 1981);
    hashTest.insert("Tom Brady", 1977);
    hashTest.insert("Betty White", 1922);

    // call search() on the inserts and verify with asserts that the years match
    assert(hashTest.search("Brad Pitt") == 1963 && "An unexpected birth year was found");
    assert(hashTest.search("Natalie Portman") == 1981 && "An unexpected birth year was found");
    assert(hashTest.search("Johnny Depp") == 1963 && "An unexpected birth year was found");
    assert(hashTest.search("Beyonce") == 1981 && "An unexpected birth year was found");
    assert(hashTest.search("Tom Brady") == 1977 && "An unexpected birth year was found");
    assert(hashTest.search("Betty White") == 1922 && "An unexpected birth year was found");
    
    // call size() for the number of records in the table and verify with an assert 
    assert(hashTest.size() == 6 && "An unexpected size was returned");

    // insert two more records
    hashTest.insert("Ariana Grande", 1993);
    hashTest.insert("Chris Rock", 1965);

    // call size() for the number of records in the table and verify with an assert 
    assert(hashTest.size() == 8 && "An unexpected size was returned");

    // verify that the non duplicate keys rule is supported
    hashTest.insert("Beyonce", 1981);

    // delete a record, test search on that record, verify that size updates, reinsert the same record, test search and size again
    hashTest.remove("Beyonce");
    assert(hashTest.search("Beyonce") == -1 && "An unexpected birth year was found");
    assert(hashTest.size() == 7 && "An unexpected size was returned");
    hashTest.insert("Beyonce", 1981);
    assert(hashTest.search("Beyonce") == 1981 && "An unexpected birth year was found");
    assert(hashTest.size() == 8 && "An unexpected size was returned");

    // test contains()
    assert(hashTest.contains("LeBron james") == 0 && "Found a record that should not exist");
    assert(hashTest.contains("Natalie Portman") == 1 && "A record that should exist was not found");
    assert(hashTest.contains("Tom Brady") == 1 && "A record that should exist was not found");

    // display the hash table
    cout << "\n";
    hashTest.display();

    //-------------------------------------------//

    //---------------- Test Cases ---------------//

    cout << "\nTEST CASES\n";

    timer hashTime;

    cout << "\nProvide a list of celebrities with wide character variation...\n\n";

    const int NUM_CELEB = 30;
    string celebList[NUM_CELEB] = {"Jake Gyllenhaal", "Zendaya", "Tom Holland", "Dax Shepard", "Winona Ryder", "Michael Fassbender", "Ice Cube",
                          "Björk", "Matthew McConaughey", "George Washington", "Julian Casablancas", "Taylor Swift", "Hugh Laurie", 
                          "Alanis Morissette", "Kyrie Irving", "Jason Mraz", "Henry VIII", "Dr. Phil", "Zach Galifianakis", "Adele", "Cardi B", 
                          "Alicia Keys", "Ellen DeGeneres", "Joaquin Phoenix", "Tony Leung", "Drake", "Robert Herjavec", "Idris Elba", "Javier Bardem", "Jay-Z"};
    int birthList[] = {1980, 1996, 1996, 1975, 1971, 1977, 1969, 1965, 1969, 1732, 1978, 1989, 1959, 1974, 1992, 1977, 1491, 1950, 1969, 1988, 1992,
                       1981, 1958, 1974, 1962, 1986, 1962, 1972, 1969, 1969};

    CuckooHash hashCeleb;
    for (int i = 0; i < NUM_CELEB; ++i)
    {
        hashTime.restart();
        hashCeleb.insert(celebList[i], birthList[i]);
        hashTime.stop();

        cout << "insert " << hashCeleb.size() << " for key " << celebList[i] << ": " << hashTime.time() << " seconds\n";
    }
    cout << "\n";
    hashCeleb.display();
    cout << "\n";

    //-------------------------------------------//
}
