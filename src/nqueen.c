
#include "hardware.h"

#define N 4 

/* A utility function to check if a queen can 
   be placed on board[row][col]. Note that this 
   function is called when "col" queens are 
   already placed in columns from 0 to col -1. 
   So we need to check only left side for 
   attacking queens */
unsigned char isSafe(int board[N][N], int row, int col) 
{ 
    int i, j; 
  
    /* Check this row on left side */
    for (i = 0; i < col; i++) 
        if (board[row][i]) 
            return 0; 
  
    /* Check upper diagonal on left side */
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--) 
        if (board[i][j]) 
            return 0; 
  
    /* Check lower diagonal on left side */
    for (i = row, j = col; j >= 0 && i < N; i++, j--) 
        if (board[i][j]) 
            return 0; 
  
    return 1; 
} 
  
/* A recursive utility function to solve N 
   Queen problem */
unsigned char solveNQUtil(int board[N][N], int col) 
{ 
    /* base case: If all queens are placed 
      then return 1 */
    if (col >= N) 
        return 1; 
  
    /* Consider this column and try placing 
       this queen in all rows one by one */
    for (int i = 0; i < N; i++) { 
        /* Check if the queen can be placed on 
          board[i][col] */
        if (isSafe(board, i, col)) { 
            /* Place this queen in board[i][col] */
            board[i][col] = 1; 
  
            /* recur to place rest of the queens */
            if (solveNQUtil(board, col + 1)) 
                return 1; 
  
            /* If placing queen in board[i][col] 
               doesn't lead to a solution, then 
               remove queen from board[i][col] */
            board[i][col] = 0; // BACKTRACK 
        } 
    } 
  
    /* If the queen cannot be placed in any row in 
        this colum col  then return 0 */
    return 0; 
} 
  
/* This function solves the N Queen problem using 
   Backtracking. It mainly uses solveNQUtil() to  
   solve the problem. It returns 0 if queens 
   cannot be placed, otherwise, return 1 and 
   prints placement of queens in the form of 1s. 
   Please note that there may be more than one 
   solutions, this function prints one  of the 
   feasible solutions.*/
unsigned char solveNQ() 
{ 
    int board[N][N] = { { 0, 0, 0, 0 }, 
                        { 0, 0, 0, 0 }, 
                        { 0, 0, 0, 0 }, 
                        { 0, 0, 0, 0 } }; 
  
    if (solveNQUtil(board, 0) == 0) { 
        SEG = 0xdead;
        return 0; 
    } 
  
    SEG = board;
    return 1; 
} 
  
// driver program to test above function 
int main() 
{ 
    solveNQ(); 
    return 0; 
} 

