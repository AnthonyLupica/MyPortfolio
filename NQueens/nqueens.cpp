/* 
    nqueens.cpp

    author: Anthony Lupica <ar127@uakron.edu>
    purpose: solve the NQueens problem using the backtracking algorithm for some .csv input 
             representing a chessboard of size N x N. If a solution is found for N queens, 
             a .csv solution is produced. Otherwise, no solution is displayed.
    
    Information: This program was built and tested in an Ubuntu shell with CMake. 
                 It contains very little validation for garbage input.

                 Assumptions made:

                 * The name of the input file is "input.csv" and is present in the build 
                   directory.
                 * Does not validate against the occurance of non-square input,
                   or input that does not adhere to strictly comma-separated 0's and 1's.
                   If these inputs are made, infinite recursion may arise.
                 * Exactly one initial queen (represented by a 1) must be present in some starting position as a given condition.
                   Future versions may add functionality for the option to omit an initial queen, or provide more than one.
    
    Implementation:

                 This implementation uses four std::unordered_sets to track row/column and positive/negative 
                 diagonal information for each queen. These sets are used for constant-time lookup
                 for the validity of placing a queen. 
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>        // isdigit() and isprint()
#include <unordered_set> // for sets of rows/columns, and positive/negative diagonals 

using std::ifstream;
using std::ofstream;
using std::cout;
using std::cin;
using std::vector;

void readFile(std::ifstream &inStream, int N); 
int findN();
void display();
void display(std::ofstream &outStream); // overloaded display() for file IO
int NQueens(int row);

// namepace containing sets for queen collisions
// each queen must occupy its own row/column, and positive/negative diagonal
namespace QueenPos
{
    std::unordered_set<int> rowSet;     // store row index
    std::unordered_set<int> colSet;     // store column index 
    std::unordered_set<int> posDiagSet; // Row + Col = constant for a given positive diagonal
    std::unordered_set<int> negDiagSet; // Row - Col = constant for a given negative diagonal
    
    // starting queen position (initQueen[0] = row && initQueen[1] = column)
    int initQueen[2];
}

// define a 2d vector (vector of vectors of chars)
vector<vector<char>> chessBoard;

int main()
{
    // declare and initialize in-file stream
    ifstream inStream("input.csv");

    // if file was successfully opened
    if (inStream)
    {
        // find what N is for the .csv input (assumes a square layout)
        int N = findN();
        
        // pass in the stream object and 2d vector by ref,
        // and the dimensions of the board
        readFile(inStream, N);

        // close the file stream
        inStream.close();

        // echo the chessboard structure to standard output
        cout << "\ninput file -->\n\n";
        display();

        // submit instantiated chessBoard to the backtracking algorithm
        // if returned 1, output the solution board
        if (NQueens(0) == 1)
        {
            // declare and initialize a file to hold the solution board
            ofstream outStream("solution.csv");
            
            // if file opened, stream the solution to the file 
            if (outStream)
            {
                cout << "Solution found for the given " << N << " by " << N << " chess board, with an initial queen at (" 
                     << QueenPos::initQueen[0] << ", " << QueenPos::initQueen[1] << ") --> 'solution.csv'\n\n";
            
                display(outStream);

                // close the file 
                outStream.close();
            }
            
            // backup plan if file IO fails (stream to the console)
            else
            {
                cout << "Solution found for the given " << N << " by " << N << " chess board, with an initial queen at (" 
                     << QueenPos::initQueen[0] << ", " << QueenPos::initQueen[1] << ") -->\n\n";
            
                display();
            }
        }

        // otherwise, no solution could be found for N with the given initial queen position provided 
        else 
        {
            cout << "No solution found for the given " << N << " by " << N << " chess board, with an initial queen at (" 
                 << QueenPos::initQueen[0] << ", " << QueenPos::initQueen[1] << ")\n\n";
        } 
    }

    else 
    {
        std::cerr << "Input file could not be opened\n";
    }

    return 0;
}

/*  
    pre: assumes the existence of a file titled "input.csv" for input,
         and expects that it will be formatted in an N x N layout.
         It takes in an ifstream by reference and a vector of vectors of chars by reference.
         It also takes in N (returned by findN()), and uses it as the for-condition.
    
    post: Nested for-loops populate the vector from the stream. The inner-for fills the inner 
          vectors, which are then pushed to the outer vector. The initial queen's position is saved.
          Additionally, the QueenPos:: sets for row pos, column pos, 
          negative diagonal, and positive diagonal are updated for the initial queen position.
*/
void readFile(std::ifstream &inStream, int N)
{
    using namespace QueenPos;

    char getChar;
    
    // temp vector to store inner vectors 
    vector<char> temp;

    // fill the inner vectors, then use them to fill the outer vector 
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            // use loop to parse through commas
            do 
            {
                inStream.get(getChar);
            } while (getChar != '0' && getChar != '1');
            
            // intitial queen added to sets
            if (getChar == '1')
            {
                // store initial queen pos for later display
                initQueen[0] = i;
                initQueen[1] = j;
                
                // store row and col position
                rowSet.insert(i);
                colSet.insert(j);

                // store pos diag 
                posDiagSet.insert(i + j);

                // store neg diag
                negDiagSet.insert(i - j);
            }
            temp.push_back(getChar);
        }
        chessBoard.push_back(temp);
        
        // clear the temp vector 
        temp.clear();
    }

    return;
}

/* 
    pre: Assumes the existence of a file titled "input.csv" for input,
         and expects that it will be formatted in an N x N layout.
         If the values are not arranged in a square, the file will 
         be truncated to the length of the first row.
    
    post: The dimensions of the file are returned.
*/
int findN()
{
    // declare and initialize temp file stream
    ifstream temp("input.csv");

    // char to hold input and N to hold width
    char getChar; 
    int N = 0;
    
    // loop reads number of 1's or 0's in first line to find N (assumes square)
    do
    {
        getChar = temp.get();

        // increment N if char is a digit (1 or 0)
        if (isdigit(getChar))
        {
            ++N;
        }
    } while (isprint(getChar)); // end if char is not printable

    temp.close();

    return N;
 }
 
/* 
    pre: Assumes a globally declared 2D vector named "chessBoard" exists.

    post: The 2d vector is output to the screen.
*/
void display()
{
    // chessBoard[i].size() checks the size of the vector rows
    for (int i = 0; i < chessBoard.size(); ++i)
    {
        for (int j = 0; j < chessBoard[i].size(); ++j)
        {
            cout << chessBoard[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n";

    return;
}

/* 
    pre: Overloaded for output file streaming. Assumes a globally declared 2D vector named "chessBoard" exists.
    
    post: The 2d vector is output to the file, in .csv format.
*/
void display(std::ofstream &outStream)
{
    // chessBoard[i].size() checks the size of the vector rows
    for (int i = 0; i < chessBoard.size(); ++i)
    {
        for (int j = 0; j < chessBoard[i].size(); ++j)
        {
            outStream << chessBoard[i][j];

            // .csv formatting
            if (j != chessBoard[i].size() - 1)
            {
                outStream << ",";
            }
        }
        outStream << "\n";
    }

    return;
}

/* 
    pre: Nqueens() is a recursive backtracking algorithm that uses rows as the recursion parameter 
         (incrementing the row for each call). Row begins at 0, and increments until N (The dimensions of the board).
         This implementation requires a number of things to be true, namely a 2D vector declared in QueenPos::, which consists 
         entirely of 0's (not queens) and a single 1 (representing a *mandatory* given initial queen). These values must also be
         arranged in a square, analagous to a chess board. 
    
    post: The recursive solution expands out the frontier[#] in depth-first fashion. If a point is reached where a queen cannot be 
          placed safely, each stack frame is "backtracked" until a valid queen can be placed with which to proceed.
          The solution, if possible for the input, will be indicated by returning a 1 to the original caller. Otherwise, 0 is returned. 
        
          #The set of actions that are possible from a given state.
*/     
int NQueens(int row)
{
    // qualify names for queen position variables
    using namespace QueenPos;

    // base case: if all queens have been placed, return true
    if (row == chessBoard.size())
    {
        return 1;
    }

    // for loop for iterating columns.
    // note: when a queen is backtracked, we return and resume in that frame at the increment of col.
    for (int col = 0; col < chessBoard[row].size(); ++col)
    {
        // first check that we have not iterated into the initial queen's box. 
        // Upon this, we immediately perform a recursive call to advance the row.
        // We disallow the initial queen from being overwritten, so we shield it from 
        // being backtracked (it will never enter the if-block for placing or backtracking queens).
        if (row == initQueen[0] && col == initQueen[1])
        {
            // recursive call
            if (NQueens(row + 1))
            {
                return 1;
            }
        }

        // place a queen if it will not be attacked by another (no row/column, or pos/neg diagonal conflict)
        if ((rowSet.count(row) == 0) && (colSet.count(col) == 0) && (negDiagSet.count(row - col) == 0) && (posDiagSet.count(row + col) == 0))
        {
            // note: character '1' is 49 in dec
            chessBoard[row][col] = 49;
            
            // update sets
            rowSet.insert(row);
            colSet.insert(col);
            negDiagSet.insert(row - col);
            posDiagSet.insert(row + col);

            // recursive call (will continue until NQueens(row + 1) returns false)
            if (NQueens(row + 1))
            {
                return 1;
            }

            // If this path did not resolve in a complete solution, backtrack the previous queen...
            
            // note: character '0' is 48 in dec
            chessBoard[row][col] = 48;

            // remove previous update to sets
            rowSet.erase(row);
            colSet.erase(col);
            negDiagSet.erase(row - col);
            posDiagSet.erase(row + col);
        }
        // following a backtrack, attempt with next iteration of col. If there is not a next iteration, no solution.
    }

    // if this is reached, we failed to place a queen in a row. Backtracking is necessary.
    return 0;
}
