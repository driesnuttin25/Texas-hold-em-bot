#ifndef HAND_H
#define HAND_H

#include <iostream>
#include "card.h"
#include "table.h"
#include "poker_rank.h"

class Hand
{
public:

	#if _MSC_VER
	#pragma warning( suppress : 4351 ) // Suppress warning 4351 only once. For MSVC...
	#endif
	Hand()										: cards(), table(nullptr)	{}

	void addCard( const Card * newCard )		{ cards[(cards[0] == nullptr)?0:1] = newCard; }
	void clear( void )							{ cards[0] = nullptr; cards[1] = nullptr; }

	bool filled( void ) const					{ return( ( cards[0] != nullptr ) && ( cards[1] != nullptr ) ); }
	PokerRank getMyRank( void ) const;
	PokerRank getCommunityRank( void ) const;
	void print( void ) const;

	const Card * getFirstCard( void ) const		{ return( cards[0] ); }
	const Card * getSecondCard( void ) const	{ return( cards[1] ); }

	void setTable( const Table * table )		{ this->table = table; }
	const Table * getTable() const				{ return( table ); }

private:
	const Card * cards[2];
	const Table * table;
};

#endif // HAND_H
