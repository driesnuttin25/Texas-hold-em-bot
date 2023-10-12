#ifndef BART_H
#define BART_H

#include "player.h"

namespace PXL2022
{
class Bart : public Player //Bart is Player
{
public:
	Bart( unsigned char instance = 0 )	: Player(instance)	{}
	const char* getName( void ) const						; //{ return "Bart"; }
	int willYouRaise( unsigned int totalBet )				;
private:
	bool bluff();
	int betSafely( int amount );
	bool stopRaising = false;
	static int amountOfWin;
};
}
#endif // BART_H
