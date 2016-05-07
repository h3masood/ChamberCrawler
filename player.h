#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"
#include "cell.h"
#include "game.h"
#include <string>

class Game; // forward declaration because of mutually recursive reference

class Player : public Character {

  protected:
    static Game *GameController; // reference to Game class
    int _gold; // gold picked up
    int _turn; // number of turns played
    int _floor; // onFloor value
    int _maxHP; // max possible HP

    bool validMove(int direction);
    Player();
    char checkCell(int direction);
    void addHealth();
    char floor();

  public:
    //player commands
    void tick();
    bool move(int direction);
    bool use(int direction);
    virtual bool attack(int direction);

    //getters and setters
    int getGP();
    int getScore();
    void setGP(int);
    void getStats();
    static void setGameController(Game *GC);
};


class Knight : public Player {

  public:
    Knight();
};


class Samurai : public Player {

  public:
    Samurai();
};


class Wizard : public Player {

  public:
    Wizard();
    bool attack(int direction);
};

#endif
