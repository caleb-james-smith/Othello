#include "othelloPlayerRandom.h"

OthelloPlayerRandom::OthelloPlayerRandom() : randDist_(0, 10000000), mtRand_()
{
    
}

void OthelloPlayerRandom::returnPlay(const OthelloBoard<8, 8>& board, int& x, int& y)
{
    auto& moves = board.getValidPlays();
    const int nMoves = moves.size();
    const int randNum = randDist_(mtRand_);

    auto iter = moves.begin();
    for(int i = 0; i < (randNum%nMoves); ++i) ++iter;
    x = iter->first;
    y = iter->second;
}
