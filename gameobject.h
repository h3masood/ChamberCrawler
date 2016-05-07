#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>


class Cell; // forward declaration because of mutually recursive reference

class GameObject {
   
    protected:
      Cell * _location; // address of GameObject on current floor
      std::string type; // type of GameObject

    public:
        GameObject();
        virtual ~GameObject();
        // getters
        int x() const;
        int y() const;
        std::string getType() const;
        // setters
        void setLocation(Cell *);

        // Called after every turn
        virtual void tick() = 0;
        // end
        int locate (char object); // get location of object
        bool ticked; 
};

class Stairs : public GameObject {
   
   	int _x, _y; // coords
   
   public:
   	Stairs();
      void setCoords( int row, int col);
      Cell *getLocation();
      void tick();   
};

#endif
