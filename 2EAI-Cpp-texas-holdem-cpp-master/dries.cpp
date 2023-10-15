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

    // Improved Randomizer
    int randomFactor = rand() % 20;

    // Basic fold conditions - don't waste chips
    if (myChips < blind * 2)
    {
        return -1;
    }

    if (getTable()->isPreFlop()) // Pre-flop
    {
        auto firstCard = getHand().getFirstCard();
        auto secondCard = getHand().getSecondCard();

        unsigned int baseRaise = blind * 4;

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
        case ONE_PAIR:
            // If you have one pair, consider calling or making a small raise
            return (totalBet - myCurrentBet < blind * 4) ? 0 : blind;
        case TWO_PAIR:
            // If you have two pairs, raise more aggressively
            return blind * 4 - myCurrentBet;
        case THREE_OF_A_KIND:
            // If you have three of a kind, raise even more aggressively
            return blind * 5 - myCurrentBet;
        case STRAIGHT:
            // With a straight, make a strong bet
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
        case ONE_PAIR:
            // With one pair, cautiously call if the total bet isn't too high
            return (totalBet - myCurrentBet < blind * 4) ? 0 : -1;
        case TWO_PAIR:
            // With two pairs, be more aggressive and raise
            return blind * 5 - myCurrentBet;
        case THREE_OF_A_KIND:
            // With three of a kind, be very aggressive
            return blind * 6 - myCurrentBet;
        case STRAIGHT:
            // With a straight, make a strong bet to maximize value
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
