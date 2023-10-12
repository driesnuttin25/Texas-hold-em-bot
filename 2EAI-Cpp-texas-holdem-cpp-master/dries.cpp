#include "dries.h"
#include <cstdlib> 
#include <cmath> 
#include "game.h"

namespace PXL2022
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

    // Randomizer
    int randomFactor = rand() % 10; // Generate random number between 0 and 9

    if (getTable()->isPreFlop()) // Pre-flop
    {
        auto firstCard = getHand().getFirstCard();
        auto secondCard = getHand().getSecondCard();

        // High Pairs (10 and above) - Aggressive strategy
        if (firstCard->getRank() == secondCard->getRank() && firstCard->getRank() >= 10)
        {
            // Adding randomization to occasionally call instead of raising
            return (randomFactor > 1) ? (blind * 5) - totalBet : (blind * 4) - totalBet;
        }

        // Suited Cards - Pot building strategy
        if (firstCard->getSuit() == secondCard->getSuit())
        {
            // Consider stack size
            if (myChips >= blind * 10) {
                return (blind * 3) - totalBet;
            }
        }

        // Connected Cards - Conditional strategy
        if (abs(firstCard->getRank() - secondCard->getRank()) == 1)
        {
            // Add opponent behavior later (for now, raise 2x blind)
            return (blind * 2) - totalBet;
        }

        // Modified folding strategy based on hand strength
        if (totalBet > blind * 4)
        {
            PokerRank myHandRank = getRank(); // Getting your hand rank

            if (myHandRank.getCategory() == HIGH_CARD || myHandRank.getCategory() == ONE_PAIR && myHandRank.getRank() < 10) {
                return -1; // Fold if you have a high card or a low pair
            }
        }


        // Unpredictable play, occasionally call or make a small raise with less optimal hands
        if (randomFactor == 0 && myChips >= blind * 5) {
            return (blind * 2) - totalBet;
        }
    }

    else if (getTable()->isFlop())
    {
        PokerRank myHandRank = getRank();
        unsigned int blind = getGame()->getBlind();
        unsigned int myChips = getChips();

        if (myHandRank.getCategory() == ONE_PAIR)
        {
            return (blind * 2) - totalBet; // Raise 2x blind if you have a pair
        }
        else if (myHandRank.getCategory() == TWO_PAIR)
        {
            return (blind * 4) - totalBet; // Raise 4x blind if you have two pair
        }
        else if (myHandRank.getCategory() == THREE_OF_A_KIND)
        {
            return (blind * 6) - totalBet; // Raise 6x blind if you have three of a kind
        }
        else if (myHandRank.getCategory() == STRAIGHT)
        {
            return (blind * 5) - totalBet; // Raise 5x blind if you have a straight
        }
        else
        {
            return 0; // Check or Call
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
