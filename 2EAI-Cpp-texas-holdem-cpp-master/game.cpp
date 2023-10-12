#include "game.h"

bool compareShowedCards( const ShowedCard & lhs, const ShowedCard & rhs )
{
	//std::cout << "Comp " << lhs.rank.toString() << " vs. " << rhs.rank.toString() << " = " << (lhs.rank < rhs.rank) << std::endl;
	return( lhs.rank < rhs.rank || ( lhs.rank == rhs.rank && lhs.player->getBet() > rhs.player->getBet() ) );
}

void Game::play( size_t winnerAmount )
{
	while( players.size() > winnerAmount )
	{
		plays++;
		std::cout << "Play " << plays << " (" << blind << ")" << std::endl << "--------" << std::endl << std::endl;
		if( plays % 32 == 0 ) //4 rounds with 8 players
		{
			blind *= 2; //double the blind ?
		}
		deck.shuffle();
		dealPocket();
#ifdef DEBUG_PRINT
		printGame();
#endif
		betting( true );
#ifdef DEBUG_PRINT
		printGame();
#endif
		if( amountHands == 1 )
		{
#ifdef FULL_PRINT
			printGame();
#endif
			showdown();
			cleanUp();
			continue;
		}
		flop();
		betting();
#ifdef DEBUG_PRINT
		printGame();
#endif
		if( amountHands == 1 )
		{
#ifdef FULL_PRINT
			printGame();
#endif
			showdown();
			cleanUp();
			continue;
		}
		turn();
		betting();
#ifdef DEBUG_PRINT
		printGame();
#endif
		if( amountHands == 1 )
		{
#ifdef FULL_PRINT
			printGame();
#endif
			showdown();
			cleanUp();
			continue;
		}
		river();
		betting();
#ifdef FULL_PRINT
		printGame();
#endif
		showdown();
		cleanUp();
	}
	printGame();
}

void Game::dealPocket( void )
{
	size_t i = ( dealer + 1 ) % players.size(); //first card is give to player left from dealer
	while( ! players[i]->hasHand() )
	{
		players[i]->addCard( deck.takeCard() );
		i = ( i + 1 ) % players.size(); //next player on the left in round robin
	}
	amountHands = players.size();
}

void Game::betting( bool isPreFlop )
{
	size_t i = ( dealer + 1 ) % players.size(); //first bet starts left from dealer
	if( isPreFlop )
	{
		highestBet = players[i]->raiseBet( blind ); //first small blind starts left from dealer
		i = ( i + 1 ) % players.size(); //next player on the left in round robin for big blind
		highestBet = std::max( highestBet, players[i]->raiseBet( blind * 2 ) );
		i = ( i + 1 ) % players.size();
	}
	char minimumPlayerCheck = players.size();
	int raisedBet;
	while( ( players[i]->getBet() < highestBet ) || ( amountHands > 1 && minimumPlayerCheck > 0 ) )
	{
		if( minimumPlayerCheck > 0 )
		{
			minimumPlayerCheck--;
		}
		if( players[i]->hasHand() && ! players[i]->isAllIn() )
		{
			//Player * temp = players[i];
			//raisedBet = temp->willYouRaise( highestBet );
#ifdef DEBUG_PRINT
			std::cout << "Will you raise ";
			players[i]->printID();
			std::cout << " ?" << std::endl << std::endl;
#endif
			raisedBet = players[i]->willYouRaise( highestBet );
#ifdef DEBUG_PRINT
			std::cout << std::endl;
			players[i]->printID();
			std::cout << " will do : " << raisedBet << std::endl;
#endif
			if( raisedBet < 0 )
			{
				if( players[i]->getBet() < highestBet )
				{
					players[i]->fold();
					amountHands--;
				}
			}
			else
			{
				highestBet = std::max( highestBet, players[i]->raiseBet( highestBet + raisedBet ) );
			}
		}
		i = ( i + 1 ) % players.size(); //next player on the left in round robin
	}
}

void Game::flop( void )
{
	deck.burnCard();
	table.addCard( deck.takeCard() );
	table.addCard( deck.takeCard() );
	table.addCard( deck.takeCard() );
}

void Game::turn( void )
{
	deck.burnCard();
	table.addCard( deck.takeCard() );
}

void Game::river( void )
{
	deck.burnCard();
	table.addCard( deck.takeCard() );
}

void Game::showdown( void )
{
	//Pay up
    std::vector<ShowedCard> showedCards;
	for( size_t i = 0 ; i < players.size() ; i++ )
	{
		if( players[i]->hasHand() )
		{
            showedCards.push_back( ShowedCard{ players[i], players[i]->getRank(), static_cast<unsigned char>( ( players.size() + i - 1 - dealer ) % players.size() ) } );
		}
	}
	if( amountHands != showedCards.size() )
	{
		std::cerr << "SOMETHING BAD HAPPEND, THE GAME IS RIGGED !!!" << std::endl;
		//system( "pause" );
	}
	if( showedCards.size() == 0 )
	{ //should NOT happen, but OK house wins :D
		std::cerr << "HOUSE WINS !!!" << std::endl;
		//system( "pause" );
	}
	else if( showedCards.size() == 1 )
	{ //Bluff !
		unsigned int pot = 0;
		for( size_t i = 0 ; i < players.size() ; i++ )
		{
			pot += players[i]->payUp( highestBet );
		}
		showedCards.back().player->win( pot );
#ifdef FULL_PRINT
		showedCards.back().player->printID();
		std::cout << " bluffed the others out and receives " << pot << std::endl;
#endif
	}
	else
	{
		std::sort( showedCards.begin(), showedCards.end(), compareShowedCards );
#ifdef FULL_PRINT
		std::cout << showedCards.size() << " players in showdown ! " << std::endl;
#endif
#ifdef DEBUG_PRINT
		for( size_t i = 1 ; i <= showedCards.size() ; i++ )
		{
			showedCards[showedCards.size()-i].player->printID();
			showedCards[showedCards.size()-i].rank.print();
			std::cout << std::endl;
		}
#endif
		while( highestBet > 0 && showedCards.size() > 0 )
		{
			unsigned int pot = 0;
			unsigned int bet = showedCards.back().player->getBet();
#ifdef DEBUG_PRINT
			std::cout << "Take bet " << showedCards.back().player->getBet() << " of ";
			showedCards.back().player->printID();
			std::cout << std::endl;
#endif
			highestBet -= bet;
			size_t i;
			for( i = 0 ; i < players.size() ; i++ )
			{
				pot += players[i]->payUp( bet );
			}
			for( i = 2 ; i <= showedCards.size() && ( showedCards[showedCards.size()-i].rank == showedCards.back().rank ) ; i++ )
				; //i will be the amount of showedCards that will need to split the pot
			i--; //we found the one that was not comform
			if( i > 1 )
			{
#ifdef FULL_PRINT
				std::cout << i << " players need to split the pot of " << pot << std::endl;
#endif
				unsigned int rest = pot % i;
				pot /= i;
				while( i > 0 )
				{
					showedCards[showedCards.size()-i].player->win( pot );
#ifdef FULL_PRINT
					showedCards[showedCards.size()-i].player->printID();
					std::cout << " wins " << pot << " with ";
					showedCards[showedCards.size()-i].rank.print();
#endif
					i--;
				}
				if( rest > 0 )
				{
					unsigned char placeAfterDealer = players.size();
					Player * luckyBastard = nullptr;
					for( i = 1 ; i <= showedCards.size() && ( showedCards[showedCards.size()-i].rank == showedCards.back().rank ) ; i++ )
					{
						if( showedCards[showedCards.size()-i].placeAfterDealer < placeAfterDealer )
						{
							luckyBastard = showedCards[showedCards.size()-i].player;
							placeAfterDealer = showedCards[showedCards.size()-i].placeAfterDealer;
						}
					}
					if( luckyBastard != nullptr )
					{
						luckyBastard->win( rest );
#ifdef FULL_PRINT
						luckyBastard->printID();
						std::cout << " is a lucky bastard and gets the rest (" << rest << ") of the split pot" << std::endl;
#endif
					} //if not found (what should not happen) house wins !
				}
			}
			else
			{
				showedCards.back().player->win( pot );
#ifdef FULL_PRINT
					showedCards.back().player->printID();
					std::cout << " has highest hand and wins " << pot << " with ";
					showedCards.back().rank.print();
#endif
			}
			while( showedCards.size() > 0 && showedCards.back().player->getBet() == 0 )
			{
				showedCards.pop_back();
			}
		}
	}
}

void Game::cleanUp( void )
{
	//Clean up
	highestBet = 0;
	table.clear();
	for( size_t i = 0 ; i < players.size() ; i++ )
	{
		players[i]->clear();
		if( players[i]->isElimitated() )
		{
			if( i < dealer )
			{
				dealer--;
			}
			players.erase( players.begin() + i );
			i--;
		}
	}
	dealer = ( dealer + 1 ) % players.size();
}

void Game::printGame( void )
{
	std::cout << "Table ";
	table.print();
	std::cout << std::endl << std::endl;
	for( size_t i = 0 ; i < players.size() ; i++ )
	{
		if( dealer == i )
		{
			std::cout << "o ";
		}
		else
		{
			std::cout << "  ";
		}
		std::cout << "Player " << i+1 << " : ";
		players[i]->print();
	}
	std::cout << std::endl;
}
