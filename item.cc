#include "item.h"
#include <iostream>
using namespace std;


static int statMin = 0;
// CLASS Item


Item::Item( ItemKind type )
{
   GameObject::type = "Item";
   _itemType = type;
   _effect = 0; // no effect
   
}


std::string Item::getItemType() const
{
	std::string itemType;
   if ( _itemType == Potion ) { return itemType  = "Potion"; }
   else { return itemType = "Gold"; }
   
}


void Item::tick() {} // do nothing

Item::~Item() {} //  do nothing

// END
// CLASS Potion

Potion::Potion( Kind potionType ) : Item( Item::Potion )
{
   _potionType = potionType;
   
   // setting appropriate effect value for potion object
   switch ( _potionType ) {
         case RH:
         _effect = 30;
         break;
         
         case BA: case BD:
         _effect = 10;
         break;
         
         case PH:
         _effect = -15;
         break;
         
         case WA: case WD:
         _effect = -5;
         break;
   }
}

void Potion::use( Character *player ) const
{
	int statMax;
   std::string playerType = player->getType(); // type of player
   
   // setting appropriate statMax value (depends on player type)
   if ( playerType == "Knight" ) statMax = 100;
   if ( playerType == "Wizard" ) statMax = 60;
   if ( playerType == "Samurai" ) statMax = 80;
   if ( playerType == "Goblin" ) statMax = 75;
   
   // applying the effect of consuming potion
   int newStat = player->getHealth() + _effect;
   switch ( _potionType ) {
   
      case RH: // Restore Health
         if (  newStat > statMax ){
            player->setHealth( statMax );
         }
         else {
            player->setHealth( newStat );
         }
         break;
         
      case BA: // Boost Attack
         player->setAttack( player->getAttack() + _effect );
         break;
      
      case BD: // Boost Defense
      	 player->setDefence( player->getDefence() + _effect );
         break;
      
      case PH: // Poison Health
         if ( newStat < statMin ) {
            player->setHealth( 0 );
         }
         else {
            player->setHealth( newStat );
         }
         break;
         
      case WA: // Wound Attack
         if ( (player->getAttack() + _effect) < statMin ) {
            player->setAttack( 0 );
         }
         else {
            player->setAttack( player->getAttack() + _effect );
         }
         break;
         
      case WD: // Wound Defense
         if ( (player->getDefence() + _effect) < statMin ) {
            player->setDefence( 0 );
         }
         else {
            player->setDefence( player->getDefence() + _effect );
         }
   }
    if ( playerType != "Goblin" ) {
      cout << "You chug the ";
      switch (_potionType){
        case RH:
          cout << "Restore Health";
          break;
        case BA:
          cout << "Boost Attack";
          break;
        case BD:
          cout << "Boost Defence";
          break;
        case PH:
          cout << "Poison Health";
          break;
        case WA:
          cout << "Wound Attack";
          break;
        case WD:
          cout << "Would Defence";
          break;
      }
      cout << " potion." << endl;
    }
}

// END
// CLASS Gold

Gold::Gold( Kind goldType ) : Item( Item::Gold )
{
   _goldType = goldType;
   
   if ( _goldType == GoldPile ) { _effect = 10; }
   else { _effect = 50; }
}


void Gold::use( Character *player) const
{
   Player *pp = (Player *)(player); //Downcast because GP is only relevant for players

   if ( _goldType == GoldPile ) {
      pp->setGP( pp->getGP() + _effect );
      cout << "You pick up the Gold Pile worth 10GP." << endl;
   }
   else {
      pp->setGP( pp->getGP() + _effect );
      cout << "You successfully take the Dragon Hoard!" << endl;
   }
}


std::string Gold::getGoldType()
{
	std::string goldType;
   if ( _goldType == GoldPile ) { return goldType = "GoldPile"; }
   else { return goldType = "DragonHoard"; }
}

