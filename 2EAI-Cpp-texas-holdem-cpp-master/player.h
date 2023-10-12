#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "hand.h"
#include "playerid.h"

class Game;

class Player
{
	friend class Tournament;
	friend class Game;

public:
	Player( unsigned char instance = 0 )				: instance(instance), chips(128), bet(0)	{} //, gameWins(0), tournamentWins(0)	{}
	virtual ~Player()									{}

    virtual const char* getName( void ) const			= 0;//{ return "Noob"; } //going abstract !
	unsigned char getInstance( void ) const				{ return instance; }
    void printID( void ) const							{ std::cout << this->getName() << "(" << (short) this->getInstance() << ")"; }

    bool isElimitated( void ) const						{ return( chips == 0 ); }
    //bool hasFolded( void ) const						{ return( !hand.filled() ); }
    bool hasHand( void ) const							{ return( hand.filled() ); }
	bool isAllIn( void ) const							{ return( chips == bet ); }
    unsigned int getChips() const						{ return chips; }
	unsigned int getBet() const							{ return bet; }
	PokerRank getCommunityRank( void ) const			{ return( hand.getCommunityRank() ); } //This is public, because everybody can see the table !

	PlayerID getID() const								{ return( ID ); }
//	unsigned int getGameWins() const					{ return gameWins; }
//	unsigned int getTournamentWins() const				{ return tournamentWins; }

protected:
	PokerRank getRank( void ) const						{ return( hand.getMyRank() ); } //This is protected, because only the player should see this
	virtual int willYouRaise( unsigned int totalBet )	= 0;//{ return 0; }

	const unsigned char instance;
	const Hand getHand( void ) const					{ return( hand ); }
	const Table * getTable( void ) const				{ return( hand.getTable() ); }
	const Game * getGame( void ) const					{ return( game ); }

private:
	void setID( const PlayerID ID )						{ this->ID = ID; }
	void addCard( const Card * newCard )				{ hand.addCard( newCard ); }
	void showHand( void ) const							{ hand.print(); }
	void clear( void )									{ hand.clear(); bet = 0; }
	void fold( void )									{ hand.clear(); }
	unsigned int payUp( unsigned int debts )			{ debts = std::min( debts, bet ); chips -= debts; bet -= debts; return( debts ); }
	void win( unsigned int gain )						{ chips += gain; }
	unsigned int raiseBet( unsigned int totalBet )		{ bet = std::min( totalBet, chips ); return( bet ); } //Check if raise is possible ? Just take the minimum value !
	void sitAtTable( const Table * table )				{ hand.setTable( table ); }
	void joinGame( const Game * newGame )				{ game = newGame; }
	void print( void )									{ this->printID(); std::cout << std::endl << "[" << chips << "] --> " << bet << std::endl; showHand(); std::cout << std::endl; }
//	void wonGame( void )								{ gameWins++; }
//	void wonTournament( void )							{ tournamentWins++; }

	Hand hand;
	const Game * game;
	unsigned int chips;
	unsigned int bet;
	PlayerID ID;
//	unsigned int gameWins;
//	unsigned int tournamentWins;
};

#endif // PLAYER_H
