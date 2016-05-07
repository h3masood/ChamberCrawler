#ifndef CELL_H
#define CELL_H

#include "gameobject.h"



class GameObject; // forward declaration because of mutually recursive reference
static const int maxNeighbours = 8;
static const int EMPTY = -1;

namespace direction {

	static const int nw = 0;
	static const int no = 1;
	static const int ne = 2;
	static const int ea = 3;
	static const int se = 4;
	static const int so = 5;
	static const int sw = 6;
	static const int we = 7;
	
}


class Cell {

    int _x, _y; // coords on floor
    char _display; // display character for cell
    GameObject *_contents; // Item or empty
    int numNeighbours;
    Cell *neighbours[maxNeighbours]; // list of neighbours

    public:
        Cell();
        ~Cell();
        int x() const; // get x-coord
        int y() const; // get y-coord
        void setCoords( int row, int col );
        void setDisplay( char dspl );
        void setContent( GameObject * contents );
        char display() const; // get display value
        GameObject *getContents() const;
        void addNeighbour( Cell *neighbour );
        Cell *getNeighbour( int neighbourPosn ) const;
   
};

#endif
