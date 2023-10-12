#ifndef POKER_RANK_H
#define POKER_RANK_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "card.h"

typedef char byte;

enum POKER_HAND
{
	HIGH_CARD = 0,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH
};

class PokerRank
{
public:
	PokerRank()											: category(HIGH_CARD), rank(0)					{}
	PokerRank( const Card * card )						: category(HIGH_CARD), rank(card->getRank())	{ hand.push_back( card ); }
	PokerRank( const Card * card1, const Card * card2 )	;
	PokerRank( POKER_HAND category, Card::RANK rank )	: category(category), rank(rank)				{}
	PokerRank( const std::vector<const Card *> combo );

	~PokerRank()										{}

	inline std::string toString( void ) const; //inline to speed up the translation (it is a small function)
	void print( void ) const							{ printRank(); printHand(); std::cout << std::endl << std::endl; }
	void printRank( void ) const						{ std::cout << toString() << std::endl; }
	void printHand( void ) const						{ for( size_t i = 0 ; i < hand.size() ; i++ ) { std::cout << hand.at( i ); } }

	POKER_HAND getCategory( void ) const				{ return( category ); }
	Card::RANK getRank( void ) const					{ return( rank ); }
	std::vector<const Card *> getHand( void ) const		{ return( hand ); } //TO CHECK : const pointer ???
	int handValue( void ) const;//4 bits for a card, 5 bytes is enough !

	bool operator > ( const PokerRank & other ) const	{ return( category > other.category || ( category == other.category && ( rank > other.rank || ( rank == other.rank && handValue() > other.handValue() ) ) ) ); }
	bool operator < ( const PokerRank & other ) const	{ return( category < other.category || ( category == other.category && ( rank < other.rank || ( rank == other.rank && handValue() < other.handValue() ) ) ) ); }
	bool operator == ( const PokerRank & other ) const	{ return( category == other.category && rank == other.rank && handValue() == other.handValue() ); }

private:
	POKER_HAND category;
	Card::RANK rank;
	std::vector<const Card *> hand;

};

inline std::string PokerRank::toString( void ) const
{
	std::string returnValue;
	switch( category )
	{
	case HIGH_CARD:
		returnValue = "High card ";
		break;
	case ONE_PAIR:
		returnValue = "One pair of ";
		break;
	case TWO_PAIR:
		returnValue = "Two pair with ";
		break;
	case THREE_OF_A_KIND:
		returnValue = "Three of a kind of ";
		break;
	case STRAIGHT:
		returnValue = "Straight from ";
		break;
	case FLUSH:
		returnValue = "Flush with ";
		break;
	case FULL_HOUSE:
		returnValue = "Full house with ";
		break;
	case FOUR_OF_A_KIND:
		returnValue = "Four of a kind of ";
		break;
	case STRAIGHT_FLUSH:
		returnValue = "Straight flush from ";
		break;
	}
	returnValue.append( std::to_string( rank ) );
	return( returnValue );
}

#endif // POKER_RANK_H
