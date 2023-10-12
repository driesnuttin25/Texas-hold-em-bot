#include "deck.h"

Deck::Deck()
{
	//srand( (unsigned int) time( NULL ) );
	unsigned char i;
	for( i = 1 ; i <= 13 ; i++ )
	{
		deck.push_back( new Card( Card::SPADES, i ) );
	}
	for( i = 1 ; i <= 13 ; i++ )
	{
		deck.push_back( new Card( Card::DIAMONDS, i ) );
	}
	for( i = 1 ; i <= 13 ; i++ )
	{
		deck.push_back( new Card( Card::CLUBS, i ) );
	}
	for( i = 1 ; i <= 13 ; i++ )
	{
		deck.push_back( new Card( Card::HEARTS, i ) );
	}
	reset();
}

Deck::~Deck()
{
	iterator = deck.begin();
	const Card * card;
	while( iterator != deck.end() )
	{
		card = *iterator;
		iterator = deck.erase( iterator );
		delete card;
	}
}

void Deck::shuffle( void )
{
	cut( 13 );
	char shuffles = ( 16 + rand() ) % 64; //minimum 16 shuffles, maximum 64
	for( char i = 0 ; i < shuffles ; i++ )
	{
		switch( rand() % 3 )
		{
		case 0:
			cut();
			break;
		case 1:
			riffle();
			break;
		case 2:
			overhand();
			break;
		}
		//printAll();
		//std::cout << std::endl;
	}
	reset();
}

void Deck::printRest( void )
{
	std::list<const Card *>::iterator it = iterator;
	while( it != deck.end() )
	{
		std::cout << *it;
		++it;
	}
	std::cout << std::endl;
}

void Deck::printAll( void )
{
	for( std::list<const Card *>::iterator it = deck.begin() ; it != deck.end() ; ++it ) //NOTE : iterator is redeclaired and will not effect the member variable
	{
		if( it == iterator )
		{
			std::cout << "[" << *it << "]";
		}
		else
		{
			std::cout << *it;
		}
	}
	std::cout << std::endl;
}

inline void Deck::cut( char amount ) //can only be place in cpp IF it is only used there ! (must be private than)
{
	if( amount == 0 )
	{
		amount = ( 1 + rand() ) % 52;
	}
	for( char i = 0 ; i < amount ; i++ )
	{
		deck.push_back( deck.front() );
		deck.pop_front();
	}
}

inline void Deck::riffle( void )
{
	std::list<const Card *>::iterator it = deck.begin();
	std::advance( it, ( deck.size() / 2 ) );
	std::list<const Card *> firstHalf( deck.begin(), it );
	std::list<const Card *> lastHalf( it, deck.end() );
	deck.clear();
	while( firstHalf.size() + lastHalf.size() > 0 )
	{
		if( firstHalf.size() > 0 && ( rand() % 2 ) == 0 )
		{
			deck.push_back( firstHalf.front() );
			firstHalf.pop_front();
		}
		else if( lastHalf.size() > 0 )
		{
			deck.push_back( lastHalf.front() );
			lastHalf.pop_front();
		}
	}
	iterator = deck.begin();
}

inline void Deck::overhand( void )
{
	std::list<const Card *> newDeck;
	std::list<const Card *>::iterator it = deck.begin();
	while( it != deck.end() )
	{
		size_t offset = ( 1 + rand() ) % 32;
		if( deck.size() > offset )
		{
			std::advance( it, offset );
		}
		else
		{
			it = deck.end();
		}
		newDeck.splice( newDeck.begin(), deck, deck.begin(), it );
	}
	deck = newDeck;
	iterator = deck.begin();
}
