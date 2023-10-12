#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "player.h"
#include "game.h"

#define STARTER_CHIPS 128

//A Texas hold'em Shoot-out !
class Tournament
{
public:
	Tournament()													: round(0), isCopy(false)	{}
	Tournament( const Tournament & copy )							: players(copy.players), round(0), isCopy(true)	{} //Only copy players !
	~Tournament();

	void registerPlayer( Player * newPlayer, PlayerID ID = CUSTOM )	{ newPlayer->setID( ID ); players.push_back( newPlayer ); }//newPlayer->chips = STARTER_CHIPS; }
    const Player * start( void );

	std::vector<Player *> getPlayers() const						{ return( players ); }
	unsigned char getRound( void ) const							{ return( round ); }
	unsigned char getAmountOfTables( void ) const					{ return( gameRounds[round].size() ); }

private:
	inline void initiatePools( void );

	std::vector<Player *> players;
	std::vector<std::vector<Game*>> gameRounds;
	unsigned char round;
	bool isCopy;

};

#endif // TOURNAMENT_H
