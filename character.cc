#include "character.h"
#include <iostream>
using namespace std;


//getters
int Character::getHealth() const
{
  return _health;
}


int Character::getAttack() const
{
  return _attack;
}


int Character::getDefence() const
{
  return _defence;
}


//setters
void Character::setHealth(int health)
{
  _health = health;
}


void Character::setAttack(int attack)
{
  _attack = attack;
}


void Character::setDefence(int defence)
{
  _defence = defence;
}


void Character::move(int direction){
  _location->setContent(0);
  _location->getNeighbour(direction)->setDisplay(_location->display());
  _location->setDisplay('.');
  _location = _location->getNeighbour(direction); //set new location
  _location->setContent(this);
}

Character::~Character()
{
	// do nothing
}



