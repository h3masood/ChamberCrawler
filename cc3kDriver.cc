#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <unistd.h> // getpid()
#include "cell.h"
#include "game.h"
#include "PRNG.h"
using namespace std;


PRNG prng( getpid() ); // random number generator; initialized with process ID(ensures continously variable behaviour)

int main( int argc, char *argv[] )
{
   string inFile = "NULL";
  	bool isFileDefault = true;
   
   // handling command-line args
   if ( argc != 1 ) {
      for ( int i = 1; i < argc; i++ ) {
         string input = argv[i];
         stringstream ss( input );
         int seed;
         if ( ss >> seed ) { // integer read successfull?
            prng.seed( seed );
            continue; // process next arg
         }
         else {
            // read floor layout from user specified file
            inFile = input;
            isFileDefault = false;
            continue;
         }
      } // for
   }
   // handling complete
   
   cout << "Welcome to ChamberCrawler3000!" << endl;
   
   for ( ;; ) { // loop as long as the user wants to play game
      cout << "What would you like to play as today? " << '\n'
      << "w) Wizard" << '\n' << "k) Knight" << '\n' << "s) Samurai"
      << endl;
      
      string playerType;
      for ( ;; ) {
         cin >> playerType;
         if ( playerType  == "w" ) {
            playerType = "Wizard";
            break;
         }
         if ( playerType == "k" ) {
            playerType = "Knight";
            break;
         }
         if ( playerType == "s" ) {
            playerType = "Samurai";
            break;
         }
         cerr << "Did not recognize input" << endl;
      }
      // create player object and print it's type identifier
      cout << "You have chosen to play as a " << playerType << ". Good luck." << endl;
      
      Game PlayGame( inFile, playerType, isFileDefault );
      
      // Begin interpreting command
      string usrCmd; // user command
      string error = "Did not recognize input.";
      while ( true ) {
         
         cout << "What will you do?" << endl;
         if ( ! ( cin >> usrCmd ) ) break; // end of input
         
         if ( usrCmd == "r" ) {
            cout << "Are you sure you want to restart? (y/n)" << endl;
            cin >> usrCmd;
            if ( usrCmd == "y" ) goto NextGame; // restart game
            if ( usrCmd == "n" ) {
               PlayGame.print();
               continue;
            }
            cerr << error << endl;
         }
         
         if ( usrCmd == "q" ) {
            cout << "Are you sure you want to quit? (y/n)" << endl;
            cin >> usrCmd;
            if ( usrCmd == "y" ) goto End; // restart game
            if ( usrCmd == "n" ) {
               PlayGame.print();
               continue;
            }
            cerr << error << endl;
         }
         
         if ( usrCmd == "stopdeath" ) {
            PlayGame.stopdeath = true;
            cout << "Player death can no longer occur. " << endl;
            PlayGame.print();
            continue;
         }
         
         if ( usrCmd == "stopwander" ) {
            PlayGame.stopwander = true;
            cout << "Enemy actions that would result in movement now do nothing instead. " << endl;
            PlayGame.print();
            continue;
         }
         
         bool attackCmd = false;
         bool useCmd = false;
         
         // is input qualified i.e. does usr wants to attack or use item
         if ( usrCmd == "a" ) { attackCmd = true; } // player wants to attack
         if ( usrCmd == "u" ) { useCmd = true; } // player wants to use Item
         
         if ( attackCmd || useCmd ) cin >> usrCmd; // read direction of action
         
         // command list
         bool wasActionSuccess = false;
         int actionDirection = -1;
         
         if ( usrCmd == "no" ) { // move NORTH
            actionDirection = direction::no;
         }
         if ( usrCmd == "ne" ) { // move NORTH-EAST
            actionDirection = direction::ne;
         }
         if ( usrCmd == "ea" ) { // move EAST
            actionDirection = direction::ea;
         }
         if ( usrCmd == "se" ) { // move SOUTH-EAST
            actionDirection = direction::se;
         }
         if ( usrCmd == "so" ) { // move SOUTH
            actionDirection = direction::so;
         }
         if ( usrCmd == "sw" ) { // move SOUTH-WEST
            actionDirection = direction::sw;
         }
         if ( usrCmd == "we" ) { // move SOUTH-WEST
            actionDirection = direction::we;
         }
         if ( usrCmd == "nw" ) { // move NORTH-WEST
            actionDirection = direction::nw;
         }
         
         if ( actionDirection == -1 ) {
            if ( attackCmd ) {
               cerr << "Unexpected direction for attack." << endl;
               continue;
            }
            if ( useCmd ) {
               cerr << "Unexpected direction for use." << endl;
               continue;
            }
            
            cerr << error << endl;
            continue;
         }
         
         if ( !attackCmd && !useCmd ) {
            wasActionSuccess = PlayGame.player->move( actionDirection );
         }
         if ( attackCmd ) {
            wasActionSuccess = PlayGame.player->attack( actionDirection );
         }
         if ( useCmd ) {
            wasActionSuccess = PlayGame.player->use( actionDirection );
         }
         
         if ( wasActionSuccess ) {
         	
            if ( PlayGame.isWon() ){
               cout << "At long last, you have outmatched the Great Cavernous Chambers. Great things await you." << endl;
               break;
            }
            
            PlayGame.tick();
            PlayGame.print(); // print new state of current floor(after action)
            
            if ( PlayGame.isLost() && !PlayGame.stopdeath ) {
               cout << "You have been bested by the Great Cavernous Chambers. Good luck next time!" << endl;
               break;
            }
            
            PlayGame.resetTick();
         }
      } // while
      
      cout << "You achieved a score of " << PlayGame.player->getGP() << "." << endl;
      cout << "Play again? (y/n)" << endl;
      
      while( cin >> usrCmd ) {
      
         if ( usrCmd == "y" ) {
            goto NextGame; // begin new game
         }
         else if ( usrCmd == "n" ) {
            goto End; // end game
         }
         else {
            cout << "Did not recognize input." << endl;
         }
      } // while
      
   NextGame:;
   } //for
   
End:; // game end
}