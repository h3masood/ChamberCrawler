#ifndef ENEMY_H
#define ENEMY_H
//
#include "character.h"
#include "cell.h"
#include "game.h"

class Game;

class Enemy : public Character {

  protected:
 	 static Game *GameController; // reference to game class
    static int count; // number of Enemy objects
    int locate(char object); // get location of object on floor
   
  public:
    int enemyCount();
    virtual ~Enemy();
    virtual void setHostile(bool) = 0;
    virtual bool isHostile();

    virtual void tick();
    virtual void move();
    virtual void attack(int direction);
    static void setGameController(Game *GC);
};


class Orc : public Enemy {

  public:
    static bool hostile; // is hositile to player ?
    void setHostile(bool);
    Orc();
    ~Orc();
};


class Merchant : public Enemy {

  public: 
    static bool hostile; // is hositile to player ?
    void setHostile(bool);
    Merchant();
    ~Merchant();
};


class GridBug : public Enemy {

  public:
    static bool hostile; // is hositile to player ?
    void setHostile(bool);
    GridBug();
    ~GridBug();
    void tick();
    void move();
};


class Dragon : public Enemy {

  public:
    static bool hostile; // is hositile to player ?
    void setHostile(bool);
    Dragon();
    ~Dragon();
    void tick();
};


class Goblin : public Enemy {

  public:
    static bool hostile; // is hositile to player ?
    void setHostile(bool);
    Goblin();
    ~Goblin();
    void tick();
    void drink();
};


#endif
