#include "poker_rank.h"

bool compareCardPointers( const Card * const lhs, const Card * const rhs )
{
	return( *lhs < *rhs );
}

int PokerRank::handValue( void ) const
{
	int returnValue = 0;
	for( size_t i = 0 ; i < hand.size() ; i++ )
	{
		if( hand.at( i )->getRank() == 1 )
		{
			returnValue |= 14;
		}
		else
		{
			returnValue |= hand.at( i )->getRank();
		}
		returnValue <<= 4;//4 bits for a card, 5 bytes is enough !
	}
	return( returnValue );
}

PokerRank::PokerRank( const Card * card1, const Card * card2 )
	: category(HIGH_CARD), rank(0)
{
	if( card1->getRank() == card2->getRank() )
	{
		category = ONE_PAIR;
	}
	if( card1->getRank() == 1 || card2->getRank() == 1 )
	{
		rank = 14; //Ace
		if( card1->getRank() <= card2->getRank() ) //Ace is smallest
		{
			hand.push_back( card1 );
			hand.push_back( card2 );
		}
		else
		{
			hand.push_back( card2 );
			hand.push_back( card1 );
		}
	}
	else if( card1->getRank() >= card2->getRank() )
	{
		rank = card1->getRank();
		hand.push_back( card1 );
		hand.push_back( card2 );
	}
	else
	{
		rank = card2->getRank();
		hand.push_back( card2 );
		hand.push_back( card1 );
	}
}

#if _MSC_VER
#pragma warning( disable : 4018 ) // Disable all warning 4018. For MSVC...
#endif

PokerRank::PokerRank( std::vector<const Card *> combo )
{
	//Prepping for help !
	std::sort( combo.begin(), combo.end(), compareCardPointers );
	byte suitCount[4] = {0,0,0,0};
//	std::cout << "Suits (" << combo.size() << ") : ";
	for( byte i = 0 ; i < (byte) combo.size() ; i++ )
	{
//		std::cout << combo.at(i);// (char) combo.at(i)->getSuit();
		suitCount[combo.at(i)->getSuit()-0x3]++;
	}
//	std::cout << std::endl;

	//Checking options
	//STRAIGHT_FLUSH ?
	{
		hand.clear();
		for( byte s = 0x3 ; s <= 0x6 ; s++ ) //find first flush as 5 or more of the same cards...
		{
			if( suitCount[s-0x3] >= 5 )
			{ //FLUSH !
				//STRAIGHT_FLUSH ?
				byte sequencingNumbers = 1;
				Card::RANK nextRankShouldBe = 0xFF; //for overflow !
				//Ace can be high AND low card !
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ ) //Check all Aces for the correct Suit
				{
					if( combo.at( i )->getSuit() == s ) //Only correct
					{
						hand.push_back( combo.at( i ) );
						nextRankShouldBe = 13; //We need a KING !
						rank = 14; //Ace !
					}
				}
				for( byte i = combo.size() - 1 ; i >= 0 ; i-- ) //check all card that have the same suit if they are in a subsequence, but from high to lower
				{
					if( combo.at( i )->getSuit() == s ) //Only Flush Straight Check
					{
						if( combo.at( i )->getRank() == nextRankShouldBe ) //It is the next card we expected ?
						{
							hand.push_back( combo.at( i ) );
							if( sequencingNumbers == 4 ) //We found the fifth subsequencing card, starting from the highest, so pack it an ship it !
							{
								category = STRAIGHT_FLUSH;
								return;
							}
							sequencingNumbers++;
						}
						else
						{
							hand.clear();
							hand.push_back( combo.at( i ) );
							sequencingNumbers = 1; //Start all over as it is not the next card in line
							rank = combo.at( i )->getRank(); //We start from the highest rank, that one we need to remember !
						}
						nextRankShouldBe = combo.at( i )->getRank() - 1; //We go from high to low
					}
				}
			}
		}
	}

	//FOUR_OF_A_KIND ?
	Card::RANK valueOfPair = 0;
	Card::RANK valueOfSecondPair = 0;
	Card::RANK valueOfTrio = 0;
	{
		byte identicalNumbers = 0;
		hand.clear();
		for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ ) //Check all Aces first
		{
			hand.push_back( combo.at( i ) );
			if( identicalNumbers == 1 )
			{
				valueOfPair = 14; //Pair of Aces
			}
			else if( identicalNumbers == 2 ) //No trio was found before, but this one is one !
			{
				valueOfTrio = 14; //Trio of Aces
				valueOfPair = 0; //not a pair...
			}
			else if( identicalNumbers == 3 ) //We found the fourth identical rank, starting from the highest, so pack it an ship it !
			{
				category = FOUR_OF_A_KIND;
				rank = 14; //Four Aces
				//Add highest card as kicker !
				if( combo.at( combo.size() - 1 )->getRank() != 1 ) //cannot be another Ace ! and if the last is Ace, there is no higher card.
				{
					hand.push_back( combo.at( combo.size() - 1 ) );
				}
				return;
			}
			identicalNumbers++;
		}
		hand.clear();
		hand.push_back( combo.at( combo.size() - 1 ) );
		identicalNumbers = 1;
		rank = combo.at( combo.size() - 1 )->getRank(); //start from the last card
		for( byte i = combo.size() - 2 ; i >= 0 && combo.at( i )->getRank() != 1 ; i-- ) //chech all the oher cards for four of the same rank, from high to low
		{
			if( rank == combo.at( i )->getRank() )
			{
				hand.push_back( combo.at( i ) );
				if( valueOfPair == 0 ) //No pair was found before, but this one is one !
				{
					valueOfPair = combo.at( i )->getRank(); //Current rank is part of a pair
				}
				else if( valueOfSecondPair == 0 && valueOfPair != combo.at( i )->getRank() ) //No second pair was found before, but this one is one !
				{
					valueOfSecondPair = combo.at( i )->getRank(); //Current rank is part of a pair
				}
				if( identicalNumbers == 2 && valueOfTrio == 0 ) //No trio was found before, but this one is one !
				{
					valueOfTrio = combo.at( i )->getRank(); //Current rank is part of a trio
					if( valueOfPair == combo.at( i )->getRank() ) //clean out the pair that bacame a trio
					{
						valueOfPair = 0;
					}
					else if( valueOfSecondPair == combo.at( i )->getRank() )
					{
						valueOfSecondPair = 0;
					}
				}
				if( identicalNumbers == 3 ) //We found the fourth identical rank, starting from the highest, so pack it an ship it !
				{
					//Add highest card as kicker !
					if( combo.at( 0 )->getRank() == 1 ) //There is an Ace ! And we are not taking Aces in this part
					{
						hand.push_back( combo.at( 0 ) );
					}
					else if( combo.at( combo.size() - 1 )->getRank() != combo.at( i )->getRank() ) //if the highest card is not one of the four, add this
					{
						hand.push_back( combo.at( combo.size() - 1 ) );
					}
					else if( i > 0 ) //else if there is still another card, add that
					{
						hand.push_back( combo.at( i - 1 ) );
					}
					category = FOUR_OF_A_KIND;
					return;
				}
				identicalNumbers++;
			}
			else
			{
				hand.clear();
				hand.push_back( combo.at( i ) );
				identicalNumbers = 1; //Start all over as it is not the same card
				rank = combo.at( i )->getRank(); //We start from the highest rank, that one we need to remember !
			}
		}
	}

	//FULL_HOUSE ?
	{
		if( valueOfTrio > 0 && valueOfPair > 0 )
		{
			category = FULL_HOUSE;
			hand.clear();
			rank = valueOfTrio;
			if( valueOfTrio == 14 ) //It was an Trio of Aces !
			{
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ )
				{
					hand.push_back( combo.at( i ) ); //Add the Trio of Aces !
				}
			}
			else
			{
				for( byte i = combo.size() - 1 ; i >= 0 ; i-- )
				{
					if( combo.at( i )->getRank() == valueOfTrio )
					{
						hand.push_back( combo.at( i ) ); //Add the Trio
					}
				}
			}
			if( valueOfPair == 14 ) //Only the first pair can be Aces !
			{
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ )
				{
					hand.push_back( combo.at( i ) ); //Add the first Pair of Aces !
					if( hand.size() == 5 )
					{
						return; //we have 5 pack it, ship it !
					}
				}
			}
			else
			{
				for( byte i = combo.size() - 1 ; i >= 0 ; i-- )
				{
					if( combo.at( i )->getRank() == valueOfPair )
					{
						hand.push_back( combo.at( i ) ); //Add the Pair
						if( hand.size() == 5 )
						{
							return; //we have 5 pack it, ship it !
						}
					}
				}
			}
			return;
		}
	}

	//FLUSH ?
	{
		hand.clear();
		rank = 0;
		for( byte s = 0x3 ; s <= 0x6 ; s++ ) //find first flush as 5 or more of the same cards...
		{
			if( suitCount[s-0x3] >= 5 )
			{ //FLUSH !
				category = FLUSH;
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ ) //Check all Aces for the correct Suit
				{
					if( combo.at( i )->getSuit() == s ) //Ace high flush !
					{
						hand.push_back( combo.at( i ) );
						rank = 14; //Ace !
					}
				}
				for( byte i = combo.size() - 1 ; i >= 0 && combo.at( i )->getRank() != 1 ; i-- ) //In reverse order we will find the highest rank with the correct suit
				{
					if( combo.at( i )->getSuit() == s ) //Only Flush Check
					{
						hand.push_back( combo.at( i ) );
						if( rank == 0 )
						{
							rank = combo.at( i )->getRank();
						}
						if( hand.size() == 5 ) //Stop when we have the 5 highest cards of the same suit !
						{
							return;
						}
					}
				}
			}
		}
	}

	//STRAIGHT?
	{
		byte sequencingNumbers = 1;
		Card::RANK nextRankShouldBe = 0xFF; //for overflow !
		hand.clear();
		//Ace can be high AND low card !
		if( combo.at( 0 )->getRank() == 1 ) //If we have an Ace, it must be in the front
		{
			hand.push_back( combo.at( 0 ) );
			nextRankShouldBe = 13; //We need a KING !
			rank = 14; //Ace !
		}
		for( byte i = combo.size() - 1 ; i >= 0 ; i-- ) //check all card if they are in a subsequence, but from high to lower
		{
			if( combo.at( i )->getRank() == nextRankShouldBe ) //It is the next card we expected ?
			{
				hand.push_back( combo.at( i ) );
				if( sequencingNumbers == 4 ) //We found the fifth subsequencing card, starting from the highest, so pack it an ship it !
				{
					category = STRAIGHT;
					return;
				}
				sequencingNumbers++;
				nextRankShouldBe = combo.at( i )->getRank() - 1; //We go from high to low
			}
			else if( combo.at( i )->getRank() - 1 != nextRankShouldBe ) //The rank should not be the same as previous, else skip this card
			{
				hand.clear();
				hand.push_back( combo.at( i ) );
				sequencingNumbers = 1; //Start all over as it is not the next card in line
				rank = combo.at( i )->getRank(); //We start from the highest rank, that one we need to remember !
				nextRankShouldBe = combo.at( i )->getRank() - 1; //We go from high to low
			}
		}
	}

	//THREE_OF_A_KIND ?
	{
		if( valueOfTrio > 0 )
		{
			category = THREE_OF_A_KIND;
			hand.clear();
			rank = valueOfTrio;
			if( valueOfTrio == 14 ) //It was an Trio of Aces !
			{
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ )
				{
					hand.push_back( combo.at( i ) ); //Add the Trio of Aces !
				}
			}
			else
			{
				for( byte i = combo.size() - 1 ; i >= 0 ; i-- )
				{
					if( combo.at( i )->getRank() == valueOfTrio )
					{
						hand.push_back( combo.at( i ) ); //Add the Trio
					}
				}
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ ) //First the Aces
				{
					hand.push_back( combo.at( i ) ); //Add the highest rest
					if( hand.size() == 5 )
					{
						return; //we have 5 pack it, ship it !
					}
				}
			}
			for( byte i = combo.size() - 1 ; i >= 0 && combo.at( i )->getRank() != 1 ; i-- ) //No more Aces !
			{
				if( combo.at( i )->getRank() != valueOfTrio )
				{
					hand.push_back( combo.at( i ) ); //Add the highest rest
					if( hand.size() == 5 )
					{
						return; //we have 5 pack it, ship it !
					}
				}
			}
			return;
		}
	}

	//TWO_PAIR ?
	{
		if( valueOfSecondPair > 0 )
		{
			category = TWO_PAIR;
			hand.clear();
			rank = valueOfPair;
			if( valueOfPair == 14 ) //Only the first pair can be Aces !
			{
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ )
				{
					hand.push_back( combo.at( i ) ); //Add the first Pair of Aces !
				}
			}
			else
			{
				for( byte i = combo.size() - 1 ; i >= 0 ; i-- )
				{
					if( combo.at( i )->getRank() == valueOfPair )
					{
						hand.push_back( combo.at( i ) ); //Add the Pair
					}
				}
			}
			for( byte i = combo.size() - 1 ; i >= 0 ; i-- )
			{
				if( combo.at( i )->getRank() == valueOfSecondPair )
				{
					hand.push_back( combo.at( i ) ); //Add the second Pair
				}
			}
			if( valueOfPair != 14 )
			{
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ ) //First the Aces
				{
					hand.push_back( combo.at( i ) ); //Add the highest rest
					if( hand.size() == 5 )
					{
						return; //we have 5 pack it, ship it !
					}
				}
			}
			for( byte i = combo.size() - 1 ; i >= 0 && combo.at( i )->getRank() != 1 ; i-- ) //No more Aces !
			{
				if( combo.at( i )->getRank() != valueOfPair && combo.at( i )->getRank() != valueOfSecondPair )
				{
					hand.push_back( combo.at( i ) ); //Add the highest rest
					return; //we have 5 pack it, ship it !
				}
			}
			return;
		}
	}

	//ONE_PAIR ?
	{
		if( valueOfPair > 0 )
		{
			category = ONE_PAIR;
			hand.clear();
			rank = valueOfPair;
			if( valueOfPair == 14 ) //Only the first pair can be Aces !
			{
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ )
				{
					hand.push_back( combo.at( i ) ); //Add the first Pair of Aces !
				}
			}
			else
			{
				for( byte i = combo.size() - 1 ; i >= 0 && combo.at( i )->getRank() != 1 ; i-- )
				{
					if( combo.at( i )->getRank() == valueOfPair )
					{
						hand.push_back( combo.at( i ) ); //Add the Pair
					}
				}
				for( byte i = 0 ; i < (byte) combo.size() && combo.at( i )->getRank() == 1 ; i++ ) //First the Aces
				{
					hand.push_back( combo.at( i ) ); //Add the highest rest
					if( hand.size() == 5 )
					{
						return; //we have 5 pack it, ship it !
					}
				}
			}
			for( byte i = combo.size() - 1 ; i >= 0 && combo.at( i )->getRank() != 1 ; i-- )
			{
				if( combo.at( i )->getRank() != valueOfPair )
				{
					hand.push_back( combo.at( i ) ); //Add the highest rest
					if( hand.size() == 5 )
					{
						return; //we have 5 pack it, ship it !
					}
				}
			}
			return;
		}
	}

	//HIGH_CARD !
	{
		category = HIGH_CARD;
		hand.clear();
		if( combo.at( 0 )->getRank() == 1 ) //If we have an Ace, it must be in the front
		{
			rank = 14; //Ace !
			hand.push_back( combo.at( 0 ) );
		}
		else
		{
			rank = combo.at( combo.size() - 1 )->getRank(); //Last card is the highest
		}
		for( byte i = combo.size() - 1 ; i >= 0 && combo.at( i )->getRank() != 1 ; i-- )
		{
			hand.push_back( combo.at( i ) ); //Add the highest rest
			if( hand.size() == 5 )
			{
				return; //we have 5 pack it, ship it !
			}
		}
		return;
	}
}

#if _MSC_VER
#pragma warning( default : 4018 ) // Enable all warning 4018. For MSVC...
#endif
