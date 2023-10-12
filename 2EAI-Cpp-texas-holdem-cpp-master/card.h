#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

class Card
{
public:

	enum SUIT
	{
		SPADES = 0x6,
		DIAMONDS = 0x4,
		CLUBS = 0x5,
		HEARTS = 0x3
	};
	typedef unsigned char RANK;

	Card( SUIT suit, RANK rank )						: suit(suit), rank(rank)	{}
	~Card()												{}

	SUIT getSuit() const								{ return suit; }
	RANK getRank() const								{ return rank; }

	void print( void )									{ std::cout << *this; }

	bool operator < ( const Card & other ) const		{ return( rank < other.rank ); }
	bool operator < ( const Card * const other ) const	{ return( rank < other->rank ); }
	bool operator <= ( const Card & other ) const		{ return( rank <= other.rank ); }
	bool operator <= ( const Card * const other ) const	{ return( rank <= other->rank ); }
	bool operator > ( const Card & other ) const		{ return( rank > other.rank ); }
	bool operator > ( const Card * const other ) const	{ return( rank > other->rank ); }
	bool operator >= ( const Card & other ) const		{ return( rank >= other.rank ); }
	bool operator >= ( const Card * const other ) const	{ return( rank >= other->rank ); }
	bool operator == ( const Card & other ) const		{ return( ( rank == other.rank ) && ( suit == other.suit ) ); }//not for pointers, normally a card should be unique !
	bool operator == ( const Card * const other ) const	{ return( ( rank == other->rank ) && ( suit == other->suit ) ); }
	bool operator != ( const Card & other ) const		{ return( ( rank != other.rank ) || ( suit != other.suit ) ); }//not for pointers, normally a card should be unique !
	bool operator != ( const Card * const other ) const	{ return( ( rank != other->rank ) || ( suit != other->suit ) ); }

	std::string toString( void ) const
	{
		std::string returnValue;
		returnValue.append( 1, (char) suit );
		switch( rank )
		{
		case 1:
		case 14:
			returnValue.append( " A" );
			break;
		case 11:
			returnValue.append( " J" );
			break;
		case 12:
			returnValue.append( " Q" );
			break;
		case 13:
			returnValue.append( " K" );
			break;
		case 10:
			returnValue.append( std::to_string( rank ) );
			break;
		default:
			returnValue.append( 1, ' ' );
			returnValue.append( std::to_string( rank ) );
			break;
		}
		return( returnValue );
	}

	friend std::ostream & operator<< ( std::ostream &output, const Card & card )
	{
		output << " " << (char) card.suit;
		switch( card.rank )
		{
		case 1:
		case 14:
			output << " A";
			break;
		case 11:
			output << " J";
			break;
		case 12:
			output << " Q";
			break;
		case 13:
			output << " K";
			break;
		default:
			output.width( 2 );
			output << (short) card.rank;
			break;
		}
		return( output );
	}

	friend std::ostream & operator<< ( std::ostream &output, const Card * const card )
	{
		output << *card;
		return( output );
	}

private:
	const SUIT suit;
	const RANK rank;
};

#endif // CARD_H
