#ifndef SHOOTOUT_H
#define SHOOTOUT_H

#include <iostream>
#include <map>

#include "tournament.h"

void runStatistics( Tournament & tournament, int amountOfTournaments )
{
	std::map<const Player *, int> wins;
	for( size_t i = 0 ; i < tournament.getPlayers().size() ; i++ )
	{
		Player * player = tournament.getPlayers().at( i );
		wins[player] = 0;
	}
//	int ** wins = new int[amountOfPlayers][4];
//	for( int i = 0 ; i < amountOfPlayers ; i++ )
//	{
//		for( int j = 0 ; j < 4 ; j++ )
//		{
//			wins[i][j] = 0;
//		}
//	}

	for( int i = 0 ; i < amountOfTournaments ; i++ )
	{
		Tournament tour( tournament ); //using copy constructor !
		//PXL2019::registerPlayers( tour );
        const Player * winnaar = tour.start();
		//wins[winnaar->getID()-PXL_2019][winnaar->getInstance()]++;
		wins[winnaar]++;

		std::cerr << i << " The ultimate winner = ";
		std::cerr << winnaar->getName() << "(" << (short) winnaar->getInstance() << ")";
		std::cerr << std::endl << "With " << winnaar->getChips() << " chips !" << std::endl << "ID = " << (unsigned int) winnaar->getID() << std::endl << std::endl;
	}

	for( int id = CUSTOM ; id < MAX_PLAYER_ID ; id++ )
	{
		int sum = 0;
		for( std::map<const Player *, int>::iterator it = wins.begin() ; it != wins.end() ; ++it )
		{
			const Player * player = it->first;
			if( player->getID() == id )
			{
				std::cerr << "ID: " << player->getID() << ", Name: " << player->getName() << ", Instance: " << (short) player->getInstance() << ", Wins: " << it->second << std::endl;
				sum += it->second;
			}
		}
		if( sum > 0 )
		{
			std::cerr << "ID: " << id << ", Total Wins: " << sum << std::endl;
		}
	}

//	for( int i = 0 ; i < PXL_2020-PXL_2019 ; i++ )
//	{
//		int sum = 0;
//		for( int j = 0 ; j < 4 ; j++ )
//		{
//			std::cerr << "ID: " << ( i + PXL_2019 ) << ", Instance: " << j << ", Wins: " << wins[i][j] << std::endl;
//			sum += wins[i][j];
//		}
//		std::cerr << "ID: " << ( i + PXL_2019 ) << ", Total Wins: " << sum << std::endl;
//	}
}

#endif // SHOOTOUT_H
