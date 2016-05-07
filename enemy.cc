#include "cell.h"
#include "enemy.h"
#include "item.h"
#include "gameobject.h"
#include "PRNG.h"
#include <string>
#include <cmath>
#include <iostream>
using namespace std;


extern list<GameObject *> ObjectsInUse;
extern PRNG prng;
int Enemy::count;


//ENEMY
Enemy::~Enemy()
{
  Enemy::count -= 1;
}


int Enemy::enemyCount()
{
  return count;
}


//return location of object if within 1 block radius, else return -1
int Enemy::locate(char object)
{
  for (int i = 0; i < maxNeighbours; i++){
    if(_location->getNeighbour(i)->display() == object){
      return i;
    }
  }
  return -1;
}


bool Enemy::isHostile()
{
  if (type == "Orc"){
    return Orc::hostile;
  } else if (type == "Goblin"){
    return Goblin::hostile;
  } else if (type == "Merchant"){
    return Merchant::hostile;
  } else if (type == "Grid Bug"){
    return GridBug::hostile;
  } else {
    return Dragon::hostile;
  }
}


void Enemy::tick()
{
  if (isHostile()){
    int direction = locate('@');
    if (direction != -1){
      attack(direction);
    } else {
      if ( !Game::stopwander ) move();
    }    
  } else {
    if ( !Game::stopwander ) move();
  }
}


void Enemy::move()
{
  int emptyPos = 0; //number of empty positions that the enemy can move to
  for (int i = 0; i < maxNeighbours; i++){
    if (_location->getNeighbour(i)->display() == '.'){
      emptyPos +=1;
    }
  }
  if (emptyPos > 0){
    int pos = prng(emptyPos-1);
    for (int i = 0; i < maxNeighbours; i++){
      if (_location->getNeighbour(i)->display() == '.'){
        if (pos == 0){
          Character::move(i);
          break;
        } else {
          pos-=1;
        }
      }
    }
  }
}


void Enemy::attack(int direction)
{
  Character *cp = (Character *)(_location->getNeighbour(direction)->getContents());
  int damage = ceil(_attack*(100.0 - cp->getDefence())/100.0);
  if (damage > cp->getHealth()){
    cp->setHealth(0);
  } else {
    cp->setHealth(cp->getHealth() - damage);
  }
  cout << "The evil " << type << " strikes you for " << damage << " damage!" << endl;
} 


//ORC
Orc::Orc()
{
  type = "Orc";
  _health = 120;
  _attack = 30;
  _defence = 30;
  Enemy::count +=1;
}


Orc::~Orc()
{
  GameObject *loot = new Gold(Gold::GoldPile);
  loot->setLocation(_location);
  _location->setContent(loot);
  _location->setDisplay('$');
}


void Orc::setHostile(bool isHostile)
{
  Orc::hostile = isHostile;
}


bool Orc::hostile; // is orc hostile?


//MERCHANT
Merchant::Merchant()
{
  type = "Merchant";
  _health = 100;
  _attack = 75;
  _defence = 5;
  Enemy::count +=1;
}


Merchant::~Merchant()
{
  GameObject *loot = new Gold(Gold::GoldPile);
  loot->setLocation(_location);
  _location->setContent(loot);
  _location->setDisplay('$');

}


void Merchant::setHostile(bool isHostile)
{
  Merchant::hostile = isHostile;
}
bool Merchant::hostile; // is merchant hostile?


//GRID BUG
GridBug::GridBug()
{
  type = "Grid Bug";
  _health = 50;
  _attack = 20;
  _defence = 10;
  Enemy::count +=1;
}


GridBug::~GridBug()
{
  //drop random potion upon death
  Potion::Kind type;
  int num = prng (5);
  switch (num){
    case 0:
      type = Potion::RH;
      break;
    case 1:
      type = Potion::BA;
      break;
    case 2:
      type = Potion::BD;
      break;
    case 3:
      type = Potion::PH;
      break;
    case 4:
      type = Potion::WA;
      break;
    case 5:
      type = Potion::WD;
      break;
  }
  GameObject *loot = new Potion(type);
  loot->setLocation(_location);
  _location->setContent(loot);
  _location->setDisplay('!');
  ObjectsInUse.push_back( loot );
}


void GridBug::tick()
{
  if (isHostile()){
    int direction = locate('@');
    if (direction == direction::no || direction == direction::ea || direction == direction::so || direction == direction::we){
      Enemy::attack(direction);
    } else {
      if ( !Game::stopwander ) move();
    }
  }else{
    if ( !Game::stopwander ) move();
  }
}


void GridBug::move()
{
  int emptyPos = 0;
  for (int i = 1; i<maxNeighbours; i+=2){
    if (_location -> getNeighbour(i)->display() == '.'){
      emptyPos += 1;
    }
  }
  if (emptyPos > 0){
    int pos = prng(emptyPos-1);
    for (int i = 1; i<maxNeighbours; i+=2){
      if (_location->getNeighbour(i)->display()=='.'){
        if (pos == 0){
          Character::move(i);
          break;
        }else {
          pos -=1;
        }
      }
    }
  }
}

void GridBug::setHostile(bool isHostile)
{
  GridBug::hostile = isHostile;
}
bool GridBug::hostile; // is GridBug hostile?


//Dragon
Dragon::Dragon()
{
  type = "Dragon";
  _health = 150;
  _attack = 50;
  _defence = 10;
  Enemy::count +=1;
}


Dragon::~Dragon()
{
  _location->setContent(0);
  _location->setDisplay('.');
}


void Dragon::setHostile(bool isHostile)
{
  Dragon::hostile = isHostile;
}


void Dragon::tick()
{
  if (isHostile()){
    int direction = locate('@');
    if (direction != -1){
      Enemy::attack(direction);
    }
  }
}
bool Dragon::hostile; // is dragon hostile ?


//Goblin
Goblin::Goblin()
{
  type = "Goblin";
  _health = 75;
  _attack = 30;
  _defence = 20;
  Enemy::count +=1;
}


Goblin::~Goblin()
{
  GameObject *loot = new Gold(Gold::GoldPile);
  loot->setLocation(_location);
  _location->setContent(loot);
  _location->setDisplay('$');
}


void Goblin::setHostile(bool isHostile)
{
  Goblin::hostile = isHostile;
}


void Goblin::tick()
{
  int location;
  if (isHostile()){
    location = locate('@');
    if (location != -1){
      Enemy::attack(location);
    } else {
      location = locate('!');
      if (location != -1){
        drink();
      } else {
        if ( !Game::stopwander ) Enemy::move();
      }
    }
  } else {
    location = locate('!');
    if (location != -1){
      drink();
    } else {
      if ( !Game::stopwander ) Enemy::move();
    }
  }
}


void Goblin::drink()
{
  int emptyPos = 0;
  for (int i = 0; i<maxNeighbours; i++){
    if (_location -> getNeighbour(i)->display() == '!'){
      emptyPos += 1;
    }
  }
  if (emptyPos > 0){
    int pos = prng(emptyPos-1);
    for (int i = 0; i<maxNeighbours; i++){
      if (_location->getNeighbour(i)->display()=='!'){
        if (pos == 0){
          //apply potion to goblin and delete potion 
          ((Item *)(_location->getNeighbour(i)->getContents()))->use(this);
          delete _location->getNeighbour(i)->getContents();
          _location->getNeighbour(i)->setContent(0);
          _location->getNeighbour(i)->setDisplay('.');
          break;
        }else{
          pos -=1;
        }
      }
    }
  }
}
bool Goblin::hostile; // is goblin hostile?
Game *Enemy::GameController; // reference to game class


void Enemy::setGameController(Game *GC)
{
   Enemy::GameController = GC;
}

