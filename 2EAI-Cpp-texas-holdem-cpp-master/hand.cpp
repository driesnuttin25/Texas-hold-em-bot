#include "hand.h"

PokerRank Hand::getMyRank( void ) const
{
	if( cards[0] == nullptr )
	{
		return( PokerRank() );
	}
	else if( cards[1] == nullptr )
	{
		return( PokerRank( cards[0] ) );
	}
	else if( table == nullptr || table->getCommunityCards().empty() )
	{
		return( PokerRank( cards[0], cards[1] ) );
	}
	else
	{
		std::vector<const Card *> combo = table->getCommunityCards();
		combo.push_back( cards[0] );
		combo.push_back( cards[1] );
		return( PokerRank( combo ) );
	}
}

PokerRank Hand::getCommunityRank( void ) const
{
	if( table == nullptr || table->getCommunityCards().empty() )
	{
		return( PokerRank() );
	}
	return( PokerRank( table->getCommunityCards() ) );
}

void Hand::print( void ) const
{
	if( cards[0] == nullptr )
	{
		std::cout << "Empty hand";
	}
	else if( cards[1] == nullptr )
	{
		std::cout << cards[0];
	}
	else
	{
		std::cout << cards[0] << cards[1];
	}
	std::cout << std::endl;
}
