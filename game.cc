#include "game.h"
#include "PRNG.h"
#include "enemy.h"
#include "item.h"
#include <map>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
using namespace std;


extern PRNG prng;
list<GameObject *> ObjectsInUse; // list of all the objects that are spread througout all the floors
bool Game::stopwander;

void addNeighbours( Cell **grid, int row, int col )
{
   
   for  (int j = 0; j < row; j++ ) {
      for (int k = 0; k < col; k++ ) {
         // adding neighbours
         // order of addition of neighbours is pivotal to the correct game play
         if ( j == 0 ) { // first row ?
            grid[j][k].addNeighbour( &grid[j+1][k] ); // SOUTH
            
            if ( k == 0 ) { // first col ?
               grid[j][k].addNeighbour( &grid[j][k+1] ); // EAST
               grid[j][k].addNeighbour( &grid[j+1][k+1] ); // SOUTH-EAST
            }
            else if ( k == col - 1 ) { // last col ?
               grid[j][k].addNeighbour( &grid[j][k-1] ); // WEST
               grid[j][k].addNeighbour( &grid[j+1][k-1] ); // SOUTH-WEST
            }
            else {
               grid[j][k].addNeighbour( &grid[j][k-1] ); // WEST
               grid[j][k].addNeighbour( &grid[j+1][k-1] ); // SOUTH-WEST
               grid[j][k].addNeighbour( &grid[j][k+1] ); // EAST
               grid[j][k].addNeighbour( &grid[j+1][k+1] ); // SOUTH-EAST
            }
         }
         else if ( j == row - 1 ) { // last row ?
            grid[j][k].addNeighbour( &grid[j-1][k] ); // NORTH
            
            if ( k == 0 ) { // first col ?
               grid[j][k].addNeighbour( &grid[j][k+1] ); // EAST
               grid[j][k].addNeighbour( &grid[j-1][k+1] ); // NORTH-EAST
            }
            else if ( k == col - 1 ) { // last col ?
               grid[j][k].addNeighbour( &grid[j][k-1] ); // WEST
               grid[j][k].addNeighbour( &grid[j-1][k-1] ); // NORTH-WEST
            }
            else {
               grid[j][k].addNeighbour( &grid[j][k-1] ); // WEST
               grid[j][k].addNeighbour( &grid[j][k+1] ); // EAST
               grid[j][k].addNeighbour( &grid[j-1][k+1] ); // NORTH-EAST
               grid[j][k].addNeighbour( &grid[j-1][k-1] ); // NORTH-WEST
            }
         }
         else { // middle rows
            if ( k == 0 ) { // first col ?
               grid[j][k].addNeighbour( &grid[j-1][k+1] ); // NORTH-EAST
               grid[j][k].addNeighbour( &grid[j-1][k] ); // NORTH
               grid[j][k].addNeighbour( &grid[j][k+1] ); // EAST
               grid[j][k].addNeighbour( &grid[j+1][k+1] ); // SOUTH-EAST
               grid[j][k].addNeighbour( &grid[j+1][k] ); // SOUTH
               grid[j][k].addNeighbour( 0 ); // SOUTH-WEST
               grid[j][k].addNeighbour( 0 ); // WEST
               grid[j][k].addNeighbour( 0 ); // NORTH-WEST
               
            }
            else if ( k == col - 1 ) { // last col ?
               grid[j][k].addNeighbour( 0 ); // NORTH-EAST
               grid[j][k].addNeighbour( &grid[j-1][k] ); // NORTH
               grid[j][k].addNeighbour( 0 ); // EAST
               grid[j][k].addNeighbour( 0 ); // SOUTH-EAST
               grid[j][k].addNeighbour( &grid[j+1][k] ); // SOUTH
               grid[j][k].addNeighbour( &grid[j+1][k-1] ); // SOUTH-WEST
               grid[j][k].addNeighbour( &grid[j][k-1] ); // WEST
               grid[j][k].addNeighbour( &grid[j-1][k-1] ); // NORTH-WEST
            }
            else {
               grid[j][k].addNeighbour( &grid[j-1][k-1] ); // NORTH-EAST
               grid[j][k].addNeighbour( &grid[j-1][k] ); // NORTH
               grid[j][k].addNeighbour( &grid[j-1][k+1] ); // EAST
               grid[j][k].addNeighbour( &grid[j][k+1] ); // SOUTH-EAST
               grid[j][k].addNeighbour( &grid[j+1][k+1] ); // SOUTH
               grid[j][k].addNeighbour( &grid[j+1][k] ); // SOUTH-WEST
               grid[j][k].addNeighbour( &grid[j+1][k-1] ); // WEST
               grid[j][k].addNeighbour( &grid[j][k-1] ); // NORTH-WEST
            }
         }
         // adding neighbours complete
      } // for
   } // for
}


Game::Game( string fileName, string playerType, bool isFileDefault )
: col( 79 ), row( 25 )
{
   // set game data members
   Won = false;
   onFloor = 0;
   isMapDefault = isFileDefault;
   Game::stopwander = false;
   stopdeath = false;
   
   // create player object
   if ( playerType == "Wizard" ) player = new Wizard();
   if ( playerType == "Knight" ) player = new Knight();
   if ( playerType == "Samurai" ) player = new Samurai();
   
   // update GameControllers to point to myself
   Player::setGameController( this );
   Enemy::setGameController( this );
   
   // helper variables
   int numPlayerAddresses = 0;
   int numStairAddresses = 0;
   bool isPlayerSet = false; // has the first instance of player been read?
   bool isStairSet = false; // has the first instance of stairs been read?
   ifstream inFile;
   inFile.exceptions( ios_base::failbit ); // set file to throw exceptions
   
   // calibrate the stream for input
   char input; // tmp store for file content(s)
   inFile >> noskipws;
   int i = onFloor; // tmp for current floor
   
   if ( !isFileDefault ) {
      try { // attempt to read floor(s) layout from "fileName"
        	inFile.open( fileName.c_str() );
         isMapDefault = false;
         
         for ( ;; ) { // loop until all 5 floor layouts are read
            // adding floors to the list
            if ( i == 5 ) break;
            Cell **grid = new Cell *[row]; // create matrix
            for ( unsigned int j = 0; j < row; j++ ) {
               grid[j] = new Cell[col];
               // initializing a row on floor
               for ( unsigned int k = 0; k < col; k++ ) {
                  // setting data members
                  grid[j][k].setCoords( j, k );
                  inFile >> input;
                  
                  if ( input == '\n' ) inFile >> input; // ignore newline
                  if ( input >= '0' && input <= '5' ) {
                     int type = ( input - '0' ) % 48;
                     Potion::Kind potionType;
                     switch ( type ) {
                        case 0:
                           potionType = Potion::RH;
                           break;
                        case 1:
                           potionType = Potion::BA;
                           break;
                        case 2:
                           potionType = Potion::BD;
                           break;
                        case 3:
                           potionType = Potion::PH;
                           break;
                        case 4:
                           potionType = Potion::WA;
                           break;
                        case 5:
                           potionType = Potion::WD;
                           break;
                     } // switch
                     Potion *newPotion = new Potion( potionType );
                     ObjectsInUse.push_back( newPotion ); // update object list to include newly allocated potion
                     
                     // update the contents of cell and potion
                     grid[j][k].setContent( newPotion );
                     newPotion->setLocation( &grid[j][k] );
                     input = '!'; // assign approppriate display
                  } // if
                  
                  if ( input == '6' || input == '7' ) {
                     int type = ( input - '0' ) % 48;
                     Gold::Kind goldType;
                     switch ( type ) {
                        case 6:
                           goldType = Gold::GoldPile;
                           break;
                        case 7:
                           goldType = Gold::DragonHoard;
                           break;
                     } // switch
                     Gold *newGold = new Gold( goldType );
                     ObjectsInUse.push_back( newGold ); // update object list to include newly allocated potion
                     
                     // update the contents of cell and gold
                     grid[j][k].setContent( newGold );
                     newGold->setLocation( &grid[j][k] );
                     input = '$'; // assign approppriate display
                  }
                  
                  if ( input == '@' ) {
                     if ( !isPlayerSet ) {
                        isPlayerSet = true;
                        player->setLocation( &grid[j][k] );
                        grid[j][k].setContent( player );
                     }
                     PlayerAddress[ numPlayerAddresses ] = &grid[j][k];
                     numPlayerAddresses += 1;
                  }
                  
                  if ( input == '>' )  {
                     stairs = new Stairs();
                     if ( !isStairSet ) {
                        isStairSet = true;
                        stairs->setLocation( &grid[j][k] );
                        grid[j][k].setContent( stairs );
                     }
                     StairAddress[ numStairAddresses ] = &grid[j][k];
                     numStairAddresses += 1;
                  }
                  
                  if ( input == 'X' ) {
                     GridBug *newGridBug = new GridBug();
                     ObjectsInUse.push_back( newGridBug );
                     newGridBug->setLocation( &grid[j][k] );
                     grid[j][k].setContent( newGridBug );
                  }
                  
                  if ( input == 'g' ) {
                     Goblin *newGoblin =  new Goblin();
                     ObjectsInUse.push_back( newGoblin );
                     newGoblin->setLocation( &grid[j][k] );
                     grid[j][k].setContent( newGoblin );
                  }
                  
                  if ( input == 'M' ) {
                     Merchant *newMerchant = new Merchant();
                     ObjectsInUse.push_back( newMerchant );
                     newMerchant->setLocation( &grid[j][k] );
                     grid[j][k].setContent( newMerchant );
                  }
                  
                  if ( input == 'O' ) {
                     Orc *newOrc = new Orc();
                     ObjectsInUse.push_back( newOrc );
                     newOrc->setLocation( &grid[j][k] );
                     grid[j][k].setContent( newOrc );
                  }
                  
                  if ( input == 'D' ) {
                     Dragon *newDragon = new Dragon();
                     ObjectsInUse.push_back( newDragon );
                     newDragon->setLocation( &grid[j][k] );
                     grid[j][k].setContent( newDragon );
                  }
                  grid[j][k].setDisplay( input );
               } // for
            } // for
            
            addNeighbours( grid, row, col );
            floors[ i ] = grid ; // add floor to the back of list
            i += 1;
         } // for
         
         print(); // print floors in their initial state
         return; // game initialization complete
      }
      catch ( ios_base::failure ) {
         cerr << "Problem loading map from file '" << fileName << "' ." << endl;
      } // try
   } // if
   
   // read default map
   inFile.open( "defaultMap.txt" ); // open default file
   for ( ;; ) {
      // add floors to the list
      if ( i == 5 ) break;
      Cell **grid = new Cell *[row]; // create matrix
      for ( unsigned int j = 0; j < row; j++ ) {
         grid[j] = new Cell[col];
         // initialze a row on floor
         for ( unsigned int k = 0; k < col; k++ ) {
            // setting data members
            grid[j][k].setCoords( j, k );
            inFile >> input;
            if ( input == '\n' ) inFile >> input; // ignore newline
            grid[j][k].setDisplay( input );
         } // for
      } // for
      
      addNeighbours( grid, row, col );
      floors[ i ] = grid ; // add floor to the back of list
      i += 1;
      
   } // for
   stairs = 0;
   // Begin spawning objects on Floor
   spawnPlayer();
   spawnItems(); // place items on board
   spawnEnemies();
   print();
}


void Game::print()
{
   Cell **currentFloor = floors[ onFloor ];
   
   for ( unsigned int j = 0; j < row; j++ ) {
      for( unsigned int k = 0; k < col; k++ ) {
         cout << currentFloor[j][k].display();
      }
      cout << '\n';
   }
   getStats();
}


// Spawn Dragon
void Game::spawnDragon( Cell **floor, int row, int col )
{
   try {
      int tries = 0;
      string error = "No place for Dragon";
      bool success = false;
      while ( !success ) {
         tries += 1;
         if ( tries == 25 ) throw error; // prevents infinite loop
         
         // Caution: 2 gets mapped to -1, the rest are mapped to themselves
         int rowPos = prng( 0, 2 ); // row position
         int colPos = prng( 0, 2 ); // column position
         if ( rowPos == 0 && colPos == 0 ) continue;
         if ( rowPos == 2 ) rowPos = -1;
         if ( colPos == 2 ) colPos = -1;
         
         rowPos += row;
         colPos += col;
         if ( floor[rowPos][colPos].display() == '.' ) {
            Dragon *newDragon = new Dragon();
            floor[rowPos][colPos].setContent( newDragon );
            newDragon->setLocation( &floor[rowPos][colPos] );
            ObjectsInUse.push_back( newDragon );
            success = true;
         }
      } // while
   }
   catch ( string warning ) {
      throw; // propogate exception
   }
}


// Spawning Objects on FLoor

void Game::placeObjectsInChamber( list<GameObject *> &GO, string GameObjectType )
{
   // maps of rows, columuns, chambers and the number of GameObject(s) at that measure of dimension
   map<int, int> usedRows;
   map<int, int> usedColumns;
   map<int, int> ChamberItemCount;
   
   int prevChamber = 0; // index of prev chamber where an object was placed
   while ( ! GO.empty() ) { // loop until all objects are placed on floor
      int ChamberNum = prng( 0, 4 ); // allocate a chamber for random generation
      if ( ChamberNum ==  prevChamber ) continue; // ignore: dont place item on the same chamber as last time
      
      ChamberItemCount[ ChamberNum ] += 1; // increment value( representative of number of objects in that chamber ) associated with key ChamberNum
      // do stuff
      if ( GameObjectType == "Potion" ) {
         if ( ChamberItemCount[ ChamberNum ] > 6 ) continue;
      }
      
      if ( GameObjectType == "GoldPile" ) {
         if ( ChamberItemCount[ ChamberNum ] > 6 ) continue;
      }
      
      if ( GameObjectType == "Enemy" ) {
         if ( ChamberItemCount[ ChamberNum ] > 8 ) continue;
      }
      
      // Chamber Length & Width Parameters
      int LBRow; // lower boundary for row
      int LBCol; // lower boundary for col
      int UBRow; // upper boundary for row
      int UBCol; // upper boundary for col
      int LB1Row; // lower boundary 1 for row
      int LB1Col; // lower boundary 1 for col
      int UB1Row; // upper boundary 1 for row
      int UB1Col; // upper boundary 1 for col
      
      // setting boundary parameters for chamber(s)
      switch ( ChamberNum ) {
            
         case 0:
            UBRow = 3;
            UBCol = 3;
            LBRow = 6;
            LBCol = 28;
            break;
            
         case 1:
            UBRow = 3;
            UBCol = 39;
            LBRow = 6;
            LBCol = 72;
            
            UB1Row = 7;
            UB1Col = 57;
            LB1Row = 12;
            LB1Col = 76;
            break;
            
         case 2:
            UBRow = 10;
            UBCol = 39;
            LBRow = 13;
            LBCol = 50;
            break;
            
         case 3:
            UBRow = 15;
            UBCol = 4;
            LBRow = 21;
            LBCol = 24;
            break;
            
         case 4:
            UBRow = 16;
            UBCol = 66;
            LBRow = 18;
            LBCol = 76;
            
            UB1Row = 19;
            UB1Col = 36;
            LB1Row = 21;
            LB1Col = 76;
            break;
      }
      // begin placing items on floor
      Cell **grid = floors[ onFloor ]; // temporarily hold the current floor
      int random = 0; // helper for random placement
      int row, col;
      try { // throws an exception if the dragon cannot be placed appropriately
         int tries = 0;
         switch ( ChamberNum ) {
               
            case 0: case 2: case 3:
               while ( true ) {
                  tries += 1;
                  if ( tries >= 20 ) break; // guards against infinite loops
                  row = prng( UBRow, LBRow );
                  col = prng( UBCol, LBCol );
                  usedRows[ row ] += 1;
                  usedColumns[ col ] += 1;
                  
                  if ( usedRows[ row ] <= 3 && usedColumns[ col ] <= 4 ) {
                     if ( grid[row][col].display() == '.') {
                        if ( GameObjectType == "DragonHoard" ) spawnDragon( grid, row, col ); // put dragon on board if object is dragon hoard
                        grid[row][col].setContent( *GO.begin() );
                        ( *GO.begin() )->setLocation( &grid[row][col] );
                        GO.pop_front(); // remove gameObject
                        break; // object placed
                     }
                     continue; // try again
                  }
               } // while
               break;
               
            case 1: case 4:
               while ( true ) {
                  tries += 1;
                  if ( tries >= 20 ) break; // prevents infinite loop
                  
                  // generate coords
                  if ( ( random % 2 ) == 0 ) {
                     row = prng( UB1Row, LB1Row );
                     col = prng( UB1Col, LB1Col );
                  }
                  else {
                     row = prng( UBRow, LBRow );
                     col = prng( UBCol, LBCol );
                  }
                  
                  random += 1; // ensure continuous pseudo-randomness
                  usedRows[ row ] += 1;
                  usedColumns[ col ] += 1;
                  
                  if ( usedRows[ row ] <= 3 && usedColumns[ col ] <= 4 ){
                     // check if coords already occupied
                     if ( grid[row][col].display() == '.') {
                        if ( GameObjectType == "DragonHoard" ) spawnDragon( grid, row, col ); // put dragon on board if object is dragon hoard
                        grid[row][col].setContent( *GO.begin() );
                        ( *GO.begin() )->setLocation( &grid[row][col] );
                        GO.pop_front(); // remove gameObject
                        break;
                     }
                     continue; // try again
                  }
               } // while
               break;
         } // switch
      }
      catch ( string warning ) {
         continue; // dragon could not be placed appropriately, try again
      }// try
      prevChamber = ChamberNum; // update the index of prev chamber
   } // for
}


void Game::spawnItems()
{
   
   // Spawning Potions
   int numPotions = 10;
   list<GameObject *> potionList;
   
   
   for ( int i = 0; i < numPotions; i++ ) {
      int type = prng( 0, 5 ); // generate random num in [0,5]
      Potion::Kind potionType;
      switch ( type ) {
         case 0:
            potionType = Potion::RH;
            break;
         case 1:
            potionType = Potion::BA;
            break;
         case 2:
            potionType = Potion::BD;
            break;
         case 3:
            potionType = Potion::PH;
            break;
         case 4:
            potionType = Potion::WA;
            break;
         case 5:
            potionType = Potion::WD;
            break;
      }
      
      Potion *newPotion = new Potion( potionType );
      ObjectsInUse.push_back( newPotion );
      potionList.push_back( newPotion );
   }
   
   placeObjectsInChamber( potionList, "Potion" );
   
   // Spawning Gold
   int numGold = 10;
   list<GameObject *> GoldPiles;
   list<GameObject *> DragonHoards;
   
   
   for ( int i = 0; i < numGold; i++ ) {
      bool isDragonHoard = false;
      int type = prng( 0, 7 ); // generate random num in [0,5]
      Gold::Kind goldType;
      
      switch ( type ) {
         case 0: case 1: case 2: case 3: case 4: case 5: case 6:
            goldType = Gold::GoldPile;
            break;
         case 7:
            goldType = Gold::DragonHoard;
            isDragonHoard = true;
            break;
      } // switch
      
      Gold *newGold = new Gold( goldType );
      ObjectsInUse.push_back( newGold );
      
      if ( isDragonHoard ) {
         DragonHoards.push_back( newGold );
      }
      else {
         GoldPiles.push_back( newGold );
      }
      
   } // for
   
   placeObjectsInChamber( GoldPiles, "GoldPile" );
   placeObjectsInChamber( DragonHoards, "DragonHoard" );
   
}

void Game::spawnStairs()
{
   if ( stairs != 0 ) delete stairs; // delete old stairs
   stairs = new Stairs();
}


void Game::spawnPlayer()
{
   list<GameObject *> mainObjects;
   mainObjects.push_back( player );
   spawnStairs();
   mainObjects.push_back( stairs );
   placeObjectsInChamber( mainObjects, "PrimaryObjects" );
}


void Game::spawnEnemies()
{
   int numEnemies = 20;
   int enemyKind = 0;
   list<GameObject *> Enemies;
   
   for( int i = 0; i < numEnemies; i++ ) {
      
      enemyKind = prng( 0, 5 );
      
      switch( enemyKind ){
            
         case 0: case 1:
         {
         GridBug *newGridBug = new GridBug();
         Enemies.push_back( newGridBug );
         ObjectsInUse.push_back( newGridBug );
         break;
         }
            
         case 2: case 3:
         {
         Goblin *newGoblin = new Goblin();
         Enemies.push_back( newGoblin );
         ObjectsInUse.push_back( newGoblin );
         break;
         }
            
         case 4:
         {
         Orc *newOrc = new Orc();
         Enemies.push_back( newOrc );
         ObjectsInUse.push_back( newOrc );
         break;
         }
            
         case 5:
         {
         Merchant *newMerchant = new Merchant();
         Enemies.push_back( newMerchant );
         ObjectsInUse.push_back( newMerchant );
         break;
         }
            
      } // switch
   } // for
   
   placeObjectsInChamber( Enemies, "Enemy" );
}
// Spawning complete

void Game::getStats()
{
   player->getStats();
   cout << '\n'; // print newline
}


void Game::tick()
{
   Cell **floor = floors[onFloor];
   for ( unsigned int i = 0; i < row; i++ ) {
      for ( unsigned int j = 0; j < col; j++ ) {
         if ( floor[i][j].getContents() != 0 ) {
            GameObject *content = floor[i][j].getContents();
            if (content->ticked == false){
               content->tick();
               content->ticked = true;
            }
         }
      }
   }
}


void Game::resetTick()
{
   Cell **floor = floors[ onFloor ];
   for ( unsigned int i = 0; i < row; i++ ) {
      for ( unsigned int j = 0; j < col; j++ ) {
         if ( floor[i][j].getContents() != 0 ) {
            GameObject *content = floor[i][j].getContents();
            content->ticked = false;
         }
      }
   }
}


void Game::nullifyPotions()
{
   Knight *tmp = dynamic_cast<Knight *>( player );
   if ( tmp != 0 ) {
      player->setAttack( 50 );
      player->setDefence( 50 );
      return;
   }
   
   { // new scope
      Wizard *tmp = dynamic_cast<Wizard *>( player );
      if ( tmp != 0 ) {
         player->setAttack( 25 );
         player->setDefence( 0 );
      }
   }
   
   { // new scope
      Samurai *tmp = dynamic_cast<Samurai *>( player );
      if ( tmp != 0 ) {
         player->setAttack( 50 );
         player->setDefence( 15 );
      }
   }
}


void Game::descendFloor()
{
   if ( onFloor == 4 ) {
      // Game won
      Won = true;
      return;
   }
   onFloor += 1;
   cerr << "You descend down to floor " << onFloor + 1 << "." << endl;
   if ( !isMapDefault ) {
      player->setLocation( PlayerAddress[ onFloor ] ); // set new player location
      ( PlayerAddress[ onFloor ] )->setContent( player );
      stairs->setLocation( StairAddress[ onFloor ] ); // set new stair location
      ( StairAddress[ onFloor ] )->setContent( stairs );
      return;
   }
   
   spawnPlayer();
   nullifyPotions();
   spawnItems();
   spawnEnemies();
   
}

bool Game::isWon() const
{
   return Won;
}


bool Game::isLost() const
{
   return ( player->getHealth() == 0 );
}


Game::~Game()
{
   delete player;
   delete stairs;
   list<GameObject *>::iterator it = ObjectsInUse.begin();
   for ( ;; ) {
      if ( it == ObjectsInUse.end() ) break;
      delete *it;
      it++;
   }
   
   //cerr << "Enter ~Game()" << endl;
   for (  int i = 0; i < maxSize; i++ ) {
      Cell **floor = floors[i];
      for ( int j = 0; j < row; j++ ) {
         delete [] floor[j]; // delete the entire row
      } // for
      delete [] floor;
   } // for
   
   ObjectsInUse.clear();
   
}

