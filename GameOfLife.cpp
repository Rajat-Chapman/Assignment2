
#include <GameOfLife.h>

// Create a board from random generation
GameOfLife::GameOfLife(int rows, int cols, double density) {

     // allocate board
     orig_board = new int*[rows];
     for (int cnt=0; cnt<rows; cnt++)
         orig_board[cnt] = new int[cols];

     num_rows = rows ;
     num_cols = cols ;

      //fill board with random pieces
      double rand_temp;
      for(int i = 0; i < rows; ++i) {
          for(int j = 0; j < cols; ++j) {
              rand_temp = ((double) rand() / (RAND_MAX));
              if(rand_temp < density) {
                  orig_board[i][j] = 1;
              } else {
                  orig_board[i][j] = 0;
              }
          }
      }
}

// Create board from file input
GameOfLife::GameOfLife(string filename) {

   // open the file. Read the first 2 lines for size and rest of board
   int rows, cols ;
   string line ;

  ifstream infile (filename);
  if (infile.is_open())
  {
    infile >> rows ;
    infile >> cols ;
    cout <<  "Rows and Cols " << rows << " " << cols << "\n" ;

    // allocate board
    orig_board = new int*[rows];
    for (int cnt=0; cnt<rows; cnt++)
        orig_board[cnt] = new int[cols];

    num_rows = rows ;
    num_cols = cols ;
    getline (infile,line) ;  // finish reading the newline after the number

    for ( int i=0; i < rows ; i++)
    {
      getline (infile,line) ;
      for(int j=0; j < cols ; j++) {
        if (line[j] == '-' )    // this is not live cell
          orig_board[i][j] = 0 ;
        else
          orig_board[i][j] = 1 ;    // assume it is an X
      }
    }

    infile.close();

    // testing the input
    PrintBoard (orig_board, 0, "P") ;
  }
  else {
    cout << "Unable to open file";
  }

}


// find the neighbors
int GameOfLife::neighborsClassic(int ** board, int curr_row, int cur_col) {
    int total = 0;
    for(int i = -1; i < 2; ++i) {
        for(int j = -1; j < 2; ++j) {
            int posY = curr_row + i;
            int posX = cur_col + j;

            if(posY >= num_rows || posY == -1 || posX >= num_cols || posX == -1) {
                //check if outside of board
            } else if(posY == curr_row && posX == cur_col) {
                //check if sqaure is itself
            } else {
                total += board[posY][posX];
            }
        }
    }
    return total;
}


int GameOfLife::neighborsDoughnut(int **board, int curr_row, int curr_col) {
    int total = 0;
    for(int i = -1; i < 2; ++i) {
        for(int j = -1; j < 2; ++j) {
            int posY = curr_row + i;
            int posX = curr_col + j;

            //adjust position to wraparound board
            if(posY == -1) {
                posY = num_rows - 1;
            }
            if(posX == -1) {
                posX = num_cols - 1;
            }
            if(posY == num_rows) {
                posY = 0;
             }
            if(posX == num_cols) {
                posX = 0;
            }

            if(posY == curr_row && posX == curr_col) {
                //check if sqaure is itself
            } else {
                total += board[posY][posX];
            }
        }
    }
    return total;
}


int GameOfLife::neighborsMirror(int **board, int curr_row, int curr_col) {
  int total = 0;
  for(int i = -1; i < 2; ++i) {
      for(int j = -1; j < 2; ++j) {
          int posY = curr_row + i;
          int posX = curr_col + j;

          //adjust position to wraparound board
          if(posY == -1) {
              posY = curr_row;
          }
          else if(posY == num_rows) {
              posY = posY-1;
           }

          if(posX == -1) {
              posX = curr_col;
          }
          else if(posX == num_cols) {
              posX = posX-1;
          }

          // if(posY == curr_row && posX == curr_col) {
              //check if sqaure is itself
          if ( i==0 && j == 0)   {
              // since the same square needs to be counted in mirror except in its own position
          } else {
              total += board[posY][posX];
              // cout << "Total:" << "  " << total << "  " << i << "  " << j << "\n" ;
          }
      }
  }
  return total;

}

// Main simlator method that runs in a loop till the board stabilizes or is empty
int GameOfLife::runSimulation (string run_option, string out_option)
{

  int neis2;
  int neis3;

  // Allocate a new board so that the original board does not get affected by simulation
  int ** current_board = new int*[num_rows];
  for (int cnt=0; cnt<num_cols; cnt++)
      current_board[cnt] = new int[num_cols];

  // copy the board to start simulation
  for(int i = 0; i < num_rows; ++i)
    for(int j = 0; j < num_cols; ++j)
       current_board[i][j] = orig_board[i][j];

  // board for holding next board
  int ** hold = new int*[num_rows];
  for (int cnt=0; cnt<num_cols; cnt++)
      hold[cnt] = new int[num_cols];

  int genCount = 0 ;

  while (genCount < 1000000 ) {
      //this is a safety catch because there are certain conditions where the
      //previous board is not equal to the last, but the board
      //will continuously loop in a repeating pattern

    // print the current board starting with generation 0
    PrintBoard (current_board, genCount, out_option) ;


    // Start the simulation
    for(int i = 0; i < num_rows; ++i) {
        for(int j = 0; j < num_cols; ++j) {

            if (run_option[0] == 'C' || run_option[0] == 'c' )
              neis3 = neighborsClassic(current_board, i, j);
            else if (run_option[0] == 'D' || run_option[0] == 'd')
              neis3 = neighborsDoughnut(current_board,i, j);
            else
              neis3 = neighborsMirror(current_board, i, j);

            if(neis3 < 2) {             //lonely, cell dies
                hold[i][j] = 0;

            } else if(neis3 == 2) {     //happy, stable condition
                hold[i][j] = current_board[i][j];

            } else if(neis3 == 3) {     //cell growth or stable
                hold[i][j] = 1;

            } else {                    //overcrowding, cell dies
                hold[i][j] = 0;
            }
        }
    }

  genCount++ ;   // this is only to break the loop

   // check if the loop should be stopped
   if (isEmpty(hold)) {
     PrintBoard (hold, genCount, out_option);
     return 1 ;
   }

   if (isEqual(hold,current_board)) {
     PrintBoard (hold, genCount, out_option);
     return 2 ;
   }
    // Copy the new board as the current board for the simulation
    for(int k = 0; k < num_rows; ++k)
      for(int l = 0; l < num_cols; ++l)
         current_board[k][l] = hold[k][l];

  }  // end of main loop

  return 0 ;

}


// Print a given board
void GameOfLife::PrintBoard (int**board, int genCount, string out_option) {

  ofstream outfile;


  if (out_option[0] == 'F' || out_option[0] == 'f') {
    outfile.open ("Assign2_Output.txt",  ios::app );
    outfile << genCount << "\n" ;
    for(int i = 0; i < num_rows; ++i) {
        for(int j = 0; j < num_cols; ++j) {
            if (board[i][j] == 1)
              outfile << "X" ;
            else
               outfile << "-"  ;

        }
        outfile << endl;
    }
    outfile.close() ;
  }
  else if (out_option[0] == 'E' || out_option[0] == 'e') {
    cout << "Press any key to continue ...." ;
    cout << endl;
    cin.get() ;     // read any character from input
    cout <<  genCount << "\n" ;
    for(int i = 0; i < num_rows; ++i) {
        for(int j = 0; j < num_cols; ++j) {
          if (board[i][j] == 1)
            cout << "X" ;
          else
             cout << "-"  ;
        }
        cout << endl;
    }
  }
  else {
    cout << genCount << "\n" ;
    for(int i = 0; i < num_rows; ++i) {
        for(int j = 0; j < num_cols; ++j) {
          if (board[i][j] == 1)
            cout << "X" ;
          else
             cout << "-"  ;
        }
        cout << endl;
    }
  }

}

//returns true if the board is empty
bool GameOfLife::isEmpty(int ** test_board) {

    for(int i = 0; i < num_rows; ++i) {
        for(int j = 0; j < num_cols; ++j) {
            if(test_board[i][j] != 0) {
                return false;
            }
        }
    }
    return true;
}


// compare two given boards
bool GameOfLife::isEqual(int **old_board, int **new_board) {

    for(int i = 0; i < num_rows; ++i) {
        for(int j = 0; j < num_cols; ++j) {
            if(old_board[i][j] != new_board[i][j]) {
                return false;
            }
        }
    }
    return true;
}
