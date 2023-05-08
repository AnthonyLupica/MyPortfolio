/*
    lzwAlgorithm435M.hpp 

    contains the core algorithms invoked by driver lzw.cpp for lzw compression Part 2

    yes, I know I'm not supposed to put code in a header file
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <map>

/* This code is derived in parts from LZW@RosettaCode for UA CS435 */

// Compress a string to a list of output symbols.
// The result will be written to the output iterator
// starting at "result"; the final iterator is returned.
template <typename Iterator>
Iterator compress(const std::string &uncompressed, Iterator result) 
{
   /* INITIALIZE THE DICTIONARY */

   int dictSize = 256;                    // start with 256. 
   std::map<std::string, int> dictionary; // dictionary maps strings to integers
   for (int i = 0; i < dictSize; ++i)
   {
      // from 0-255, map character representation to integer representation
      dictionary[std::string(1, i)] = i;
   }
   
   /* BUILD OUT THE DICTIONARY FOR INPUT STRING */

   // represents longest matching "prefix" for the next iteration
   // initialized to empty string
   std::string w; 

   // loop through each character of uncompressed string
   for (std::string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it) 
   {
      // store character at this iteration 
      char c = *it;

      // append the previous longest prefix with this character
      std::string wc = w + c;
      if (dictionary.count(wc))
      {
         // if already in dictionary, this is the new longest prefix
         w = wc;
      }

      // if not already in dictionary
      else 
      {
         // write code for previous longest prefix to output buffer and increment for next iteration
         *result++ = dictionary[w];

         // Add wc to the dictionary. Assuming the size is 65,536!!!
         if (dictionary.size() < 65536)
         {
            dictionary[wc] = dictSize++;
         }
   
         // new longest prefix is the current character 
         w = std::string(1, c);
      }
   }
   
   // Output the code for w.
   if (!w.empty())
   {
      *result++ = dictionary[w];
   }
      
   return result;
}
 
// Decompress a list of output ks to a string.
// "begin" and "end" must form a valid range of ints
template <typename Iterator>
std::string decompress(Iterator begin, Iterator end) 
{
   /* INITIALIZE THE DICTIONARY */

   int dictSize = 256;                   // start with 256.
   std::map<int,std::string> dictionary; // dictionary maps strings to integers
   for (int i = 0; i < dictSize; ++i)
   {
      // from 0-255, map character representation to integer representation
      dictionary[i] = std::string(1, i);
   }
   
   // get first code in sequence of codes and assign translation to result 
   std::string w(1, *begin++);
   std::string result = w;

   // temp string to store the translation to be appended to result for each iteration 
   std::string entry;
   for (; begin != end; begin++) 
   {
      // store current code from compressed 
      int k = *begin;

      if (dictionary.count(k))
      {
         // if there's a representation in dictionary, output the translation
         entry = dictionary[k];
      }
      else if (k == dictSize)
      {
         // special case: not found in dictionary output previous word + first char of previous word
         entry = w + w[0]; 
      }
      else 
      {
         throw "Bad compressed k";
      }
   
      // append temp string to result string
      result += entry;
   
      // Add w+entry[0] to the dictionary. 65,536 = 2^16
      if (dictionary.size() < 65536) 
      {
         // add to the dictionary the previous word plus first char of temp entry for this iteration
         dictionary[dictSize++] = w + entry[0];
      }
   
      // assign the temp entry as the new "old" word for the next iteration
      w = entry;
   }

   return result;
}

//
std::string int2BinaryString(int c, int cl) {
      std::string p = ""; //a binary code string with code length = cl
      int code = c;
      while (c>0) {         
		   if (c%2==0)
            p="0"+p;
         else
            p="1"+p;
         c=c>>1;   
      }
      int zeros = cl-p.size();
      if (zeros<0) {
         std::cout << "\nWarning: Overflow. code " << code <<" is too big to be coded by " << cl <<" bits!\n";
         p = p.substr(p.size()-cl);
      }
      else {
         for (int i=0; i<zeros; i++)  //pad 0s to left of the binary code if needed
            p = "0" + p;
      }
      return p;
}

//
int binaryString2Int(std::string p) {
   int code = 0;
   if (p.size()>0) {
      if (p.at(0)=='1') 
         code = 1;
      p = p.substr(1);
      while (p.size()>0) { 
         code = code << 1; 
		   if (p.at(0)=='1')
            code++;
         p = p.substr(1);
      }
   }
   return code;
}
