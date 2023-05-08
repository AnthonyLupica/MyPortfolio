# ALG-RSA
Spring 2023 Algorithms - RSA (Project 1)

Project description [here](https://www.cs.uakron.edu/~duan/classes/435/projects/project1/project1.htm)

For this project, the only files that have been edited by me are 'rsa435.cc' and 'messageDigest435.cpp'.
I assume that the bigInt435 package containing my rsa435.cc, and the messageDigest435 package containing my messageDigest435.cpp
are adjacent directories, although I will not submit these packages alongside my files.

execution:

part 1 :: rsa 
```
make all

public and private keys     ---> ./rsa435
```          
- private key written to 'd_n.txt', and public key written to 'e_n.txt', both within the bigInt435 directory

part 2 :: digital signatures
```
make all

to sign a document          ---> ./messageDigest435 [s] [file_name]

to verify a signed document ---> ./messageDigest435 [v] [file_name] [signature_file]
```           
- assumption for s and v is that the private/public key can be found one adjecent directory over in bigInt435 and are appropriately named
- similarly, it is assumed that the document.txt to be signed is within the messageDigest435 directory containing messageDigest435.cpp.
  The signature document will also be ouptut here
            
           
           
