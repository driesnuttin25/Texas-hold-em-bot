#include "tournament.h"

Tournament::~Tournament()
{
	if( !isCopy ) //Do not clear players in a copy !
	{
		for( std::vector<Player *>::iterator it = players.begin() ; it != players.end() ; ++it )
		{
			delete( *it );
		}
		players.clear();
	}
	for( std::vector<std::vector<Game*>>::iterator it = gameRounds.begin() ; it != gameRounds.end() ; ++it )
	{
		for( std::vector<Game*>::iterator itsit = it->begin() ; itsit != it->end() ; ++itsit )
		{
			delete( *itsit );
		}
		it->clear();
	}
	gameRounds.clear();
}

const Player * Tournament::start( void )
{
	//srand( (unsigned int) time( NULL ) );
	for( std::vector<Player *>::iterator it = players.begin() ; it != players.end() ; ++it )
	{
		(*it)->chips = STARTER_CHIPS;
	}
    initiatePools();
	unsigned int highestBlind = 0;
	while( gameRounds[round].size() > 1 )
	{
		std::cout << " ROUND " << round + 1 << std::endl << "---------" << std::endl << std::endl;
		for( unsigned char game = 0 ; game < gameRounds[round].size() ; game++ )
		{
			gameRounds[round][game]->printGame();
		}
//		system( "pause" );
		std::cout << "! START !" << std::endl << "---------" << std::endl << std::endl;
		for( unsigned char game = 0 ; game < gameRounds[round].size() ; game++ )
		{
			gameRounds[round][game]->play();
			highestBlind = std::max( highestBlind, gameRounds[round][game]->getBlind() );
		}
//		system( "pause" );
		gameRounds.push_back( std::vector<Game*>() );
		/* 6-10
		 8  8  8  = 24
		 8  8  6  = 22
		 8  6  6  = 20
		 -------------
		 8 10     = 18 --> 4 5  = 9
		 8  8     = 16 --> 4 4  = 8
		 8  6     = 14 --> 4 3  = 7
		 6  6     = 12 --> 3 3  = 6
		*/
		Game * newGame = new Game( highestBlind );
		for( unsigned char game = 0 ; game < gameRounds[round].size() ; game++ )
		{
			if( game != 0 && ( ( game % 4 == 0 && ( gameRounds[round].size() - game ) != 1 ) || ( game % 4 == 3 && ( gameRounds[round].size() - game ) == 3 ) ) )
			{
				gameRounds[round+1].push_back( newGame );
				newGame = new Game( highestBlind );
			}
			for( unsigned char i = 0 ; i < gameRounds[round][game]->getPlayers().size() ; i++ )
			{
				newGame->addPlayer( gameRounds[round][game]->getPlayers().at( i ) );
			}
		}
		gameRounds[round+1].push_back( newGame );
		round++;
	}
	std::cout << " FINAL ROUND " << std::endl << "---------" << std::endl << std::endl;
	gameRounds[round].front()->printGame();
//	system( "pause" );
	std::cout << "! START !" << std::endl << "---------" << std::endl << std::endl;
	gameRounds[round].front()->play( 1 );
	std::cout << "The ultimate winner = ";
	gameRounds[round].front()->getPlayers().front()->printID();
	std::cout << std::endl << "With " << gameRounds[round].front()->getPlayers().front()->getChips() << " chips !" << std::endl << "ID = " << (unsigned int) gameRounds[round].front()->getPlayers().front()->getID() << std::endl << std::endl;
//	system( "pause" );
	return( gameRounds[round].front()->getPlayers().front() );
}

inline void Tournament::initiatePools( void )
{
	gameRounds.push_back( std::vector<Game*>() );
	while( players.size() > 0 )
	{ // make groups of 8 players
		unsigned char groupSize = 8;
		if( players.size() < 20 ) //the last group will be smaller than 6 !
		{
			/* 6-10
			 8  8  8  = 24
			 8  8  7  = 23
			 8  8  6  = 22
			 8  7  6  = 21
			 8  6  6  = 20
			 -------------
			 9 10     = 19
			 8 10     = 18
			 8  9     = 17
			 8  8     = 16
			 8  7     = 15
			 8  6     = 14
			 7  6     = 13
			 6  6     = 12
			*/

			/* 7-9
			 8 9 9 9  = 33
			 8 8 9 9  = 34
			 8 8 8 9  = 33
			 8 8 8 8  = 32
			 8 8 8 7  = 31
			 8 8 7 7  = 30
			 8 7 7 7  = 29
			 7 7 7 7  = 28
			 9 9 9    = 27
			 8 9 9    = 26
			 8 8 9    = 25
			 8 8 8    = 24
			 8 8 7    = 23
			 8 7 7    = 22
			 7 7 7    = 21
			*/

			switch( players.size() )
			{
			case 19:
				groupSize = 9;
				break;
			case 18:
			case 17:
			case 16:
			case 15:
			case 14:
				groupSize = 8;
				break;
			case 13:
				groupSize = 7;
				break;
			case 12:
				groupSize = 6;
				break;
			case 11:
				std::cerr << "TOURNAMENT IS UNFAIR, 11 players should not have happend, added players while tournament already started ?" << std::endl;
				// fall through
			default:
				groupSize = players.size();
			}
		}
		Game * newGame = new Game();
		for( unsigned char i = 0 ; i < groupSize ; i++ ) // everything between 10-6 is ok
		{
			size_t randomIndex = rand() % players.size();
			newGame->addPlayer( players[randomIndex] );
			players.erase( players.begin() + randomIndex );
		}
		gameRounds.front().push_back( newGame );
	}
}
