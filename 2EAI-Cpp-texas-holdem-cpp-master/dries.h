#ifndef DRIES_H
#define DRIES_H
#include "player.h"


namespace PXL2023
{
class Dries : public Player
{
public:
    Dries(unsigned char instance = 0) : Player(instance) {}
    virtual const char* getName(void) const override;
    virtual int willYouRaise(unsigned int totalBet) override;
};
}
#endif // DRIES_H
