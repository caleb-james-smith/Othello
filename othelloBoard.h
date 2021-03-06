#include <cstdio>
#include <cstring>
#include <set>
#include <utility>

#ifndef OTHELLOBOARD_h
#define OTHELLOBOARD_h

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

template<const int N, const int M> class OthelloBoard
{
private:
    unsigned char board_[N][M];
    std::set<std::pair<int, int>> moves_;
    unsigned char lastPlayer_;

    inline const bool onBoard(const int x, const int y) const
    {
        return x >= 0 && y >= 0 && x < N && y < M;
    }
    
public:
    OthelloBoard()
    {
        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < M; ++j)
            {
                board_[i][j] = 0;
            }
        }

        board_[N/2 - 1][M/2 -1] = 2;
        board_[N/2    ][M/2 -1] = 1;
        board_[N/2 - 1][M/2   ] = 1;
        board_[N/2    ][M/2   ] = 2;

        lastPlayer_ = 0;
    }

    unsigned char winner() const
    {
        int player[256];
        unsigned char winner = 0;
        int winnerCount = 0;
        for(int i = 0; i < 256; ++i) player[i] = 0;
        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < M; ++j)
            {
                ++player[board_[i][j]];
            }
        }
        for(int i = 1; i < 256; ++i)
        {
            if(player[i] > winnerCount)
            {
                winner = i;
                winnerCount = player[i];
            }
        }

        return winner;
    }

    int avaliableMoves(const unsigned char player)
    {
        moves_.clear();
        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < M; ++j)
            {
                //Skip anf non-blank square
                if(board_[i][j]) continue;

                //check blank squares for possible flips
                const int dirSet[] = {-1, 0, 1};
                for(const int& dirX : dirSet)
                {
                    for(const int& dirY : dirSet)
                    {
                        if(dirX || dirY)
                        {
                            int dir = 1;
                            for(int xp = i + dirX, yp = j + dirY; onBoard(xp, yp) && (!dirX || i != xp) && (!dirY || j != yp); xp += dir*dirX, yp += dir*dirY)
                            {
                                if(board_[xp][yp] == 0) break;
                                if(dir == -1) 
                                {
                                    moves_.insert(std::make_pair(i, j));
                                    break;
                                }
                                if(board_[xp][yp] == player) dir = -1;
                            }
                        }
                    }
                }
            }
        }
        
        return moves_.size();
    }

    bool play(const unsigned char player, const int x, const int y)
    {
        //Check if play is on board
        if(!onBoard(x, y)) return false;

        //Check if a play was made here 
        if(board_[x][y] != 0) return false;

        //Check if this is a legal move
        if(!moves_.count(std::make_pair(x, y))) return false;

        //Flip spots based on this move
        bool isLegal = false;
        const int dirSet[] = {-1, 0, 1};
        for(const int& dirX : dirSet)
        {
            for(const int& dirY : dirSet)
            {
                if(dirX || dirY)
                {
                    int dir = 1;
                    for(int xp = x + dirX, yp = y + dirY; onBoard(xp, yp) && (!dirX || x != xp) && (!dirY || y != yp); xp += dir*dirX, yp += dir*dirY)
                    {
                        if(board_[xp][yp] == 0) break;
                        if(dir == -1) 
                        {
                            board_[xp][yp] = player; 
                            isLegal = true;
                        }
                        if(board_[xp][yp] == player) dir = -1;
                    }
                }
            }
        }
    
        //Set play if move was legal
        //legal moves are on board, and must flip at least one opponent tile
        if(isLegal) 
        {
            board_[x][y] = player;
            lastPlayer_ = player;
        }
    
        return isLegal;
    }

    void print() const
    {
        printf("   ");
        for(int j = 0; j < M; ++j) printf("%2d", j);
        printf("\n");
        printf("   ");
        for(int j = 0; j < 2*M + 1; ++j) printf("-");
        printf("\n");

        for(int i = 0; i < N; ++i)
        {
            printf("%2d |", i);
            for(int j = 0; j < M; ++j)
            {
                unsigned char player = 0;
                if(moves_.count(std::make_pair(i, j)))
                {
                    player = 255;
                }
                else
                {
                    player = board_[i][j];
                }
                printf("%s%c%s|", playerToColor(player), playerToChar(player), ANSI_COLOR_RESET);
            }
            printf("\n");
        }

        printf("   ");
        for(int j = 0; j < 2*M + 1; ++j) printf("-");
        printf("\n");
    }

    inline char playerToChar(const unsigned char player) const
    {
        switch(player)
        {
        case 0:
            return ' ';
        case 1:
            return 'X';
        case 2:
            return 'O';
        case 255:
            return '+';
        default:
            return '-';
        }
    }

    inline const char* playerToColor(const unsigned char player) const
    {
        switch(player)
        {
        case 0:
            return "";
        case 1:
            return ANSI_COLOR_RED;
        case 2:
            return ANSI_COLOR_BLUE;
        case 255:
            return ANSI_COLOR_GREEN;
        default:
            return "";
        }
    }

    inline const OthelloBoard<8, 8>& getState() const
    {
        return *this;
    }
    
    inline const std::set<std::pair<int, int>>& getValidPlays() const
    {
        return moves_;
    }

    inline const unsigned char getLastPlayer() const
    {
        return lastPlayer_;
    }
};

#endif
