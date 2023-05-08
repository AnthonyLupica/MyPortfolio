/***
   prepared for CS435 Project 1 part 2
**/

#include <string.h>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

std::string hexString_to_decimalString(const std::string &hex_string);

int main(int argc, char *argv[])
{
   // validate that we have 3 or 4 args, and the first arg is either 's' or 'v'
   if (argc != 3 && argc != 4 || (argv[1][0] != 's' && argv[1][0] != 'v')) 
   {
      std::cerr << "wrong format! should be ./messageDigest435 [s] [file_name] to sign a document, "
                << "or ./messageDigest435 [v] [file_name] [signature_file] to verify a document\n"
                << "where both file_name arguments are the same file (modified or unmodified) pre and post signing, and signature_file is a file containing its signature\n";
      exit(1);
   }
   
   // file name to either sign or verify
   std::string filename = argv[2];
   
   // read in the file
   std::streampos begin,end;
   std::ifstream myfile (filename.c_str(), std::ios::binary);

   // verify connection to file is good
   if (!myfile)
   {
      std::cerr << "\nError connecting to file... check that the \"file to sign\" argument is correct and ensure it is present at the same directory level as messageDigest435.cpp\n";
      exit(1);
   }

   // find size of the file in bytes
   begin = myfile.tellg();
   myfile.seekg (0, std::ios::end);
   end = myfile.tellg();
   std::streampos size = end-begin; 
   
   // read file; save in the char array memblock
   myfile.seekg (0, std::ios::beg);
   char * memblock = new char[size];
   myfile.read (memblock, size); 
   myfile.close();
   
   // display one of two dialogs depending on whether 's' or 'v' was specified
   if (argv[1][0]=='s')
   {
      std::cout << "file to sign >>\n\n```\n" << memblock << "\n```\n";
   }
   else
   {
      std::cout << "file to verify >>\n\n```\n" << memblock << "\n```\n";
   }
   
   std::cout << "file name: \"" << filename.c_str() << "\"\n" ;
   std::cout << "size of the file: " << size << " bytes.\n\n";

   // generate a SHA-256 hash of the content of the file to be signed and send to 
   // hexString_to_decimalString() to be converted to a decimal string
   std::string decimal_string = hexString_to_decimalString(sha256(memblock));

   // convert decimal_string to a BigInteger to form a hash_code
   BigInteger hash_code = stringToBigInteger(decimal_string);
   
   // [D][O][C][U][M][E][N][T] [S][I][G][N][I][N][G] 

   std::ifstream public_private_in; // filestream object for reading public/private key 
   BigInteger n = BigInteger(0);    // BigInteger to store n from public and private keys 
   std::string temp_value1;         // temp variables for reading in values as strings 
   std::string temp_value2;
   std::string temp_value3;

   if (argv[1][0]=='s') 
   {
      if (argc != 3)
      {
         std::cerr << "wrong format! please specify a file to sign using ./messageDigest435 [s] [file_name]\n";
         exit(1);
      }

      // file stream for the private key 
      public_private_in.open("../bigInt435/d_n.txt");

      // verify a good connection to 'd_n.txt'
      if (!public_private_in)
      {
         std::cerr << "Error connecting to file containing private key for document signing...\n"
                   << "Make sure it is reachable through \"../bigInt435/d_n.txt\"\n";
         exit(1);
      }

      BigInteger d = BigInteger(0);
      
      // read d into value 1 and n into value 2
      public_private_in >> temp_value1 >> temp_value2;

      // convert strings to BigIntegers
      d = stringToBigInteger(temp_value1);
      n = stringToBigInteger(temp_value2);

      // sign the hash code 
      BigInteger signature = modexp(hash_code, d.getMagnitude(), n.getMagnitude());

      // write the signature to filename + ".signature"
      std::ofstream signature_out(filename + ".signature");
      signature_out << signature << std::endl;

      std::cout << "signature of " << filename << " written to \"" << filename + ".signature" << "\"\n";
      std::cout << "to perform a verification of this file, execute the command ./messageDisgest435 v " << filename << " " << filename + ".signature\n";

      signature_out.close();
      public_private_in.close();
   }

   // [D][O][C][U][M][E][N][T] [V][E][R][I][F][I][C][A][T][I][O][N]
   else 
   {
      if (argc != 4)
      {
         std::cerr << "wrong format! given a file \"file_name\" has been signed with ./messageDigest435 [s] [file_name]\n"
                   << "./messageDigest435 [v] [file_name.signed] [signature_file] is used to verify that document, "
                   << "where validity is determined by whether it has been modified since its signature was produced\n"
                   << "ensure all needed arguments are provided\n";
         exit(1);
      } 

      // file stream for the public key 
      public_private_in.open("../bigInt435/e_n.txt");

      if (!public_private_in)
      {
         std::cerr << "Error connecting to file containing public key for document verification...\n"
                   << "Make sure it is reachable through \"../bigInt435/e_n.txt\"\n";
         exit(1);
      }

      // file stream for the signature of filename
      std::string signature_file = argv[3];
      std::ifstream signature_in(signature_file); // try to open the signature file using the provided name

      if (!signature_in)
      {
         // clear error flag
         signature_in.clear();

         std::cerr << "unable to open provided signature file \"" << signature_file << "\", attempting to derive and open "
                   << "anticipated file name \"" << filename + ".signature" << "\"...\n\n";

         // try again by attempting to derive the expected file name 
         signature_in.open(filename + ".signature");

         // now check again if the file stream is connected
         if (!signature_in)
         {
            // if this didn't work, issue an error, and end execution 
            std::cerr << "error connecting to file containing signature of " << "\"" << filename << "\", presumed to be " << "\"" << filename + ".signature" << "\"...\n"
                      << "please recheck names, and ensure it exists in the current directory\n";
            exit(1);
         }
      }

      BigInteger e = BigInteger(0);
      BigInteger signature = BigInteger(0);

      // read e into value 1 and n into value 2
      public_private_in >> temp_value1 >> temp_value2;

      // read the signature into value 3
      signature_in >> temp_value3;

      // convert strings to BigIntegers
      e = stringToBigInteger(temp_value1);
      n = stringToBigInteger(temp_value2);
      signature = stringToBigInteger(temp_value3);

      // verify the signature on the hash code by using the public key on the signature (should return the original hash code)
      BigInteger hash_code_verify = modexp(signature, e.getMagnitude(), n.getMagnitude());

      // finally, display to the user whether the document is authentic or modified by comparing the original hash code with the ecrypted signature 
      if (hash_code == hash_code_verify)
      {
         std::cout << "the document received, \"" << filename << "\" has been determined to be authentic\n";
      }
      else 
      {
         std::cout << "the document received, \"" << filename << "\" has been determined to be modified\n";
      }

      signature_in.close();
      public_private_in.close();
   }
   delete[] memblock;
    
   return 0;
}

// pre:  A hexidecimal string 
// post: A decimal string representation of the hex string  
std::string hexString_to_decimalString(const std::string &hex_string)
{
   std::string output = "";

   // take chunks of four characters from hex_string at a time
   for (size_t i = 0; i < hex_string.length(); i += 4) 
   {
      // store the chunk and convert the chunk to an integer using stoi and the base 16 specifier
      std::string hex_chunk = hex_string.substr(i, 4);
      int decimal_chunk = std::stoi(hex_chunk, nullptr, 16);

      // then convert the decimal_chunk to a string to form a decimal representation as a string 
      output += std::to_string(decimal_chunk);
   }
   
   return output;
}
