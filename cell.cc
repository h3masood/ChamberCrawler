#include "cell.h"
#include "item.h"
#include "character.h"
#include "enemy.h"
#include <cassert>
#include <iostream>
using namespace std;


Cell::Cell()
{
	_contents = 0; // No content
   _x = _y = EMPTY;
   numNeighbours = 0; // no neighbours
}

int Cell::x() const
{
   return _x;
}

Cell::~Cell() 
{
	// do nothing
}

int Cell::y() const
{
   return _y;
}

void Cell::setCoords( int row, int col )
{
   _y = row;
   _x = col;
}


char Cell::display() const
{
   return _display;
}


void Cell::setDisplay( char dspl )
{
   _display = dspl;
}

void Cell::setContent( GameObject *contents )
{
   _contents = contents;
   
   // check if content == Potion
   {
		Potion *tmp = dynamic_cast<Potion *>( contents );
   	if ( tmp != 0 ) {
      	_display = '!';
      	return;
   	}
   }
   
   // check if content == Gold
   {
   	Gold *tmp = dynamic_cast<Gold *>(contents);
   	if ( tmp != 0 ) {
    	  _display = '$';
    	  return;
      }
   }
   
   // check if content == Dragon
   {
   	Dragon *tmp = dynamic_cast<Dragon *>( contents );
   	if ( tmp != 0 ) {
   		_display = 'D';
         return;
      }
   }
   
   // check if content == gridBug
   {
   	GridBug *tmp = dynamic_cast<GridBug *>( contents);
   	if ( tmp != 0 ) {
      	_display = 'X';
      	return;
   	}
   }
   
   // check if content == Goblin
   {
   	Goblin *tmp = dynamic_cast<Goblin *>( contents );
   	if ( tmp != 0 ) {
      	_display = 'g';
         return;
   	}
   }
   
   // check if content == Orc
   {
   	Orc *tmp = dynamic_cast<Orc *>( contents );
   	if ( tmp != 0 ) {
         _display = 'O';
         return;
   	}
   }
   
   // check if content == Merchant
   {
   	Merchant *tmp = dynamic_cast<Merchant *>( contents );
   	if ( tmp != 0 ) {
      	_display = 'M';
      	return;
   	}
   }
   
   // check if content == Player
   {
      Player *tmp = dynamic_cast<Player *>( contents );
   	if ( tmp != 0 )  {
      	_display = '@';
      	return;
   	}
   }
   
   // check if content == Stairs
   {
   	Stairs *tmp = dynamic_cast<Stairs *>( contents );
   	if ( tmp != 0 ) {
      	_display = '>';
   	}
   }
   
}

GameObject *Cell::getContents() const
{
   return _contents;
}


void Cell::addNeighbour( Cell *neighbour )
{
   neighbours[numNeighbours] = neighbour;
   numNeighbours += 1;
}

Cell *Cell::getNeighbour( int neighbourPosn ) const
{
	return neighbours[neighbourPosn];
}



