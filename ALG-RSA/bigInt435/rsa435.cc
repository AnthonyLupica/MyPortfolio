// Standard libraries
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include <time.h>

// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"
#include "BigIntegerAlgorithms.hh"

bool primality(const BigInteger &n);

// number of times looped to produce p and q
const int LOOP_COUNT = 200;

int main()
{
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this.
	 * Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */

   // seed rand
   srand(time(0));

   // booleans for the two required primality tests
   bool check1 = false;
   bool check2 = false;

	try 
   {  
      // [F][I][N][D][I][N][G] [P]
      std::cout << "finding p...\n";
      BigInteger p = BigInteger(0);
      int primeCandidate = 0;
     
      do
      {
         // reset checks to false
         check1 = false;
         check2 = false;

         // assign p back to 0
         p = 0;

         // in a loop, left shift by multiplying by ten, and concatenate a random digit [0, 9]
         for (int i = 0; i < LOOP_COUNT; ++i) 
         {
            p = (p * 10) + (rand() % 10);
         }
         p = (p * 10) + 7; // concatenating a 7 to increase odds that this number is prime

         // display to the console every candidate number that is a multiple of 10
         ++primeCandidate;
         if (primeCandidate % 10 == 0)
         {
            std::cout << "p candidate number " << primeCandidate << std::endl;
         }

         // send this candidate off to be tested for primeness (twice to be extra sure)
         check1 = primality(p);
         
         if(check1)
         {
            check2 = primality(p);
         }
      } while (!check1 || !check2); // while either call to primality returns false
      
      std::cout << "success on candidate " << primeCandidate << "!!\n\n";

      // [F][I][N][D][I][N][G] [Q]
      std::cout << "finding q...\n";
      BigInteger q = BigInteger(0);
      primeCandidate = 0;
     
      do
      {
         // reset checks to false
         check1 = false;
         check2 = false;

         // assign q back to 0
         q = 0;

         // in a loop, left shift by multiplying by ten, and concatenate a random digit [0, 9]
         for (int i = 0; i < LOOP_COUNT; ++i) 
         {
            q = (q * 10) + (rand() % 10);
         }
         q = (q * 10) + 7; // concatenating a 7 to increase odds that this number is prime

         // display to the console every candidate number that is a multiple of 10
         ++primeCandidate;
         if (primeCandidate % 10 == 0)
         {
            std::cout << "q candidate number " << primeCandidate << std::endl;
         }

         // send this candidate off to be tested for primeness (twice to be extra sure)
         check1 = primality(q);

         if(check1)
         {
            check2 = primality(q);
         }
      } while (!check1 || !check2); // while either call to primality returns false
      
      std::cout << "success on candidate " << primeCandidate << "!!\n\n";
      
      // [S][T][R][E][A][M][I][N][G] [P] [A][N][D] [Q]
      std::ofstream out_p_q("p_q.txt");
      
      if (!out_p_q)
      {
         std::cerr << "Error connecting to file for streaming of p and q, displaying to console instead\n";

         std::cout << "p: " << p << "\nq: " << q << std::endl;
      }

      else 
      {
         std::cout << "results for p and q written to \"p_q.txt\"\n";
         
         out_p_q << p << std::endl << q << std::endl;
      }

      // [N], [P][H][I], [E], [A][N][D] [D]

      BigInteger n = p * q;                // compute n
      BigInteger phi = (p - 1) * (q - 1);  // compute phi

      // choose an initial candidate for e that is relatively prime to (p - 1)(q - 1)
      BigInteger e = BigInteger(1);

      BigUnsigned GCD;
      do 
      {
         e += 2;
         GCD = gcd(phi.getMagnitude(), e.getMagnitude());   
      } while (GCD != 1); // outside this do-while, an e has been found which is relatively prime to phi

      // assign d a value using e
      BigInteger d(modinv(e, phi.getMagnitude()));

      /* check that e and d are modular inverses */
      BigInteger base = BigInteger(1);                           // a base value 
      base = modexp(1, e.getMagnitude(), phi.getMagnitude());    // encrypt with e
      base = modexp(base, d.getMagnitude(), phi.getMagnitude()); // decript with d
      
      // base should return to its initial state
      if (base != 1)
      {
         std::cerr << "e and d broke a test that concluded they aren't modular inverses\n";

         exit(1);
      }

      // [S][T][R][E][A][M][I][N][G] [K][E][Y][S]

      // connect to files for streaming keys
      std::ofstream outPubKey("e_n.txt");
      std::ofstream outPrivKey("d_n.txt");
      
      // if either stream fails, we opt to display both the console
      if (!outPubKey || !outPrivKey)
      {
         std::cerr << "\nError connecting to file for streaming of keys, displaying to console instead\n";

         std::cout << "public key -\n" << "e: " << e << "\nn: " << n << std::endl;
         std::cout << "\nprivate key -\n" << "d: " << d << "\nn: " << n << std::endl;

         return 0;
      }

      // else, stream to files 'e_n.txt' and 'd_n.txt'
      outPubKey << e << std::endl 
                << n << std::endl;
      outPubKey.close();
   
      outPrivKey << d << std::endl 
                 << n << std::endl;
      outPrivKey.close();

      std::cout << "public and private key written to \"e_n.txt\" and \"d_n.txt\" respectively\n";
	} 
   
   catch(char const* err) 
   {  
		std::cout << "The library threw an exception:\n"
			       << err << std::endl;
	}

	return 0;
}

// Input: Positive integer n
// Output: is prime? (yes/no)
bool primality(const BigInteger &n)
{
   // Fermat's Little Theorem states: 
   // "if n is prime, then for all a, such that 1 <= a < n, a^(n-1) == 1 mod n"

   /* generating a random integer for a */

   // LOOP_COUNT_RAND is in [1, LOOP_COUNT]
   // this ensures a is never greater than n (p and q) is concatenated one beyond its initalization loop
   const int LOOP_COUNT_RAND = rand() % (LOOP_COUNT) + 1;
   
   // initialization loop for a 
   BigInteger a = BigInteger(0);
   for (int i = 0; i < LOOP_COUNT_RAND; ++i) 
   {
      a = (a * 10) + (rand() % 10);
   } 

   // in the unlikely, but possible event that a resolved to 0, we assign 1
   if (a < 1)
   {
      a = 1;
   }

   // if a is found to be equal to, or greater than n, assign n - 1
   if (a >= n)
   {
      a = n - 1;
   }

   // with this valid number for a, run test for a^(n-1) == 1 mod n
   if (modexp(a, (n - 1).getMagnitude(), n.getMagnitude()) == 1)
   {
      // passes FLT, is *very* likely prime
      return true;
   }
   else
   {
      // not prime 
      return false;
   }
}
