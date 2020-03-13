
#include <GameOfLife.h>


int main(int argc, char** argv) {

 //   SETUP
 string in_option,run_option, out_option, in_filename ;

 int in_row;  // rows
 int in_col;  // columns
 double density ;
 GameOfLife * G ;    // a pointer to the board so that it can be initialized in 2 different ways


 cout << "Would you like to provive a input file (y/n): ";
 cin >> in_option ;

 if (in_option[0] == 'y' ) {
   cout << "\nPlease provide a filename : " ;
   cin >> in_filename ;

   // fill the initial board
  G = new GameOfLife(in_filename);

 } else {
   cout << "\nPlease provide number of rows: "  ;
   cin >> in_row;
   cout << "\nPlease provide number of column: " ;
   cin >> in_col;
   cout << "\nPlease provide population density (between 0 and 1) : " ;
   cin >> density ;

   // fill the initial board
   G = new GameOfLife(in_row, in_col, density);
 }

 // Which mode to run in
 cout << "\nWhich mode do you want to run in Classic, Doughnut or Mirror (C/D/M) :" ;
 cin >> run_option ;

 //  Output options
 cout << "\nWould you to press Enter between simulations or print to File  (E/F) : " ;
 cin >> out_option ;

 // Run the SIMULATION in user given mode
 int ret  = G->runSimulation (run_option, out_option );

 //let user know results of simulation
 if(ret == 1) {
     cout << "The simulation has ended because there are no living cells remaining. " << endl;
 } else if(ret == 2) {
     cout << "The simulation has ended because the cell population has stabilized. " << endl;
 }


    return 0;
}
