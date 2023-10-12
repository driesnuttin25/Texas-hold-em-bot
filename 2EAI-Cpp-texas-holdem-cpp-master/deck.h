#ifndef DECK_H
#define DECK_H

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <list>
#include "card.h"

class Deck
{
public:
	Deck();
	~Deck();

	void shuffle( void );
	void reset( void )				{ iterator = deck.begin(); }

	const Card* takeCard( void )	{ const Card* returnValue = *iterator; ++iterator; return( returnValue ); }
	void burnCard( void )			{ ++iterator; }

	void printRest( void );
	void printAll( void );

private:
	inline void cut( char amount = 0 );
	inline void riffle( void );
	inline void overhand( void );

	std::list<const Card *> deck;
	std::list<const Card *>::iterator iterator;
};

#endif // DECK_H
