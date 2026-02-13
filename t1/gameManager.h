#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

enum class Winner { None = 0, Player1 = 1, Player2 = 2};

class GameManager
{
    int lifePlayer1, lifePlayer2;
    Winner winner;
    
    public:
        GameManager()
        {
            this->lifePlayer1 = 3;
            this->lifePlayer2 = 3;
            this->winner = Winner::None;
        }

        int getLifePlayer1() const { return lifePlayer1; }
        int getLifePlayer2() const { return lifePlayer2; }

        bool isGameOver() const;

        void decLifePlayer1();
        void decLifePlayer2();

        Winner getWinner() const { return this->winner; }
        
        void resetGame();
};

#endif