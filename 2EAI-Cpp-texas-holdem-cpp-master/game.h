#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "player.h"
#include "table.h"
#include "deck.h"

struct ShowedCard
{
	Player * player;
	PokerRank rank;
	unsigned char placeAfterDealer;
};

class Game
{
	friend class Tournament;

public:
	Game( int blind = 1 )										: dealer(0), blind(blind), highestBet(0), amountHands(0), plays(0) {}

	unsigned int getPlays( void ) const							{ return( plays ); }
	void printGame( void );

	std::vector<Player *> getPlayers( void ) const				{ return( players ); }
	unsigned int getBlind( void ) const							{ return( blind ); }
	unsigned int getHighestBet( void ) const					{ return( highestBet ); }
	unsigned char getAmountHands( void ) const					{ return( amountHands ); }
	const Player * getDealer( void ) const						{ return( players.at( dealer ) ); }
	size_t getDealerLocation( void ) const						{ return( dealer ); }
	size_t getPlayerLocation( const Player * player ) const		{ for( size_t i = 0 ; i < players.size() ; i++ ) if( players.at(i) == player) return( i ); return( -1 ); }
	size_t getDistanceToDealer( const Player * player ) const	{ return( ( players.size() + ( getPlayerLocation( player ) - dealer ) ) % players.size() ); }
	const Table * getTable( void ) const						{ return( &table ); }

private:
	void addPlayer( Player * newPlayer )						{ newPlayer->joinGame( this ); newPlayer->sitAtTable( &table ); players.push_back( newPlayer ); }

	void play( size_t winnerAmount = 2 );
	void dealPocket( void );
	void betting( bool isPreFlop = false );
	void flop( void );
	void turn( void );
	void river( void );
	void showdown( void );
	void cleanUp( void );

	size_t dealer;
	unsigned int blind;
	unsigned int highestBet;
	unsigned char amountHands;
	std::vector<Player *> players;
	Deck deck;
	Table table;
	unsigned int plays;
};

#endif // GAME_H
