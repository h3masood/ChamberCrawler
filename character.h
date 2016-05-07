#ifndef CHARACTER_H
#define CHARACTER_H
#include "gameobject.h"
#include "cell.h"
#include <string>

class Character : public GameObject {

    protected:
      int _health; // HP
      int _attack; // attack capability
      int _defence; // defence capability

    public:
        //getters
        int getHealth() const;
        int getAttack() const;
        int getDefence() const;

        //setters
        void setHealth(int);
        void setAttack(int);
        void setDefence(int);

        virtual ~Character();
        void move(int direction);
};

#endif
