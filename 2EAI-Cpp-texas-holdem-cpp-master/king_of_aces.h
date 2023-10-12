#ifndef KING_OF_ACES_H
#define KING_OF_ACES_H

#include "player.h"

namespace PXL2022
{

class KingOfAces : public Player //KingOfAces "is" a Player
{
public: //Everything below this line is publicly accessible
	KingOfAces( unsigned char instance = 0 ) : Player(instance) {} //Constructor with parameter "instance" (i.e. 0->3) and forward to Player constructor
	const char* getName( void ) const; //Overwrite this pure virtual function to define the name for printing
	int willYouRaise( unsigned int totalBet ); //Overwrite the pure virtual function to define the way of playing
private:
	bool checked = true;
	void talkSmack();

};

}
#endif // KING_OF_ACES_H
