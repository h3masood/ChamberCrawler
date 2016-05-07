#ifndef ITEM_H
#define ITEM_H

#include "gameobject.h"
#include "cell.h"
#include "character.h"
#include "player.h"


class Item : public GameObject {
   
   protected:
   	int _effect; // damage or boost value of Item
   
	public:
   	enum ItemKind { Potion, Gold };
   	Item( ItemKind type );
      std::string getItemType() const;
   	virtual void use( Character *player ) const = 0; // applies the effect of consuming/using the item
   	virtual ~Item();
    	virtual void tick();
   
   private:
      ItemKind _itemType;
};


class Potion : public Item {
   
   public:
   	enum Kind { RH, BA, BD, PH, WA, WD }; // potion kind
   	Potion( Kind potionType );
   	void use( Character *player ) const;
   
   private:
   	Kind _potionType;
};


class Gold : public Item {
   
   public:
   	enum Kind { GoldPile, DragonHoard };
   	Gold( Kind goldType );
    	void use(Character *player ) const;
    	std::string getGoldType();
   
   private:
   	Kind _goldType;
};
#endif
