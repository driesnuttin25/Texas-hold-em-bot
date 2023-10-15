# Texas-hold-em-bot
## Description
"Dries the Strategist" is a Poker bot designed to play Texas Hold'em. It makes decisions on whether to fold, call, or raise based on a variety of factors including the stage of the game, the hand it has, and the total bet on the table. The bot is implemented in C++ and is part of a larger Poker game simulation environment. At the time of testing it has a 97% win percentage against beginner bots.

## Features
- Intelligent decision-making based on the hand and the stage of the game (Pre-flop, Flop, Turn, River).
- Randomization techniques for unpredictable play.
- Considers stack size and pot size for decisions.
- Aggressive and conservative strategies for different hand types.
## How it Works
The bot's behavior is encapsulated in the willYouRaise(unsigned int totalBet) function. The function uses a variety of strategies based on the game stage:

## Pre-flop
- Aggressively raises with high pairs (10 and above).
- Raises with suited cards.
- Raises with connected cards (consecutive ranks).
- andom plays for unpredictability.
## Flop
- Raises based on the strength of the hand (One pair, Two pairs, Three of a kind, Straight).
## Turn
- Decisions are made considering the increased pot size and total bets.
- Varying levels of aggressive play based on hand strength.
## River
- Final aggressive plays with strong hands, while being cautious with weaker hands.
## Usage
Include the "dries.h" and "dries.cpp" files in your project and use it as part of the Poker game simulation.

## Extra information
This code was created as part of a task given to me by my professors. This will be graded and is not to be used in real poker games :) I mean go for it, just don't hold me responsible please.
