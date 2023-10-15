#include "dries.h"
#include <cstdlib> // for rand()
#include <cmath> // for abs()
#include "game.h"

namespace PXL2023
{

const char* Dries::getName() const
{
    return "Dries the Strategist";
}

int Dries::willYouRaise(unsigned int totalBet)
{
    PokerRank myHandRank = getRank();
    unsigned int blind = getGame()->getBlind();
    unsigned int myChips = getChips();

    // Randomizer that will allow for a more dynamic game, pretty much. I'm just hoping this will create a more dynamic game
    int randomFactor = rand() % 20;

    // Don't waste chips, gotta save em if they're running low hehe
    if (myChips < blind * 2)
    {
        return -1;
    }

    if (getTable()->isPreFlop()) // Pre-flop
    {
        auto firstCard = getHand().getFirstCard();
        auto secondCard = getHand().getSecondCard();

        unsigned int baseRaise = blind * 4;
        // Generally looking for the best type of cards to keep playing and raise more or less aggresively
        if (firstCard->getRank() == secondCard->getRank() && firstCard->getRank() >= 10)
        {
            return baseRaise + blind;
        }

        if (firstCard->getSuit() == secondCard->getSuit())
        {
            return baseRaise;
        }

        if (abs(firstCard->getRank() - secondCard->getRank()) == 1)
        {
            return baseRaise - blind;
        }

        if (firstCard->getRank() < 8 && secondCard->getRank() < 8)
        {
            return -1;
        }
        // For more unpredictability, raise if bot has enough chips
        if (randomFactor == 0 && myChips >= blind * 5)
        {
            return baseRaise + blind * 2;
        }
    }

    else if (getTable()->isFlop() || getTable()->isTurn() || getTable()->isRiver())
    {
        unsigned int myCurrentBet = getBet();

        switch (myHandRank.getCategory())
        {
        case ONE_PAIR:
            return (totalBet - myCurrentBet < blind * 3) ? 0 : blind * 2;
        case TWO_PAIR:
            return blind * 4;
        case THREE_OF_A_KIND:
            return blind * 5;
        case STRAIGHT:
            return blind * 6;
        default:
            if (totalBet - myCurrentBet > myChips * 0.2)
            {
                return -1; // Fold
            }
            return 0; // Call
        }
    }

    else if (getTable()->isTurn())
    {
        // Retrieve the hand rank of your bot during the Turn stage
        PokerRank myHandRank = getRank();
        unsigned int blind = getGame()->getBlind();
        unsigned int myChips = getChips();
        unsigned int myCurrentBet = getBet(); // The amount you've already bet

        switch (myHandRank.getCategory())
        {
        // Depending on how good your hand is, raise more or less
        case ONE_PAIR:
            return (totalBet - myCurrentBet < blind * 4) ? 0 : blind;
        case TWO_PAIR:
            return blind * 4 - myCurrentBet;
        case THREE_OF_A_KIND:
            return blind * 5 - myCurrentBet;
        case STRAIGHT:
            return blind * 6 - myCurrentBet;
        default:
            // Basic Folding strategy, fold if the bet is too high relative to your chips
            if (totalBet - myCurrentBet > myChips * 0.2)
            {
                return -1;  // Fold
            }
            // If no special hands, then call
            return 0;
        }
    }
    else if (getTable()->isRiver())
    {
        // Retrieve the hand rank of your bot during the River stage
        PokerRank myHandRank = getRank();
        unsigned int blind = getGame()->getBlind();
        unsigned int myChips = getChips();
        unsigned int myCurrentBet = getBet(); // The amount you've already bet

        switch (myHandRank.getCategory())
        {
        // Check if there is a good hand, depending on this raise more or less
        case ONE_PAIR:
            return (totalBet - myCurrentBet < blind * 4) ? 0 : -1;
        case TWO_PAIR:
            return blind * 5 - myCurrentBet;
        case THREE_OF_A_KIND:
            return blind * 6 - myCurrentBet;
        case STRAIGHT:
            return blind * 7 - myCurrentBet;
        default:
            // Basic Folding strategy, fold if the bet is too high relative to your chips
            if (totalBet - myCurrentBet > myChips * 0.2)
            {
                return -1;  // Fold
            }
            // If no special hands, then call
            return 0;
        }
    }

    return 0;
}
}
