#include "player.h"
#include "enemy.h"
#include "game.h"
#include "item.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
using namespace std;


Player::Player()
{
  _gold = 0;
  _turn = 1;
  _floor = 1;
}

int Player::getGP()
{
  return _gold;
}

void Player::setGP(int GP)
{
  _gold = GP;
}

void Player::tick() {} // do nothing


void Player::getStats()
{
  cout << left << setw(8) << "" << "Class: " << setw(12) << type << "GP: " << setw(15) << _gold <<"Floor " << setw(7) << _floor << endl;
  cout << left << setw(8) << "" << "HP: " << _health <<   "/" << setw(12) << _maxHP   << "Atk: "   << setw(14) << _attack  << "Def: "  <<  _defence <<  setw(8) << "%" << "Turn: " << _turn << endl;
}


//Adds 5 health to the player at the start of every turn
void Player::addHealth()
{
  const int HPgain = 5;
  if (_health + HPgain > _maxHP){
    this->setHealth(_maxHP);
  } else {
    this->setHealth(_health + HPgain);
  }
}


char Player::floor()
{
  int doorCount = 0;
  int passageCount = 0;
  int wallCount = 0;

  for (int i = 1; i<maxNeighbours; i+=2){
    char tile = checkCell(i);
    if (tile == '#'){
      passageCount += 1;
    } else if (tile == '+'){
      doorCount +=1;
    } else if (tile == '-' || tile == '|'){
      wallCount +=1;
    }
  } 
  
  if (wallCount == 2 && passageCount == 1){
    return '+';
  } else if (doorCount + passageCount >= 2){
    return '#';
  } else {
    return '.';
  }
}


bool Player::move(int direction)
{
  char dest = checkCell(direction);
  //move to empty location
  if (dest == '.' || dest == '#' || dest == '+' ){
    addHealth();
    _turn += 1;
    char icon = floor();
    Cell *_prevLocation = _location;
    Character::move(direction);
    _prevLocation->setDisplay(icon);

    //output successful move message
    cout << "You move ";
    switch (direction){
      case 0:
        cout << "north-west";
        break;
      case 1:
        cout << "north";
        break;
      case 2:
        cout << "north-east";
        break;
      case 3:
        cout << "east";
        break;
      case 4:
        cout << "south-east";
        break;
      case 5:
        cout << "south";
        break;
      case 6:
        cout << "south-west";
        break;
      case 7:
        cout << "west";
        break;
    }
    cout << "." << endl;

    return true;

  //move towards gold
  } else if (dest == '$'){
    addHealth();
    use(direction);
    return true;

  //move to stairs
  } else if (dest == '>'){
    addHealth();
    _turn += 1;
    _floor += 1;
    GameController->descendFloor(); // move to next floor or end game if won
    return true;

  //invalid move
  } else {
    cout << "You can't move in that direction." << endl;
    return false;
  }
}


bool Player::use(int direction)
{
  if (checkCell(direction) == '!' || checkCell(direction) == '$'){
    addHealth();
    _turn += 1;
    if (checkCell(direction) == '$' && ((Gold *)(_location->getNeighbour(direction)->getContents()))->getGoldType() == "DragonHoard" && _location->getNeighbour(direction)->getContents()->locate('D') != -1 ){

      cout << "It would be foolhardy to take the dragon's gold right now." << endl;

    } else {
      ((Item *)(_location->getNeighbour(direction)->getContents()))->use(this);
      _location->getNeighbour(direction)->setDisplay('.');
      delete _location->getNeighbour(direction)->getContents();
      _location->getNeighbour(direction)->setContent(0);
    } 
    return true;
  } else {
    cout << "There is nothing to use!" << endl;
    return false;
  }
}

bool Player::attack(int direction)
{
  Enemy *ep = dynamic_cast<Enemy *>(_location->getNeighbour(direction)->getContents());  
  //if pointer cannot be down-casted since it's not an enemy, thus given null 
  if (ep == 0){   
    cout << "There is nothing to attack!" << endl;
    return false;
  } else {
    addHealth();
    _turn += 1;
    int damage = (int)(ceil(_attack*(100.0 - ep->getDefence())/100.0));
    ep->setHealth(ep->getHealth() - damage);

    //output attack message
    cout << "You attack the " << ep->getType() << " with your ";
    if (type == "Knight"){
      cout << "Sword of Segfault";
    } else if (type == "Samurai"){
      cout << "Memory Corruption Katana";
    } else if (type == "Wizard"){
      cout << "Infinite Loop Laser";
    }
    cout << " for " << damage << " damage!" << endl;    

    //Check if enemy is hostile
    if (!(ep->isHostile())){
      cout << "You have declared war with the " << ep->getType() << "." << endl;
      ep->setHostile(true);
    }

    //If enemy dies, set their location pointer to null
    if (ep->getHealth() <= 0){
      delete ep;
     // dead = ep;
    }
    return true;
  }
}


char Player::checkCell(int direction)
{
  return _location->getNeighbour(direction)->display();
}


//KNIGHT
Knight::Knight()
{
  cout << "Enter: Knight()" << endl;
  type = "Knight";
  _health = 100;
  _attack = 50;
  _defence = 50;
  _maxHP = 100;

  GridBug::hostile = true;
  Goblin::hostile = true;
  Merchant::hostile = false;
  Orc::hostile = true;
  Dragon::hostile = true;
}


//WIZARD
Wizard::Wizard()
{
  type = "Wizard";
  _health = 60;
  _attack = 25;
  _defence = 0;
  _maxHP = 60;

  GridBug::hostile = true;
  Goblin::hostile = true;
  Merchant::hostile = false;
  Orc::hostile = true;
  Dragon::hostile = true;
}


//SAMURAI
Samurai::Samurai()
{
  type = "Samurai";
  _health = 80;
  _attack = 50;
  _defence = 15;
  _maxHP = 80;

  GridBug::hostile = false;
  Goblin::hostile = false;
  Merchant::hostile = false;
  Orc::hostile = false;
  Dragon::hostile = false;
}


bool Wizard::attack(int direction)
{
  Cell *current = _location->getNeighbour(direction);
  Enemy *ep;
  for (;;){
    if (current->display() == '|' || current->display() == '-' || current->display() == ' '){
      cout << "There is nothing to attack!" << endl;
      return false;
    }
    ep = dynamic_cast<Enemy *>(current->getContents());
    if (ep != 0){
      addHealth();
      _turn += 1;
      int damage = (int)(ceil(_attack*(100.0 - ep->getDefence())/100.0));
      ep->setHealth(ep->getHealth() - damage);

      cout << "You attack the " << ep->getType() << " with your Infinite Loop Laser for " << damage << " damage!" << endl;
      
      if (!(ep->isHostile())){
        cout << "You have declared war with the " << ep->getType() << "s." << endl;
        ep->setHostile(true);
      }
      
      if (ep->getHealth() <= 0){
        delete ep;
        //dead = ep;
      }
      return true;
    }
    current = current->getNeighbour(direction);
  }
  return false;
}


int Player::getScore()
{
   return _gold;
}


Game *Player::GameController; // reference to Game class


void Player::setGameController(Game *GC)
{
   Player::GameController = GC;
}

