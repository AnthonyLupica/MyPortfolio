/*
    Data Structures
    Project Hash: Cuckoo Hashing
    Author: Anthony Lupica <arl127@uakron.edu> 2022

    Implementation file for a hash table class using the cuckoo hashing technique.

    The hash table implements a name - year pair lookup. The name of a person 
    can be used as the key to quickly find an associated year.
    
    Ex.] Birth Year
    std::cout << CuckooHash.search("Brad Pitt");
        --> "1963"
    
    The user may decide which logical assiocation to use between names and years.
*/

#include "CuckooHash.hpp"
#include <iostream>
#include <cmath> 

/* Default Constructor 
*
*  Initialize table size to the first value in PRIME_LIST. 
*  When a rehash is necessary, the table size
*  will use the next value in PRIME_LIST (double previous table size and round up to the nearest
*  prime number).
*/
CuckooHash::CuckooHash() : tableSize(PRIME_LIST[0]), tableSizeCounter(0), nodeCount1(0), nodeCount2(0)
{
    table1 = new HashNode[tableSize];
    table2 = new HashNode[tableSize];
}

/* key-value Constructor 
*
*  Initialize table size to the first value in PRIME_LIST. 
*  When a rehash is necessary, the table size
*  will use the next value in PRIME_LIST (double previous table size and round up to the nearest
*  prime number). Take in an intital key and value to pass to insert().
*/
CuckooHash::CuckooHash(const string &key, const int value) : tableSize(PRIME_LIST[0]), tableSizeCounter(0), nodeCount1(0), nodeCount2(0)
{
    table1 = new HashNode[tableSize];
    table2 = new HashNode[tableSize];

    // call insert with given key and value
    insert(key, value);
}

/* ~Destructor() 
*
*  Destructs both hash tables used for the CuckooHash object.
*/
CuckooHash::~CuckooHash()
{
    delete[] table1;
    delete[] table2;
}

/* insert() 
*
*  If the given key is unique, and the given value is a four digit number, the record 
*  will be inserted at the home slot computed by the hash function for table 1. If either table
*  is at or over half full, the tableSize is first rehashed. If there was already an occupant in the home
*  slot, that occupant is evicted and passed to evictToTwo() for reseating. In the event of an eviction cycle, 
*  specifically determined by log N evictions (where N is the table size), rehash is called. 
*/
void CuckooHash::insert(const string &key, const int value)
{
    // compute both hash values for checking if this key is a duplicate
    int homePosition = hash1(key);     // position found for the first table 
    int evictionPosition = hash2(key); // position found for the second table

    // CONDITION ONE: key must be unique amongst both tables 
    // use hash values to index into the tables and verify this key is unique
    if (table1[homePosition].name == key || table2[evictionPosition].name == key)
    {
        std::cerr << "key " << "'" << key << "' " << "already exists within the hash table\n";
        
        return;
    }
    
    // CONDITION TWO: value must be four digits
    if (!isFourDigit(value)) 
    {
        std::cerr << "The year must be in four-digit form\n";

        return;
    }

    // CONDITION THREE: We have good data, now check that both tables are less than half full.
    // if not, call rehash()
    if ((nodeCount1 >= tableSize / 2) || (nodeCount2 >= tableSize / 2))
    {
        if (rehash() == 1)
        {
            return;
        }

        // need to recompute the hash value 1 with new tableSize 
        homePosition = hash1(key); 
    }
    
    // try to insert in the home position
    
    // save a temporary copy of the data already there, if it exists
    string tempKey;
    int tempValue = 0;
    bool needEvict = 0;
    if (!table1[homePosition].name.empty())
    {
        tempKey = table1[homePosition].name;
        tempValue = table1[homePosition].year;
        needEvict = 1;
    }

    // new data replaces the old occupant as the new owner of the index
    table1[homePosition].name = key;
    table1[homePosition].year = value;
    
    // only increment nodeCount1 if the new key didn't evict a record
    // (in which case we would be adding a record to table1 but also removing a record from table1)
    if (!needEvict)
    {
        ++nodeCount1;

        return;
    }
    // if there was an eviction (flag is turned on), then call evictToTwo()
    else 
    {
        // pass in an initial value of 0 for the evictCount
        evictToTwo(tempKey, tempValue, 0);
    }

    return;
}

/* search() 
*
*  looks first in table 1 to see if the key can be found at its hash location.
*  If not present, looks instead in table 2 for the record. If found in either table,
*  the year is returned. If the record is not found at either hash location, -1 is returned.
*/
int CuckooHash::search(const string &key)
{
    int homePosition = hash1(key); // position found for the first table 

    // if the key at that index matches the key argument, return the year
    if (table1[homePosition].name == key)
    {
        return table1[homePosition].year;
    }
    else
    {
        int evictionPosition = hash2(key); // position found for the second table
        
        // if the key at that index matches the key argument, return the year
        if (table2[evictionPosition].name == key)
        {
            return table2[evictionPosition].year;
        }
    }

    // -1 signals that the record could not be found
    return -1;
}

/* hash1 
*
*  hash function for table 1 
*/
int CuckooHash::hash1(const string &key)
{
    // iterate through the string and find a hash value according 
    // to character ascii values (dependent on their position i within the string)
    
    int hashTotal = 0; 
    int mult = 29;              
    for (int i = 0; i < key.length(); ++i)
    {   
        mult += i; 
        hashTotal += key[i] * mult;
    }

    return hashTotal % tableSize;
}

/* hash2 
*
*  hash function for table 2
*/
int CuckooHash::hash2(const string &key)
{
    // iterate through the string and find a hash value according 
    // to character ascii values (dependent on their position i within the string)
    
    int hashTotal = 0; 
    int mult = 31;              
    for (int i = 0; i < key.length(); ++i)
    {    
        hashTotal += mult * (key[i] + i);
    }

    return hashTotal % tableSize;
}

/* isFourDigit()
*
*  in a loop, divides by ten until the value is 0,
*  each iteration incrementing a counter.
*  Ex.] 1234 / 10 = 123    (++numDigits)
*        123 / 10 = 12     (++numDigits)
*         12 / 10 = 1      (++numDigits)
*          1 / 10 = 0      (++numDigits)
*          end loop
*  If numDigits is anything other than 4, isFourDigit returns 0,
*  and otherwise returns 1.
*/        
bool CuckooHash::isFourDigit(const int value)
{
    int numDigits = 0;
    int tempValue = value;
    while (tempValue != 0)
    {
        tempValue /= 10;
        ++numDigits;
    }
    
    if (numDigits == 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* evictToOne() 
*
*  Reseats the key - value pair in table 1. If there is a previous occupant.
*  Call evictToTwo() with that occupant. The result is a "ping-pong" effect back and forth 
*  until no eviction is necessary.
*/
void CuckooHash::evictToOne(const string &key, const int value, int staticPass)
{
    // when evictCount is log n, we rehash
    static int evictCount; 
    evictCount = staticPass;
    ++evictCount;

    // if evictCount is greater than or equal to log(N), rehash
    if (evictCount >= log2(nodeCount1 + nodeCount2))
    {
        if (rehash() == 1)
        {
            return;
        } 

        // reset evictCount to 0 after rehash
        evictCount = 0;
    }

    // try to insert in table1
    // compute the hash value for table 1
    int hashVal1 = hash1(key); 
    
    // save a temporary copy of the data already there, if it exists
    string tempKey;
    int tempValue = 0;
    bool needEvict = 0;
    if (!table1[hashVal1].name.empty())
    {
        tempKey = table1[hashVal1].name;
        tempValue = table1[hashVal1].year;
        needEvict = 1;
    }

    // new data replaces the old occupant as the new owner of the index
    table1[hashVal1].name = key;
    table1[hashVal1].year = value;
    
    // only increment nodeCount1 if the new key didn't evict a record
    // (in which case we would be adding a record to table1 but also removing a record from table1)
    if (!needEvict)
    {
        ++nodeCount1;

        return;
    }
    // if there was an eviction (flag is turned on), then call evictToTwo()
    else 
    {
        evictToTwo(tempKey, tempValue, evictCount);
    }

    return;
}

/* evictToTwo() 
*
*  Reseats the key - value pair in table 2. If there is a previous occupant.
*  Call evictToOne() with that occupant. The result is a "ping-pong" effect back and forth 
*  until no eviction is necessary.
*/
void CuckooHash::evictToTwo(const string &key, const int value, int staticPass)
{
    // when evictCount is log n, we rehash
    static int evictCount; 
    evictCount = staticPass;
    ++evictCount;

    // if evictCount is greater than or equal to log(N), rehash
    if (evictCount >= log2(nodeCount1 + nodeCount2))
    {
        if (rehash() == 1)
        {
            return;
        } 

        // reset evictCount to 0 after rehash
        evictCount = 0;
    }

    // try to insert in table2
    // compute the hash value for table 2
    int hashVal2 = hash2(key); 
    
    // save a temporary copy of the data already there, if it exists
    string tempKey;
    int tempValue = 0;
    bool needEvict = 0;
    if (!table2[hashVal2].name.empty())
    {
        tempKey = table2[hashVal2].name;
        tempValue = table2[hashVal2].year;
        needEvict = 1;
    }

    // new data replaces the old occupant as the new owner of the index
    table2[hashVal2].name = key;
    table2[hashVal2].year = value;
    
    // only increment nodeCount2 if the new key didn't evict a record
    // (in which case we would be adding a record to table2 but also removing a record from table2)
    if (!needEvict)
    {
        ++nodeCount2;

        return;
    }
    // if there was an eviction (flag is turned on), then call evictToOne()
    else 
    {
        evictToOne(tempKey, tempValue, evictCount);
    }

    return;
}

/* rehash() 
*
*  uses the next value in PRIME_LIST (approximately double the size), to allocate new tables.
*  All records in the old tables are then rehashed to the new tables using the new table size.
*  If the user has stored enough records such that there is no next value in PRIME_LIST, the demo
*  has reached it's conclusion and no new records may be inserted.
*/
bool CuckooHash::rehash() 
{
    if (tableSizeCounter == LENGTH_PRIME - 1)
    {
        std::cerr << "You've reached the maximum size for this hash table demo.\n";

        // return 1 to signal insert() to return without attempting to insert
        return 1;
    }

    // increment the index to use for selecting a prime number from PRIME_LIST
    ++tableSizeCounter;    
    // store tempTableSize as a copy of tableSize  
    int tempTableSize = tableSize;
    // update tableSize using new PRIME_LIST index
    tableSize = PRIME_LIST[tableSizeCounter];

    // allocate new (temporary tables with increased size)
    tempTable1 = new HashNode[tableSize];
    tempTable2 = new HashNode[tableSize];

    // reset nodeCount1 and nodeCount2 to allow the rehash loop to recompute these values 
    // (as the distribution of records is very likely to change)
    nodeCount1 = 0;
    nodeCount2 = 0;

    // loop through the elements for table1 and table2, and rehash all intialized nodes to the temporary tables
    // use the old tableSize for the loop condition. Note: tableSize has already been updated, so any calls to hash1() or hash2()
    // correctly mod over the increased size. Further, see that the records are "renormalized" by calling insert again, in that 
    // the first table will be the prime objective for hash slots.
    for (int i = 0; i < tempTableSize; ++i)
    {
        if (!table1[i].name.empty())
        {
            // call overloaded insert()
            // It will hash its argument to tempTable1.
            insert(table1[i].name, table1[i].year, 0);
        }
        if (!table2[i].name.empty())
        {
            // call overloaded insert()
            // It will hash its argument to tempTable1.
            insert(table2[i].name, table2[i].year, 0);
        }
    }

    // delete the old arrays
    delete[] table1;
    delete[] table2;

    // point old array pointers to new arrays
    table1 = tempTable1;
    table2 = tempTable2;

    // make temp pointers point to null
    tempTable1 = nullptr;
    tempTable2 = nullptr;

    // 0 for good reallocation
    return 0;
}

/* contains()
*
*  returns true if the key is found in the table, and false otherwise
*/
bool CuckooHash::contains(const string &key)
{
    int homePosition = hash1(key); // position found for the first table 

    // if the key at that index matches the key argument, return true
    if (table1[homePosition].name == key)
    {
        return 1;
    }
    else
    {
        int evictionPosition = hash2(key); // position found for the second table
        
        // if the key at that index matches the key argument, return true
        if (table2[evictionPosition].name == key)
        {
            return 1;
        }
    }

    // return false if the element was not found
    return 0;
}

/* remove()
*
*  deletes the record if it exists in either table, and otherwise does nothing. This version 
*  of a cuckoo delete does not promote a record from table 2 to table 1 when a record is deleted from table 1.
*/
void CuckooHash::remove(const string &key)
{
    int whichTable = -1;
    int index = position(key, whichTable);

    // if the key is in the table
    if (index != -1)
    {
        if (whichTable == 1)
        {
            // make name empty so this index operates as an uninitialized node
            table1[index].name = "";

            // decrement nodeCount1
            --nodeCount1;
        }
        if (whichTable == 2)
        {
            // make name empty so this index operates as an uninitialized node
            table2[index].name = "";

            // decrement nodeCount2
            --nodeCount2;
        }
        
        return;
    }
    else 
    {
        std::cerr << "key " << "'" << key << "' " << "does not exist within the table\n";

        return;
    }
}

/* position()
*
*  helper for delete(). Implements a variation of search() and contains() which returns the index of the record,
*  if it is found, and -1 otherwise. Also updates a reference parameter "whichTable" to 1 or 2, which tells delete()
*  which table to use the returned index for.
*/
int CuckooHash::position(const string &key, int &whichTable)
{
    int homePosition = hash1(key); // position found for the first table 

    // if the key at that index matches the key argument, return the index
    if (table1[homePosition].name == key)
    {
        whichTable = 1; // for table 1
        return homePosition;
    }
    else
    {
        int evictionPosition = hash2(key); // position found for the second table
        
        // if the key at that index matches the key argument, return the index
        if (table2[evictionPosition].name == key)
        {
            whichTable = 2; // for table 2
            return evictionPosition;
        }
    }

    // return -1 to delete() to signal that there is no record to delete
    return -1;
}

void CuckooHash::display() const
{
    for (int i = 0; i < tableSize; ++i) 
    {
        // if the key at this index has a value, display the key and value
        if (!table1[i].name.empty())
        {
            std::cout << table1[i].name << " : " << table1[i].year << "\n";
        }

        // do the same now for table 2
        if (!table2[i].name.empty())
        {
            std::cout << table2[i].name << " : " << table2[i].year << "\n";
        }
    }
    // output a new line
    std::cout << "\n";
}

/* insert() 
*
*  Overloaded for use as a helper for rehash().
*  This version is stripped down, because it does not need to do any validation.
*  It will also not call rehash() under any circumstances, given that it was just called 
*  by rehash() itself.
*/
void CuckooHash::insert(const string &key, const int value, int signal)
{
    // we do nothing with "signal". It's purpose is purely to give rehash() an alternate signature of 
    // insert() to call

    // compute hash value 
    int homePosition = hash1(key);     
     
    // try to insert in the home position
    
    // save a temporary copy of the data already there, if it exists
    string tempKey;
    int tempValue = 0;
    bool needEvict = 0;
    if (!tempTable1[homePosition].name.empty())
    {
        tempKey = tempTable1[homePosition].name;
        tempValue = tempTable1[homePosition].year;
        needEvict = 1;
    }

    // new data replaces the old occupant as the new owner of the index
    tempTable1[homePosition].name = key;
    tempTable1[homePosition].year = value;
    
    // only increment nodeCount1 if the new key didn't evict a record
    // (in which case we would be adding a record to tempTable1 but also removing a record from tempTable1)
    if (!needEvict) 
    {
        ++nodeCount1;

        return;
    }
    // if there was an eviction (flag is turned on), then call evictToTwo()
    else
    {
        evictToTwo(tempKey, tempValue);
    }

    return;
}

/* evictToOne() 
*
*  Overloaded evictToOne() for use by overloaded insert().
*  This version does not check for an eviction cycle, and will not call rehash()
*/
void CuckooHash::evictToOne(const string &key, const int value)
{
    // try to insert in tempTable1
    // compute the hash value for tempTable1
    int hashVal1 = hash1(key); 
    
    // save a temporary copy of the data already there, if it exists
    string tempKey;
    int tempValue = 0;
    bool needEvict = 0;
    if (!tempTable1[hashVal1].name.empty())
    {
        tempKey = tempTable1[hashVal1].name;
        tempValue = tempTable1[hashVal1].year;
        needEvict = 1;
    }

    // new data replaces the old occupant as the new owner of the index
    tempTable1[hashVal1].name = key;
    tempTable1[hashVal1].year = value;
    
    // only increment nodeCount1 if the new key didn't evict a record
    // (in which case we would be adding a record to tempTable1 but also removing a record from tempTable1)
    if (!needEvict)
    {
        ++nodeCount1;

        return;
    }
    // if there was an eviction (flag is turned on), then call evictToTwo()
    else 
    {
        evictToTwo(tempKey, tempValue);
    }

    return;
}
        
/* evictToTwo() 
*
*  Overloaded evictToTwo() for use by overloaded insert().
*  This version does not check for an eviction cycle, and will not call rehash()
*/
void CuckooHash::evictToTwo(const string &key, const int value)
{
    // try to insert in tempTable2
    // compute the hash value for tempTable2
    int hashVal2 = hash2(key); 
    
    // save a temporary copy of the data already there, if it exists
    string tempKey;
    int tempValue = 0;
    bool needEvict = 0;
    if (!tempTable2[hashVal2].name.empty())
    {
        tempKey = tempTable2[hashVal2].name;
        tempValue = tempTable2[hashVal2].year;
        needEvict = 1;
    }

    // new data replaces the old occupant as the new owner of the index
    tempTable2[hashVal2].name = key;
    tempTable2[hashVal2].year = value;
    
    // only increment nodeCount2 if the new key didn't evict a record
    // (in which case we would be adding a record to tempTable2 but also removing a record from tempTable2)
    if (!needEvict)
    {
        ++nodeCount2;

        return;
    }
    // if there was an eviction (flag is turned on), then call evictToOne()
    else 
    {
        evictToOne(tempKey, tempValue);
    }

    return;
} 
