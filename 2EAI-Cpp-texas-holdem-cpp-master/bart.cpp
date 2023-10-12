#include "bart.h"
#include "game.h"

namespace PXL2022
{

int Bart::amountOfWin = 0;

const char * Bart::getName( void ) const
{
	if( getChips() > 128 )
	{
		return "Almighty Deus";
	}
	if( getChips() < getGame()->getBlind() * 10 )
	{
		return "Jezus Christ !";
	}
	return "Bart";
}

int Bart::willYouRaise( unsigned int totalBet )
{
	static int num = 0;
	std::cout << amountOfWin++ << " & " << num++ << std::endl;
	switch( instance )
	{
	case 0:
	case 1:
		if( getGame()->getDistanceToDealer( this ) < 3 )
			return betSafely( 0 );
		if( !stopRaising && bluff() )
			return betSafely( 20 );
		return betSafely( getGame()->getBlind() * 2 );
		break;
	case 2:
	case 3:
		if( getGame()->getDealer()->getID() == PXL_2021 && getGame()->getBlind() > 2 )
			return( -1 );
		return( 0 );
		break;
	}
	return( 0 );
}

bool Bart::bluff()
{
	return( ( rand() % 10 ) == 0 );
}

int Bart::betSafely( int amount )
{
	if( amount < getGame()->getHighestBet() )
	{
		if( getGame()->getHighestBet() - amount > ( (2+instance) * getGame()->getBlind() ) )
		{
			return -1;
		}
		else
		{
			stopRaising = true;
			return 0;
		}
	}
	stopRaising = false;
	return( amount - getGame()->getHighestBet() );
}

}
