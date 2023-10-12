#include "king_of_aces.h"
#include "game.h"

namespace PXL2022
{

const char * KingOfAces::getName() const
{
	return "King of Aces";
}


int KingOfAces::willYouRaise( unsigned int totalBet )
{
	PokerRank mijnHandRank = getRank();
	if( getTable()->isPreFlop() ) //Pre flop
	{
		if( getHand().getFirstCard()->getSuit() == getHand().getSecondCard()->getSuit() )
		{ //suited hands
			int diffBetweenOrderedCards = mijnHandRank.getHand().at(0) - mijnHandRank.getHand().at(1);
			if( diffBetweenOrderedCards == 1 || diffBetweenOrderedCards == 13 )
			{ //suited connected
				return ( getGame()->getBlind() * 7 ) - totalBet; //max 7x blind and fold otherwise
			}
			return ( getGame()->getBlind() * 3 ) - totalBet; //max 3x blind and fold otherwise
		}
		if( getHand().getFirstCard()->getRank() == getHand().getSecondCard()->getRank() )
		{ //1 pair to start
			return ( getGame()->getBlind() * 5 ) - totalBet; //max 5x blind and fold otherwise
		}
		int diffBetweenOrderedCards = mijnHandRank.getHand().at(0) - mijnHandRank.getHand().at(1);
		if( diffBetweenOrderedCards == 1 || diffBetweenOrderedCards == 13 )
		{ //unsuited connected
			return ( getGame()->getBlind() * 3 ) - totalBet; //max 3x blind and fold otherwise
		}
		if( totalBet > getGame()->getBlind() * 2 )
		{
			return -1;
		}
		return 0;
	}
	else if( getTable()->isFlop() )
	{ //on flop
		talkSmack();
		return 0;
	}
	else if( getTable()->isTurn() )
	{ //on turn
		return 0;
	}
	else
	{ //on river
		return 0;
	}
}

void KingOfAces::talkSmack()
{
	std::cout << "You are all losers !!!" << 42 << std::endl;
}
}
