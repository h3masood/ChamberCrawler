#ifndef __GAME_H__
#define __GAME_H__

#include <list>
#include "cell.h"
#include "character.h"
#include "player.h"
#include "item.h"
#include "gameobject.h"
using namespace std;


class Player; // forward declaraion for mutually recursive reference

static const int maxSize = 5; // max number of neighbours
static const int totalAddresses = 5; // total number of references to player and stair objects

class Game {
   
      bool isMapDefault;
		Cell **floors[ maxSize ]; // store for floors of the game
		const unsigned int col; // length of a floor
  	 	const unsigned int row; // width of a floor
      unsigned int onFloor; // current floor on which player is at
    	Cell **currentFloor; // pointer to the floor on which the player is at in the game
      Cell *PlayerAddress[ totalAddresses ]; // references to the initial address of player on each floor
      Cell *StairAddress[ totalAddresses ]; // references to stair objects(cell)
		bool Won; // state of player's success
   
 	public:
   	Player *player;
      Stairs *stairs;
      Game( string fileName, string playerType, bool isFileDefault ); // generate game floors as specified in fileName
      void descendFloor(); // move to next floor
   	void print(); // print current floor
      bool isWon() const;
      bool isLost() const;
      void updateStats();
      // player action routines
      bool movePlayer( int direction );
      bool attack( int direction );
      void useItem( string direction );
      // end
      void nullifyPotions();
      void placeObjectsInChamber( list<GameObject *> &GO, string GameObjectType ); // place Item objects on current floor
   	void getStats();
      // spawning routines
      void spawnStairs();
		void spawnDragon( Cell **floor, int row, int col );
   	void spawnItems(); // place item in a randomn chamber
   	void spawnPlayer(); // place player in a random chamber
   	void spawnEnemies(); // place enemies in a randomn chamber
      // end
      // move enemy routines
      void tick();
    	void resetTick();
     	~Game();
      // cheat codes
     	static bool stopwander; // stop enemy movement
     	bool stopdeath; // player cannot die(ever!)
      // end
};

#endif
