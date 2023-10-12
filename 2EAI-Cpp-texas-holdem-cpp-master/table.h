#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include "card.h"

class Table
{
public:
	#if _MSC_VER
	#pragma warning( suppress : 4351 ) // Suppress warning 4351 only once. For MSVC...
	#endif
	Table()												{}
	~Table()											{}

	void addCard( const Card * newCard )				{ communityCards.push_back( newCard ); }
	void clear( void )									{ communityCards.clear(); }
	bool isPreFlop( void ) const						{ return( communityCards.size() == 0 ); }
	bool isFlop( void ) const							{ return( communityCards.size() == 3 ); }
	bool isTurn( void ) const							{ return( communityCards.size() == 4 ); }
	bool isRiver( void ) const							{ return( communityCards.size() == 5 ); }

	std::vector<const Card *> getCommunityCards() const	{ return( communityCards ); }
	void print( void ) const;

private:
	std::vector<const Card *> communityCards;
};

#endif // TABLE_H
