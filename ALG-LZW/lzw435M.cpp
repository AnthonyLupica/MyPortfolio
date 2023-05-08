/*
   lzw435M.cpp

   driver for the lzw compression algorithm Part 2
*/

#include "lzwAlgorithm435M.hpp"
#include <sstream> // std::stringstream
#include <unordered_set>

void compressionDriver(const std::string &filename);
void decompressionDriver(const std::string &filename);
void compressionWriteResult(const std::string &filename, const std::vector<int> &compressed);
std::string compressionReadResult(const std::string &filename);
bool isValidFileExtension(const std::string &filename, const std::string &extension);

const std::unordered_set<int> POWERS_OF_2 = { 512, 1024, 2048, 4096, 8192, 16384, 32768 };

int main(int argc, char* argv[]) 
{
   std::cout << " ________________________________________________________________ " << std::endl;
   std::cout << "|                                                                |" << std::endl;
   std::cout << "| 3460:435/535 Algorithms Project Two Part Two - LZW Compression |" << std::endl;
   std::cout << "|________________________________________________________________|" << std::endl << std::endl;

   // validate # of command-line args
   if (argc != 3)
   {
      std::cerr << "Error: invalid invocation" << std::endl;
      std::cerr << "Required Format: ./lzw435 <c/e> <filename>" << std::endl;
      
      return 1;
   }

   char option = *(argv[1]);
   std::string filename(argv[2]);
   switch (option)
   {
      case 'c':
      case 'C':
         std::cout << "Option Select: compress '" << filename << "'\n\n";
         compressionDriver(filename);
         break;
      case 'e':
      case 'E': 
         std::cout << "Option Select: expand '" << filename << "'\n\n";

         try 
         {
            decompressionDriver(filename);
         } catch(const char *a) {
             std::cout << a;
         }
         break;
      default:
         std::cerr << "Error: unrecognized option '" << option
                   << "'. Valid options are 'c' for compress, and 'e' for expand (decompression)" << std::endl;
         return 1;
   }

   return 0;
}

void compressionDriver(const std::string &filename)
{
   // validate file is a .txt
   if (!isValidFileExtension(filename, ".txt"))
   {
      std::cerr << "Error: unsupported file extension" << std::endl;
      std::cerr << "Compression can only be performed on a text file" << std::endl;

      return;
   }
 
   //open the input file
   std::ifstream inFile;
   inFile.open(filename); 

   if (!inFile)
   {
      std::cerr << "Error: unable to open file '" << filename << "'\n";
      std::cerr << "Please ensure it is located in the same directory as the executable" << std::endl;

      return;
   }

   // read the contents of the file and store in string
   std::stringstream sstream;
   sstream << inFile.rdbuf();
   std::string inputTxt = sstream.str(); 

   // pass string to compress and a back_insert_iterator that inserts elements at the end of container "compressed".
   // std::vector<int> compressed holds the sequence of codes produced by LZW compression 
   std::vector<int> compressed;

   compress(inputTxt, std::back_inserter(compressed));

   // Binary IO to write compression results
   compressionWriteResult(filename, compressed);

   return;
}

void decompressionDriver(const std::string &filename) 
{
   // validate file is a .lzw
   if (!isValidFileExtension(filename, ".lzw2"))
   {
      std::cerr << "Error: unsupported file extension" << std::endl;
      std::cerr << "Expansion can only be performed on a .lzw2 file" << std::endl;

      return;
   }

   // binary IO to read result of compression 
   std::string binaryString = compressionReadResult(filename);

   // convert to integer code sequence
         
   int bits = 9; // code word length begins at 9
   int codeCount = 256; // counter to keep track of how many bits to use for code words
   std::vector<int> codeSequence;
   
   // convert n bits at a time and push into vector 
   std::string chunk;
   int code;
   for (int i = 0; i < binaryString.size(); i += bits) 
   {
      if (POWERS_OF_2.count(codeCount))
      {
         --i;
      }

      chunk = binaryString.substr(i, bits);

      // we've reached the next power of 2, time to increment bits
      if (POWERS_OF_2.count(codeCount))
      {
         ++bits;
      }

      code = binaryString2Int(chunk);

      codeSequence.push_back(code);

      // increment codeCount
      ++codeCount;
   }

   // remove the final code if its a 0
   if (codeSequence.back() == 0) 
   {
      codeSequence.pop_back();
   }
   
   // decompress and write result
   std::string decompressed = decompress(codeSequence.begin(), codeSequence.end());

   // produce substring of filename with extension removed
   // assuming the file extension is ".lzw", we know we don't want the final 4 characters
   std::string extensionlessFileName = filename.substr(0, filename.length() - 4);
   
   // derive file name target
   std::string derivedFileToWrite = extensionlessFileName + "2M"; 

   std::ofstream outFile;
   outFile.open(derivedFileToWrite.c_str());
   outFile << decompressed;
   std::cout << "Results of decompression written -> " << derivedFileToWrite << "'\n";

   return;
}

void compressionWriteResult(const std::string &filename, const std::vector<int> &compressed) 
{
   // code word length begins at 9 
   int bits = 9;

   // counter to keep track of how many bits to use for code words
   int codeCount = 256;

   std::string bCode = "";
   std::string bCodeTemp = "";
   for (std::vector<int>::const_iterator itr = compressed.begin() ; itr != compressed.end(); ++itr) 
   {
      // convert current code to bits and append to running string of full byte code string
      bCodeTemp = int2BinaryString(*itr, bits);
      bCode += bCodeTemp;

      // we've reached the next power of 2, time to increment bits
      if (POWERS_OF_2.count(codeCount))
      {
         ++bits;
      }
      
      // increment codeCount
      ++codeCount;
   }

   std::string zeros = "00000000";
   // make sure the length of the binary string is a multiple of 8
   if (bCode.size() % 8 != 0) 
   {
      // if not, pad with 0s
      bCode += zeros.substr(0, 8 - bCode.size() % 8);
   }

   /* WRITING TO FILE */

   // produce substring of filename with extension removed
   // assuming the file extension is ".txt", we know we don't want the final 4 characters
   std::string extensionlessFileName = filename.substr(0, filename.length() - 4);
   
   // derive file name target
   std::string derivedFileToWrite = extensionlessFileName + ".lzw2"; 

   std::ofstream outFile;
   outFile.open(derivedFileToWrite.c_str(),  std::ios::binary);
   
   int byteValue; 
   for (int i = 0; i < bCode.size(); i += 8)
   { 
      byteValue= 1;
      for (int j = 0; j < 8; j++) 
      {
         byteValue = byteValue << 1;
         if (bCode.at(i+j) == '1')
         {
            byteValue += 1;
         }
      }
      char outputByte = (char) (byteValue & 255); // save the string byte by byte
      outFile.write(&outputByte, 1);  
   }

   std::cout << "Results of compression written -> " << derivedFileToWrite << "'\n";

   return;
}

std::string compressionReadResult(const std::string &filename) 
{
   std::ifstream inFile;
   inFile.open(filename.c_str(),  std::ios::binary); 

   if (!inFile)
   {
      std::cerr << "Error: unable to open file '" << filename << "'\n";
      std::cerr << "Please ensure it is located in the same directory as the executable" << std::endl;

      exit(1);
   }

   struct stat filestatus;
   stat(filename.c_str(), &filestatus );
   long fsize = filestatus.st_size; // get the size of the file in bytes

   char contents[fsize];
   inFile.read(contents, fsize);
   std::string zeros = "00000000";
   
   // empty string to hold the binary representation of the file contents
   std::string binaryString = "";
   
   // loop controls 
   long byteCount = 0;
   
   while(byteCount < fsize) 
   {
      // Convert the byte to an unsigned char
      unsigned char unsignedByte =  (unsigned char) contents[byteCount];

      // Convert the byte to a binary string
      std::string byteBinaryString = ""; //a binary string
      for (int j = 0; j < 8 && unsignedByte > 0; j++) 
      {         
		   if (unsignedByte % 2 == 0)
         {
            byteBinaryString= "0" + byteBinaryString;
         }
         else
         {
            byteBinaryString = "1" + byteBinaryString;
         }
         unsignedByte = unsignedByte >> 1;   
      }

      // Pad the binary string with zeros if necessary
      byteBinaryString = zeros.substr(0, 8-byteBinaryString.size()) + byteBinaryString; 

      // Append the binary string to the overall binary representation of the file
      binaryString += byteBinaryString; 

      byteCount++;
   } 

   return binaryString;
}

bool isValidFileExtension(const std::string &filename, const std::string &extension)
{
   // filename is shorter than or of the same length as the extension
   if (filename.length() <= extension.length())
   {
      return false;
   }

   // search for extension, starting at position where the extension begins
   if (filename.find(extension, filename.length() - extension.length()) == std::string::npos)
   {
      // the expected extension did not match
      return false;
   }

   return true;
}

