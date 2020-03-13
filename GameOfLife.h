
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;

class GameOfLife {

public :

// Constructors
GameOfLife(string filename) ;    // PopulateBoardFromFile constructor
GameOfLife(int rows, int cols, double density) ;   // PopulateBoardDensity constructor

int neighborsClassic( int ** board, int curr_row, int cur_col);     // calculate num of neighbors
int neighborsDoughnut( int ** board, int curr_row, int curr_col);   // calculate num of neighbors
int neighborsMirror( int **board, int curr_row, int curr_col);      // calculate num of neighbors

// Simulator
int runSimulation (string run_option, string out_option) ;

// Helper functions
bool isEmpty(int **board) ;
bool isEqual(int **board1, int **board2) ;
void PrintBoard (int **board, int genCount, string out_option);


int ** orig_board ;
int num_rows ;
int num_cols ;

private :


} ;
