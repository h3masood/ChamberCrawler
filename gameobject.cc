#include "gameobject.h"
#include "cell.h"
#include <string>
#include <iostream>
using std::cerr;
using std::endl;


GameObject::GameObject()
{
  ticked = false;
}


GameObject::~GameObject()
{
	// do nothing
}


int GameObject::x() const
{
  return _location->x();
}


int GameObject::y() const
{
  return _location->y();
}


std::string GameObject::getType() const
{
  return type;
}


void GameObject::setLocation(Cell * location)
{
  _location = location;
}


Stairs::Stairs() {} // nothing for constructor to do


void Stairs::setCoords( int row, int col)
{
   _x = col;
   _y = row;
}


Cell *Stairs::getLocation()
{
   return _location;
}


void Stairs::tick()
{
	// do nothing
}

//return location of object if within 1 block radius of character, else return -1
int GameObject::locate(char object)
{
  for (int i = 0; i < maxNeighbours; i++){
    if(_location->getNeighbour(i)->display() == object){
      return i;
    }
  }
  return -1;
}

